#include <ebl/assert.h>
#include <malloc.h>
#include <exception>
#include <iostream>

void assert(bool condition, std::source_location const &loc) {
   if(!condition) [[unlikely]] {
      std::cout << "Assertion failed at " << loc.file_name() << ":" << loc.line() << " "
                << loc.function_name() << std::endl;
      throw std::exception();
   }
}

void assert(bool condition, const char *message, std::source_location const &loc) {
   if(!condition) [[unlikely]] {
      std::cout << "Assertion failed at " << loc.file_name() << ":" << loc.line() << " "
                << loc.function_name() << std::endl;
      std::cout << "Message: " << message << std::endl;
      throw std::exception();
   }
}

void panic(const char *message, std::source_location const &loc) {
   std::cout << "Kernel panicked at " << loc.file_name() << ":" << loc.line() << " "
             << loc.function_name() << std::endl;
   std::cout << "Message: " << message << std::endl;
   throw std::exception();
}

namespace kmem {
   void* alloc(unsigned int size) { return malloc(size); }
   void free(void* obj) { ::free(obj); }
} // namespace kmem
