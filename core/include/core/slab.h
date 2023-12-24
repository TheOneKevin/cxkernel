#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/thread_safety.h>

#include "arch/types.h"
#include "core/mutex.h"

namespace kmem {
   void bootstrap();
   void* alloc(unsigned int size);
   void free(void* obj);
} // namespace kmem

namespace kmem_private {
   struct slabcache;
   struct slab;
   struct objctl;

   using slab_list_head = ebl::IntrusiveList<slab>;
   using slabcache_list_head = ebl::IntrusiveList<slabcache>;

   static inline unsigned int num_objects_per_slab(unsigned int size, uint16_t order);
   constexpr unsigned int align_up(unsigned int value, uint16_t order) {
      // FIXME: Use ebl::align_up instead
      return (value + ((1 << (unsigned int)order) - 1)) & ~((1 << (unsigned int)order) - 1);
   }

   constexpr int cache_name_max = 32;
   constexpr uint16_t default_order = 3;
   static_assert((1 << default_order) <= arch::page_size,
                 "2^default_order must be <= size of page");

   struct objctl {
      objctl* next_free_obj; //!< Pointer to the next free object within the slab
   };

#define G GUARDED_BY(lock)

   struct slab : ebl::IntrusiveListNode<slab> {
      slab(slabcache* parent, objctl* objctl, void* objects)
            : lock{},
              parent{parent},
              inuse{0},
              last_alloc_time{0},
              objctl{objctl},
              objects{objects} {}
      mutable core::Mutex lock;      //!< Lock for this slab
      slabcache* const parent;       //!< Parent slabcache
      unsigned int inuse G;          //!< Number of objects in use (not free)
      timestamp_t last_alloc_time G; //!< Time of last allocation, used for reaping
      objctl* objctl G;              //!< Pointer to the first free object in this slab
      void* const objects;           //!< Pointer to the first object in this slab
   };

   struct slabcache : ebl::IntrusiveListNode<slabcache> {
      slabcache(const char* name, unsigned int size, uint16_t order)
            : lock{},
              freelist{},
              partiallist{},
              fulllist{},
              name{},
              order{order},
              slab_objsize{size},
              slab_nobjects{num_objects_per_slab(size, order)},
              slab_last_color{0} {
         for(int i = 0; i < cache_name_max; ++i) this->name[i] = name[i];
      }
      slabcache(unsigned int size, uint16_t order)
            : lock{},
              freelist{},
              partiallist{},
              fulllist{},
              name{},
              order{order},
              slab_objsize{size},
              slab_nobjects{num_objects_per_slab(size, order)},
              slab_last_color{0} {}
      mutable core::Mutex lock;            //!< Lock for this slabcache
      slab_list_head freelist G;           //!< List of free slabs
      slab_list_head partiallist G;        //!< List of partially full slabs
      slab_list_head fulllist G;           //!< List of full slabs
      /*const*/ char name[cache_name_max]; //!< Name of this slabcache
      const uint16_t order;                //!< 2^order byte alignment of objects
      const unsigned int slab_objsize; //!< Size of an object (in bytes) aligned to 2^order bytes
      unsigned int slab_nobjects G;    //!< Number of objects per slab
      unsigned int slab_last_color G;  //!< Last color used for slab allocation
   };

   unsigned int num_objects_per_slab(unsigned int size, uint16_t order) {
      return (arch::page_size - align_up(sizeof(slab), order)) / align_up(size, order);
   }

#undef G
} // namespace kmem_private
