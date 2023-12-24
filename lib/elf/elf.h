#pragma once

#include <ebl/status.h>
#include <ebl/type_traits.h>

#include "sysv_elf_abi.h"

namespace elf {

   class Context {
     private:
      struct Context32;
      struct Context64;
      template <typename U>
      struct header;

     public:
      static Result<void> load(Context& ctx, void* img);
      bool is_64bits() const { return is_64bits_; }
      template <typename T>
      header<T> get_header() {
         static_assert(ebl::always_false<T>::value, "Unsupported type <T> for get_header()");
      }
      template <>
      header<elf64_phdr_t> get_header<elf64_phdr_t>() {
         return header<elf64_phdr_t>{impl64_.phdr, impl64_.img->e_phnum};
      }
      // FIXME: Should this be genericized too?
      void* get_image() const { return is_64bits_ ? (void*)impl64_.img : (void*)impl32_.img; }

     public:
      Context() {}

     private:
      struct Context32 {
         Result<void> load(elf32_ehdr_t* hdr);
         elf32_ehdr_t* img;
         elf32_shdr_t* shdr;
         elf32_phdr_t* phdr;
         elf32_shdr_t* symtab;
         elf32_shdr_t* strtab;
      };

      struct Context64 {
         Result<void> load(elf64_ehdr_t* hdr);
         elf64_shdr_t* find_section(const char* name);
         elf64_addr_t get_symbol_addr(elf64_addr_t addr);
         char* get_symbol_name(elf64_addr_t addr);
         elf64_ehdr_t* img;
         elf64_shdr_t* shdr;
         elf64_phdr_t* phdr;
         elf64_shdr_t* symtab;
         elf64_shdr_t* strtab;
      };

     private:
      template <typename U>
      struct header {
        private:
         struct iterator {
            iterator(U* hdr) : hdr_{hdr} {}
            iterator& operator++() {
               hdr_++;
               return *this;
            }
            bool operator==(const iterator& a) const { return hdr_ == a.hdr_; }
            bool operator!=(const iterator& a) const { return hdr_ != a.hdr_; }
            U& operator*() const { return *hdr_; }

           private:
            U* hdr_;
         };

        public:
         header(U* hdr, uint16_t limit) : hdr_{hdr}, limit_{limit} {}
         iterator begin() const { return iterator{hdr_}; }
         iterator end() const { return iterator{&hdr_[limit_]}; }

        private:
         U* hdr_;
         uint16_t limit_;
      };

     private:
      union {
         Context64 impl64_;
         Context32 impl32_;
      };
      bool is_64bits_;
   };

} // namespace elf
