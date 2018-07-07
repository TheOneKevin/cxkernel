/**
 * @file   malloc.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, June 30th 2018, 11:23:32 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T19:08:12-04:00
 */

#pragma once
#include "common.h"
#include "panic.h"
#include "arch/arch_paging.h"

#define ABORT PANIC("dlmalloc called abort()")
#define ABORT_ON_ASSERT_FAILURE 1
#define DL_DEBUG 0

#define LACKS_FCNTL_H
#define LACKS_TIME_H
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_MMAN_H
#define LACKS_STDLIB_H

#define NO_MALLOC_STATS 0
#define MALLOC_FAILURE_ACTION

#define HAVE_MMAP 0
#define HAVE_MORECORE 1

#define USE_LOCKS 0
#define USE_SPIN_LOCKS 0

#define malloc_getpagesize ARCH_PAGE_SIZE

/** Version identifier to allow people to support multiple versions */
#ifndef DLMALLOC_VERSION
#define DLMALLOC_VERSION 20806
#endif /* DLMALLOC_VERSION */

/*
#ifndef DLMALLOC_EXPORT
#define DLMALLOC_EXPORT extern
#endif
*/

#ifndef WIN32
#ifdef _WIN32
#define WIN32 1
#endif  /* _WIN32 */
#ifdef _WIN32_WCE
#define LACKS_FCNTL_H
#define WIN32 1
#endif /* _WIN32_WCE */
#endif  /* WIN32 */
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#define HAVE_MMAP 1
#define HAVE_MORECORE 0
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_MMAN_H
#define LACKS_STRING_H
#define LACKS_STRINGS_H
#define LACKS_SYS_TYPES_H
#define LACKS_ERRNO_H
#define LACKS_SCHED_H
#ifndef MALLOC_FAILURE_ACTION
#define MALLOC_FAILURE_ACTION
#endif /* MALLOC_FAILURE_ACTION */
#ifndef MMAP_CLEARS
#ifdef _WIN32_WCE /* WINCE reportedly does not clear */
#define MMAP_CLEARS 0
#else
#define MMAP_CLEARS 1
#endif /* _WIN32_WCE */
#endif /*MMAP_CLEARS */
#endif  /* WIN32 */

#if defined(DARWIN) || defined(_DARWIN)
/* Mac OSX docs advise not to use sbrk; it seems better to use mmap */
#ifndef HAVE_MORECORE
#define HAVE_MORECORE 0
#define HAVE_MMAP 1
/* OSX allocators provide 16 byte alignment */
#ifndef MALLOC_ALIGNMENT
#define MALLOC_ALIGNMENT ((size_t)16U)
#endif
#endif  /* HAVE_MORECORE */
#endif  /* DARWIN */

#ifndef LACKS_SYS_TYPES_H
#include "common.h"  /* For size_t */
#endif  /* LACKS_SYS_TYPES_H */

/* The maximum possible size_t value has all bits set */
#define MAX_SIZE_T           (~(size_t)0)

#ifndef USE_LOCKS /* ensure true if spin or recursive locks set */
#define USE_LOCKS  ((defined(USE_SPIN_LOCKS) && USE_SPIN_LOCKS != 0) || \
                    (defined(USE_RECURSIVE_LOCKS) && USE_RECURSIVE_LOCKS != 0))
#endif /* USE_LOCKS */

#if USE_LOCKS /* Spin locks for gcc >= 4.1, older gcc on x86, MSC >= 1310 */
#if ((defined(__GNUC__) &&                                              \
      ((__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)) ||      \
       defined(__i386__) || defined(__x86_64__))) ||                    \
     (defined(_MSC_VER) && _MSC_VER>=1310))
#ifndef USE_SPIN_LOCKS
#define USE_SPIN_LOCKS 1
#endif /* USE_SPIN_LOCKS */
#elif USE_SPIN_LOCKS
#error "USE_SPIN_LOCKS defined without implementation"
#endif /* ... locks available... */
#elif !defined(USE_SPIN_LOCKS)
#define USE_SPIN_LOCKS 0
#endif /* USE_LOCKS */

#ifndef ONLY_MSPACES
#define ONLY_MSPACES 0
#endif  /* ONLY_MSPACES */
#ifndef MSPACES
#if ONLY_MSPACES
#define MSPACES 1
#else   /* ONLY_MSPACES */
#define MSPACES 0
#endif  /* ONLY_MSPACES */
#endif  /* MSPACES */
#ifndef MALLOC_ALIGNMENT
#define MALLOC_ALIGNMENT ((size_t)(2 * sizeof(void *)))
#endif  /* MALLOC_ALIGNMENT */
#ifndef FOOTERS
#define FOOTERS 0
#endif  /* FOOTERS */
#ifndef ABORT
#define ABORT  abort()
#endif  /* ABORT */
#ifndef ABORT_ON_ASSERT_FAILURE
#define ABORT_ON_ASSERT_FAILURE 1
#endif  /* ABORT_ON_ASSERT_FAILURE */
#ifndef PROCEED_ON_ERROR
#define PROCEED_ON_ERROR 0
#endif  /* PROCEED_ON_ERROR */

#ifndef INSECURE
#define INSECURE 0
#endif  /* INSECURE */
#ifndef MALLOC_INSPECT_ALL
#define MALLOC_INSPECT_ALL 0
#endif  /* MALLOC_INSPECT_ALL */
#ifndef HAVE_MMAP
#define HAVE_MMAP 1
#endif  /* HAVE_MMAP */
#ifndef MMAP_CLEARS
#define MMAP_CLEARS 1
#endif  /* MMAP_CLEARS */
#ifndef HAVE_MREMAP
#ifdef linux
#define HAVE_MREMAP 1
#define _GNU_SOURCE /* Turns on mremap() definition */
#else   /* linux */
#define HAVE_MREMAP 0
#endif  /* linux */
#endif  /* HAVE_MREMAP */
#ifndef MALLOC_FAILURE_ACTION
#define MALLOC_FAILURE_ACTION  errno = ENOMEM;
#endif  /* MALLOC_FAILURE_ACTION */
#ifndef HAVE_MORECORE
#if ONLY_MSPACES
#define HAVE_MORECORE 0
#else   /* ONLY_MSPACES */
#define HAVE_MORECORE 1
#endif  /* ONLY_MSPACES */
#endif  /* HAVE_MORECORE */
#if !HAVE_MORECORE
#define MORECORE_CONTIGUOUS 0
#else   /* !HAVE_MORECORE */
#define MORECORE_DEFAULT sbrk
#ifndef MORECORE_CONTIGUOUS
#define MORECORE_CONTIGUOUS 1
#endif  /* MORECORE_CONTIGUOUS */
#endif  /* HAVE_MORECORE */
#ifndef DEFAULT_GRANULARITY
#if (MORECORE_CONTIGUOUS || defined(WIN32))
#define DEFAULT_GRANULARITY (0)  /* 0 means to compute in init_mparams */
#else   /* MORECORE_CONTIGUOUS */
#define DEFAULT_GRANULARITY ((size_t)64U * (size_t)1024U)
#endif  /* MORECORE_CONTIGUOUS */
#endif  /* DEFAULT_GRANULARITY */
#ifndef DEFAULT_TRIM_THRESHOLD
#ifndef MORECORE_CANNOT_TRIM
#define DEFAULT_TRIM_THRESHOLD ((size_t)2U * (size_t)1024U * (size_t)1024U)
#else   /* MORECORE_CANNOT_TRIM */
#define DEFAULT_TRIM_THRESHOLD MAX_SIZE_T
#endif  /* MORECORE_CANNOT_TRIM */
#endif  /* DEFAULT_TRIM_THRESHOLD */
#ifndef DEFAULT_MMAP_THRESHOLD
#if HAVE_MMAP
#define DEFAULT_MMAP_THRESHOLD ((size_t)256U * (size_t)1024U)
#else   /* HAVE_MMAP */
#define DEFAULT_MMAP_THRESHOLD MAX_SIZE_T
#endif  /* HAVE_MMAP */
#endif  /* DEFAULT_MMAP_THRESHOLD */
#ifndef MAX_RELEASE_CHECK_RATE
#if HAVE_MMAP
#define MAX_RELEASE_CHECK_RATE 4095
#else
#define MAX_RELEASE_CHECK_RATE MAX_SIZE_T
#endif /* HAVE_MMAP */
#endif /* MAX_RELEASE_CHECK_RATE */
#ifndef USE_BUILTIN_FFS
#define USE_BUILTIN_FFS 0
#endif  /* USE_BUILTIN_FFS */
#ifndef USE_DEV_RANDOM
#define USE_DEV_RANDOM 0
#endif  /* USE_DEV_RANDOM */
#ifndef NO_MALLINFO
#define NO_MALLINFO 0
#endif  /* NO_MALLINFO */
#ifndef MALLINFO_FIELD_TYPE
#define MALLINFO_FIELD_TYPE size_t //!< mallinfo type
#endif  /* MALLINFO_FIELD_TYPE */
#ifndef NO_MALLOC_STATS
#define NO_MALLOC_STATS 0
#endif  /* NO_MALLOC_STATS */
#ifndef NO_SEGMENT_TRAVERSAL
#define NO_SEGMENT_TRAVERSAL 0
#endif /* NO_SEGMENT_TRAVERSAL */

/**
 * mallopt tuning options.  SVID/XPG defines four standard parameter
 * numbers for mallopt, normally defined in malloc.h.  None of these
 * are used in this malloc, so setting them has no effect. But this
 * malloc does support the following options.
 */

//!@{
#define M_TRIM_THRESHOLD     (-1)
#define M_GRANULARITY        (-2)
#define M_MMAP_THRESHOLD     (-3)
//!@}

/* ------------------------ Mallinfo declarations ------------------------ */

#if !NO_MALLINFO

/** HAVE_USR_INCLUDE_MALLOC_H should be set if you have a
 * /usr/include/malloc.h file that includes a declaration of struct
 * mallinfo.  If so, it is included; else a compliant version is
 * declared below.  These must be precisely the same for mallinfo() to
 * work.  The original SVID version of this struct, defined on most
 * systems with mallinfo, declares all fields as ints. But some others
 * define as unsigned long. If your system defines the fields using a
 * type of different width than listed here, you MUST #include your
 * system version and #define HAVE_USR_INCLUDE_MALLOC_H.
 */

/* #define HAVE_USR_INCLUDE_MALLOC_H */

#ifdef HAVE_USR_INCLUDE_MALLOC_H
#include "/usr/include/malloc.h"
#else /* HAVE_USR_INCLUDE_MALLOC_H */
#ifndef STRUCT_MALLINFO_DECLARED
/* HP-UX (and others?) redefines mallinfo unless _STRUCT_MALLINFO is defined */
#define _STRUCT_MALLINFO
#define STRUCT_MALLINFO_DECLARED 1
/**
 * This version of malloc supports the standard SVID/XPG mallinfo
 * routine that returns a struct containing usage properties and
 * statistics. It should work on any system that has a
 * /usr/include/malloc.h defining struct mallinfo.  The main
 * declaration needed is the mallinfo struct that is returned (by-copy)
 * by mallinfo().  The malloinfo struct contains a bunch of fields that
 * are not even meaningful in this version of malloc.  These fields are
 * are instead filled by mallinfo() with other numbers that might be of
 * interest.
 */
struct mallinfo {
  MALLINFO_FIELD_TYPE arena;    /**< current total non-mmapped bytes allocated from system */
  MALLINFO_FIELD_TYPE ordblks;  /**< number of free chunks */
  MALLINFO_FIELD_TYPE smblks;   /**< always zero */
  MALLINFO_FIELD_TYPE hblks;    /**< current number of mmapped regions (always zero) */
  MALLINFO_FIELD_TYPE hblkhd;   /**< total bytes held in mmapped regions */
  MALLINFO_FIELD_TYPE usmblks;  /**< the maximum total allocated space. This will be greater than current total if trimming has occurred. */
  MALLINFO_FIELD_TYPE fsmblks;  /**< always zero */
  MALLINFO_FIELD_TYPE uordblks; /**< current total allocated space (normal or mmapped) */
  MALLINFO_FIELD_TYPE fordblks; /**< total free space */
  MALLINFO_FIELD_TYPE keepcost; /**< the maximum number of bytes that could ideally be released back to system via malloc_trim. ("ideally" means that it ignores page restrictions etc.) */
};

#endif /* STRUCT_MALLINFO_DECLARED */
#endif /* HAVE_USR_INCLUDE_MALLOC_H */
#endif /* NO_MALLINFO */

/**
 * Try to persuade compilers to inline. The most critical functions for
 * inlining are defined as macros, so these aren't used for them.
 */
#ifndef FORCEINLINE
  #if defined(__GNUC__)
#define FORCEINLINE __inline __attribute__ ((always_inline))
  #elif defined(_MSC_VER)
    #define FORCEINLINE __forceinline
  #endif
#endif
#ifndef NOINLINE
  #if defined(__GNUC__)
    #define NOINLINE __attribute__ ((noinline))
  #elif defined(_MSC_VER)
    #define NOINLINE __declspec(noinline)
  #else
    #define NOINLINE
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#ifndef FORCEINLINE
 #define FORCEINLINE inline
#endif
#endif /* __cplusplus */
#ifndef FORCEINLINE
 #define FORCEINLINE
#endif

#if !ONLY_MSPACES

/* ------------------- Declarations of public routines ------------------- */

#ifndef USE_DL_PREFIX
//! @{
//! Malloc functions without the dl prefix
#define calloc               dlcalloc               //!< @sa dlcalloc
#define free                 dlfree                 //!< @sa dlfree
#define malloc               dlmalloc               //!< @sa dlmalloc
#define memalign             dlmemalign             //!< @sa dlmemalign
#define posix_memalign       dlposix_memalign       //!< @sa dlposix_memalign
#define realloc              dlrealloc              //!< @sa dlrealloc
#define realloc_in_place     dlrealloc_in_place     //!< @sa dlrealloc_in_place
#define valloc               dlvalloc               //!< @sa dlvalloc
#define pvalloc              dlpvalloc              //!< @sa dlpvalloc
#define dlmallinfo           mallinfo               //!< Declared backwards? @sa mallinfo
#define mallopt              dlmallopt              //!< @sa dlmallopt
#define malloc_trim          dlmalloc_trim          //!< @sa dlmalloc_trim
#define malloc_stats         dlmalloc_stats         //!< @sa dlmalloc_stats
#define malloc_usable_size   dlmalloc_usable_size   //!< @sa dlmalloc_usable_size
#define malloc_footprint     dlmalloc_footprint     //!< @sa dlmalloc_footprint
#define malloc_max_footprint dlmalloc_max_footprint //!< @sa dlmalloc_max_footprint
#define malloc_footprint_limit      dlmalloc_footprint_limit     //!< @sa dlmalloc_footprint_limit
#define malloc_set_footprint_limit  dlmalloc_set_footprint_limit //!< @sa dlmalloc_set_footprint_limit
#define malloc_inspect_all   dlmalloc_inspect_all   //!< @sa dlmalloc_inspect_all
#define independent_calloc   dlindependent_calloc   //!< @sa dlindependent_calloc
#define independent_comalloc dlindependent_comalloc //!< @sa dlindependent_comalloc
#define bulk_free            dlbulk_free            //!< @sa dlbulk_free
//! @}
#endif /* USE_DL_PREFIX */

/**
 * Returns a pointer to a newly allocated chunk of at least n bytes, or
 * null if no space is available, in which case errno is set to @ref ENOMEM
 * on ANSI C systems.
 * 
 * @param n Size of memory to be allocated
 * 
 * @return Pointer to the newly allocated chunk. If n is zero, @ref malloc returns a
 * minimum-sized chunk. (The minimum size is 16 bytes on most 32bit systems,
 * and 32 bytes on 64bit systems).
 * @note Note that <tt>size_t</tt> is an unsigned type, so calls with
 * arguments that would be negative if signed are interpreted as
 * requests for huge amounts of space, which will often fail. The
 * maximum supported value of n differs across systems, but is in all
 * cases less than the maximum representable value of a <tt>size_t</tt>.
 */
/* DLMALLOC_EXPORT */ void* dlmalloc(size_t n);

/**
 * Releases the chunk of memory pointed to by p, that had been previously
 * allocated using @ref malloc or a related routine such as @ref realloc.
 * It has no effect if p is null.
 * 
 * @warning If p was not malloced or already freed, @ref free(p) will by default cause the current program to abort.
 * 
 * @param p Pointer to memory to be freed
 */
/* DLMALLOC_EXPORT */ void  dlfree(void* p);

/**
 * Allocates memory for an array of n_elements of size element_size and initializes
 * all bytes in the allocated storage to zero.
 * 
 * @param n_elements   Number of elements to allocate
 * @param element_size Size of each element
 * 
 * @return A pointer to n_elements * element_size bytes, with all locations
 * set to zero.
 */
/* DLMALLOC_EXPORT */ void* dlcalloc(size_t n_elements, size_t element_size);

/**
 * Resize chunk at p to size n.
 * 
 * @note The old unix realloc convention of allowing the last-free'd chunk
 * to be used as an argument to realloc is not supported.
 * 1. If p is null, @ref realloc() is equivalent to @ref malloc().
 * 2. If space is not available, realloc returns null, @ref errno is set (if on
 * ANSI) and p is NOT freed.
 * 3. If n is for fewer bytes than already held by p, the newly unused
 * space is lopped off and freed if possible. Realloc with a size
 * argument of zero (re)allocates a minimum-sized chunk.
 *
 * @param p Pointer to allocated memory
 * @param n New size
 * 
 * @return A pointer to a chunk of size n that contains the same data
 * as does chunk p up to the minimum of (n, p's size) bytes, or null
 * if no space is available.
 *
 * The returned pointer may or may not be the same as p. The algorithm
 * prefers extending p in most cases when possible, otherwise it
 * employs the equivalent of a malloc-copy-free sequence.
 */
/* DLMALLOC_EXPORT */ void* dlrealloc(void* p, size_t n);

/**
 * Resizes the space allocated for p to size n, <b>only if this can be
 * done without moving p</b> (i.e., only if there is adjacent space
 * available if n is greater than p's current allocated size, or n is
 * less than or equal to p's size). This may be used instead of plain
 * realloc if an alternative allocation strategy is needed upon failure
 * to expand space; for example, reallocation of a buffer that must be
 * memory-aligned or cleared. You can use realloc_in_place to trigger
 * these alternatives only when needed.
 *
 * @param p Pointer to allocated memory
 * @param n New memory size
 * 
 * @return p if successful; otherwise null.
 */
/* DLMALLOC_EXPORT */ void* dlrealloc_in_place(void* p, size_t n);

/**
 * Allocate chunk of n bytes, aligned
 * in accord with the alignment argument.
 * 
 * @note The alignment argument should be a power of two.
 * If the argument is not a power of two, the nearest greater power is used.
 * 8-byte alignment is guaranteed by normal malloc calls, so don't
 * bother calling memalign with an argument of 8 or less.
 * @warning Overreliance on memalign is a sure way to fragment space.
 * 
 * @param alignment Alignment of memory
 * @param n         Size of memory to allocate
 * 
 * @return A pointer to a newly allocated memory
 */
/* DLMALLOC_EXPORT */ void* dlmemalign(size_t alignment, size_t n);

/**
 * Allocates a chunk of n bytes, aligned in accord with the alignment
 * argument.
 * Differs from @ref memalign only in that it:
 * 1. assigns the allocated memory to *pp rather than returning it,
 * 2. fails and returns @ref EINVAL if the alignment is not a power of two
 * 3. fails and returns @ref ENOMEM if memory cannot be allocated.
 * 
 * \sa memalign
 * 
 * @param pp        Pointer to pointer. Will set the pointed pointer to point to the beginning of the allocated block. See above for example.
 * @param alignment Alignment of memory.
 * @param n         Size of memory to allocate
 * 
 * @return 0 on success, EINVAL or ENOMEM on error (see description)
 */
/* DLMALLOC_EXPORT */ int dlposix_memalign(void** pp, size_t alignment, size_t n);

/**
 * Equivalent to @ref memalign(pagesize, n), where pagesize is the page
 * size of the system.
 * @note If the pagesize is unknown, 4096 is used.
 * \sa dlmemalign(pagesize, n)
 */
/* DLMALLOC_EXPORT */ void* dlvalloc(size_t n);

/**
 * Sets tunable parameters. The format is to provide a
 * (parameter-number, parameter-value) pair. mallopt then sets the
 * corresponding parameter to the argument value if it can (i.e., so
 * long as the value is meaningful), and returns 1 if successful else
 * 0. To workaround the fact that mallopt is specified to use int,
 * not size_t parameters, the value -1 is specially treated as the
 * maximum unsigned size_t value.
 *
 * SVID/XPG/ANSI defines four standard param numbers for mallopt,
 * normally defined in @ref malloc.h.  None of these are use in this malloc,
 * so setting them has no effect. But this malloc also supports other
 * options in mallopt. See below for details. Briefly, supported
 * parameters are as follows (listed defaults are for "typical"
 * configurations):
 * | Symbol                 | Param # | Default     | Allowed param values            |
 * |------------------------|---------|-------------|---------------------------------|
 * | @ref M_TRIM_THRESHOLD  |   -1    | 2*1024*1024 | any   (-1 disables)             |
 * | @ref M_GRANULARITY     |   -2    | page size   | any power of 2 >= page size     |
 * | @ref M_MMAP_THRESHOLD  |   -3    | 256*1024    | any   (or 0 if no MMAP support) |
 *  
 * @param parameter_number Param number (see table above)
 * @param parameter_value  Parameter value
 * 
 * @return 1 if successful else 0
 */
/* DLMALLOC_EXPORT */ int dlmallopt(int parameter_number, int parameter_value);

/**
 * Gets the number of bytes obtained from the system. The total
 * number of bytes allocated by malloc, realloc etc., is less than this
 * value. Unlike mallinfo, this function returns only a precomputed
 * result, so can be called frequently to monitor memory consumption.
 * @warning Even if locks are otherwise defined, this function does not use them,
 * so results might not be up to date.
 * 
 * @return The number of bytes obtained from the system.
 */
/* DLMALLOC_EXPORT */ size_t dlmalloc_footprint(void);

/**
 * Gets the maximum number of bytes obtained from the system. This
 * value will be greater than current footprint if deallocated space
 * has been reclaimed by the system. The peak number of bytes allocated
 * by malloc, realloc etc., is less than this value. Unlike mallinfo,
 * this function returns only a precomputed result, so can be called
 * frequently to monitor memory consumption.
 * @warning Even if locks are otherwise defined, this function does not use them,
 * so results might not be up to date.
 * 
 * @return The maximum number of bytes obtained from the system.
 */
/* DLMALLOC_EXPORT */ size_t dlmalloc_max_footprint(void);

/**
 * Gets the number of bytes that the heap is allowed to obtain from
 * the system, returning the last value returned by
 * malloc_set_footprint_limit, or the maximum size_t value if
 * never set. The returned value reflects a permission.
 * @warning There is no guarantee that this number of bytes can actually
 * be obtained from the system.
 * 
 * @return The number of bytes that the heap is allowed to obtain from
 * the system
 */
/* DLMALLOC_EXPORT */ size_t dlmalloc_footprint_limit();
/**
 * Sets the maximum number of bytes to obtain from the system, causing
 * failure returns from malloc and related functions upon attempts to
 * exceed this value. <b>The argument value may be subject to page
 * rounding to an enforceable limit; this actual value is returned.</b>
 * Using an argument of the maximum possible <tt>size_t</tt> effectively
 * disables checks. If the argument is less than or equal to the
 * current malloc_footprint, then all future allocations that require
 * additional system memory will fail. However, invocation cannot
 * retroactively deallocate existing used memory.
 * 
 * @param bytes Limit in bytes
 * 
 * @return Actual limit that was set
 */
size_t dlmalloc_set_footprint_limit(size_t bytes);

#if MALLOC_INSPECT_ALL
/*
  malloc_inspect_all(void(*handler)(void *start,
                                    void *end,
                                    size_t used_bytes,
                                    void* callback_arg),
                      void* arg);
  Traverses the heap and calls the given handler for each managed
  region, skipping all bytes that are (or may be) used for bookkeeping
  purposes.  Traversal does not include include chunks that have been
  directly memory mapped. Each reported region begins at the start
  address, and continues up to but not including the end address.  The
  first used_bytes of the region contain allocated data. If
  used_bytes is zero, the region is unallocated. The handler is
  invoked with the given callback argument. If locks are defined, they
  are held during the entire traversal. It is a bad idea to invoke
  other malloc functions from within the handler.

  For example, to count the number of in-use chunks with size greater
  than 1000, you could write:
  static int count = 0;
  void count_chunks(void* start, void* end, size_t used, void* arg) {
    if (used >= 1000) ++count;
  }
  then:
    malloc_inspect_all(count_chunks, NULL);

  malloc_inspect_all is compiled only if MALLOC_INSPECT_ALL is defined.
*/
/* DLMALLOC_EXPORT */ void dlmalloc_inspect_all(void(*handler)(void*, void *, size_t, void*),
                           void* arg);

#endif /* MALLOC_INSPECT_ALL */

#if !NO_MALLINFO

/**
 * Returns (by copy) a struct containing various summary statistics. 
 * \sa struct mallinfo
 * @note Because these fields are ints, but internal bookkeeping may
 * be kept as longs, the reported values may wrap around zero and
 * thus be inaccurate.
 * 
 * @return struct @ref mallinfo 
 */
/* DLMALLOC_EXPORT */ struct mallinfo dlmallinfo(void);
#endif /* NO_MALLINFO */

/**
 * independent_calloc is similar to @ref calloc, but instead of returning a
 * single cleared space, it returns an array of pointers to n_elements
 * independent elements that can hold contents of size elem_size, each
 * of which starts out cleared, and can be independently freed,
 * realloc'ed etc. The elements are guaranteed to be adjacently
 * allocated (this is not guaranteed to occur with multiple callocs or
 * mallocs), which may also improve cache locality in some
 * applications.
 *
 * The "chunks" argument is optional (i.e., may be null, which is
 * probably the most typical usage). If it is null, the returned array
 * is itself dynamically allocated and should also be freed when it is
 * no longer needed. Otherwise, the chunks array must be of at least
 * n_elements in length. It is filled in with the pointers to the
 * chunks.
 *
 * In either case, independent_calloc returns this pointer array, or
 * null if the allocation failed.  If n_elements is zero and "chunks"
 * is null, it returns a chunk representing an array with zero elements
 * (which should be freed if not wanted).
 *
 * @node Each element must be freed when it is no longer needed. This can be
 * done all at once using @ref bulk_free.
 *
 * independent_calloc simplifies and speeds up implementations of many
 * kinds of pools.  It may also be useful when constructing large data
 * structures that initially have a fixed number of fixed-sized nodes,
 * but the number is not known at compile time, and some of the nodes
 * may later need to be freed. For example:
 * @code{.c}
 * struct Node { int item; struct Node* next; };
 *
 * struct Node* build_list() {
 *   struct Node** pool;
 *   int n = read_number_of_nodes_needed();
 *   if (n <= 0) return 0;
 *   pool = (struct Node**)(independent_calloc(n, sizeof(struct Node), 0);
 *   if (pool == 0) die();
 *   // organize into a linked list...
 *   struct Node* first = pool[0];
 *   for (i = 0; i < n-1; ++i)
 *     pool[i]->next = pool[i+1];
 *   free(pool);     // Can now free the array (or not, if it is needed later)
 *   return first;
 * }
 * @endcode
 * 
 * @param n_elements   Number of elements to allocate
 * @param element_size Size of each element
 * @param chunks       Optional pointer to array and must be at least n_elements long. Will be filled in with the pointers to the chunks.
 * 
 * @return An array of pointers to the elements allocated
 */
/* DLMALLOC_EXPORT */ void** dlindependent_calloc(size_t n_elements, size_t element_size, void* chunks[]);

/**
  * independent_comalloc allocates, all at once, a set of n_elements
 * chunks with sizes indicated in the "sizes" array. It returns
 * an array of pointers to these elements, each of which can be
 * independently freed, realloc'ed etc. The elements are guaranteed to
 * be adjacently allocated (this is not guaranteed to occur with
 * multiple callocs or mallocs), which may also improve cache locality
 * in some applications.
 *
 * The "chunks" argument is optional (i.e., may be null). If it is null
 * the returned array is itself dynamically allocated and should also
 * be freed when it is no longer needed. Otherwise, the chunks array
 * must be of at least n_elements in length. It is filled in with the
 * pointers to the chunks.
 *
 * In either case, independent_comalloc returns this pointer array, or
 * null if the allocation failed.  If n_elements is zero and chunks is
 * null, it returns a chunk representing an array with zero elements
 * (which should be freed if not wanted).
 *
 * Each element must be freed when it is no longer needed. This can be
 * done all at once using bulk_free.
 *
 * independent_comallac differs from independent_calloc in that each
 * element may have a different size, and also that it does not
 * automatically clear elements.
 *
 * independent_comalloc can be used to speed up allocation in cases
 * where several structs or objects must always be allocated at the
 * same time.  For example:
 * @code{.c}
 * struct Head { ... }
 * struct Foot { ... }
 *
 * void send_message(char* msg) {
 *   int msglen = strlen(msg);
 *   size_t sizes[3] = { sizeof(struct Head), msglen, sizeof(struct Foot) };
 *   void* chunks[3];
 *   if (independent_comalloc(3, sizes, chunks) == 0)
 *     die();
 *   struct Head* head = (struct Head*)(chunks[0]);
 *   char*        body = (char*)(chunks[1]);
 *   struct Foot* foot = (struct Foot*)(chunks[2]);
 *   // ...
 * }
 * @endcode
 * In general though, independent_comalloc is worth using only for
 * larger values of n_elements. For small values, you probably won't
 * detect enough difference from series of malloc calls to bother.
 *
 * @warning Overuse of independent_comalloc can increase overall memory usage,
 * since it cannot reuse existing noncontiguous small chunks that
 * might be available for some of the elements.
 * 
 * @param n_elements Number of elements to allocate
 * @param sizes      Sizes of each element
 * @param chunks     Optional pointer to array and must be at least n_elements long. Will be filled in with the pointers to the chunks.
 * 
 * @return an array of pointers to the allocated elements
 */
/* DLMALLOC_EXPORT */ void** dlindependent_comalloc(size_t n_elements, size_t sizes[], void* chunks[]);

/**
 * Frees and clears (sets to null) each non-null pointer in the given
 * array. This is likely to be faster than freeing them one-by-one.
 * If footers are used, pointers that have been allocated in different
 * mspaces are not freed or cleared, and the count of all such pointers
 * is returned.
 * @note For large arrays of pointers with poor locality, it
 * may be worthwhile to sort this array before calling bulk_free.
 *
 * @param array      Array of elements to free
 * @param n_elements Number of elements to free
 * 
 * @return Number of elements freed
 */
/* DLMALLOC_EXPORT */ size_t  dlbulk_free(void* array[], size_t n_elements);

/**
 * Equivalent to valloc(minimum-page-that-holds(n)), that is, it will
 * round up n to nearest pagesize.
 * 
 * \sa valloc
 */
/* DLMALLOC_EXPORT */ void*  dlpvalloc(size_t);

/**
 * If possible, gives memory back to the system (via negative arguments
 * to @ref sbrk) if there is unused memory at the `high' end of the malloc
 * pool or in unused MMAP segments. You can call this after freeing
 * large blocks of memory to potentially reduce the system-level memory
 * requirements of a program. However, it cannot guarantee to reduce
 * memory. Under some allocation patterns, some large free blocks of
 * memory will be locked between two used chunks, so they cannot be
 * given back to the system.
 *
 * @param pad The amount of free
 * trailing space to leave untrimmed. If this argument is zero, only
 * the minimum amount of memory to maintain internal data structures
 * will be left. Non-zero arguments can be supplied to maintain enough
 * trailing space to service future expected allocations without having
 * to re-obtain memory from the system.
 * 
 * @return    1 if it actually released any memory, else 0.
 */
/* DLMALLOC_EXPORT */ int  dlmalloc_trim(size_t pad);

/**
 * Prints on stderr (@ref STREAM_ERR) the amount of space obtained from the system (both
 * via @ref sbrk and mmap), the maximum amount (which may be more than
 * current if @ref malloc_trim and/or munmap got called), and the current
 * number of bytes allocated via @ref malloc (or realloc, etc) but not yet
 * freed. Note that this is the number of bytes allocated, not the
 * number requested. It will be larger than the number requested
 * because of alignment and bookkeeping overhead. Because it includes
 * alignment wastage as being in use, this figure may be greater than
 * zero even when no user-level chunks are allocated.
 *
 * The reported current and maximum system memory can be inaccurate if
 * a program makes other calls to system memory allocation functions
 * (normally @ref sbrk) outside of malloc.
 *
 * malloc_stats prints only the most commonly interesting statistics.
 * More information can be obtained by calling @ref mallinfo.
 */
/* DLMALLOC_EXPORT */ void  dlmalloc_stats(void);

/**
 * Gets the number of bytes you can actually use in
 * an allocated chunk, which may be more than you requested (although
 * often not) due to alignment and minimum size constraints.
 * You can use this many bytes without worrying about
 * overwriting other allocated objects. This is not a particularly great
 * programming practice. malloc_usable_size can be more useful in
 * debugging and assertions, for example:
 * @code{.c}
 * p = malloc(n);
 * assert(malloc_usable_size(p) >= 256);
 * @endcode
 * 
 * @param p Pointer to an allocated chunk of memory
 * 
 * @return The number of usable bytes in an allocated chunk
 */
/* DLMALLOC_EXPORT */ size_t dlmalloc_usable_size(void* p);

#endif /* ONLY_MSPACES */

#if MSPACES

/*
  mspace is an opaque type representing an independent
  region of space that supports mspace_malloc, etc.
*/
typedef void* mspace;

/*
  create_mspace creates and returns a new independent space with the
  given initial capacity, or, if 0, the default granularity size.  It
  returns null if there is no system memory available to create the
  space.  If argument locked is non-zero, the space uses a separate
  lock to control access. The capacity of the space will grow
  dynamically as needed to service mspace_malloc requests.  You can
  control the sizes of incremental increases of this space by
  compiling with a different DEFAULT_GRANULARITY or dynamically
  setting with mallopt(M_GRANULARITY, value).
*/
/* DLMALLOC_EXPORT */ mspace create_mspace(size_t capacity, int locked);

/*
  destroy_mspace destroys the given space, and attempts to return all
  of its memory back to the system, returning the total number of
  bytes freed. After destruction, the results of access to all memory
  used by the space become undefined.
*/
/* DLMALLOC_EXPORT */ size_t destroy_mspace(mspace msp);

/*
  create_mspace_with_base uses the memory supplied as the initial base
  of a new mspace. Part (less than 128*sizeof(size_t) bytes) of this
  space is used for bookkeeping, so the capacity must be at least this
  large. (Otherwise 0 is returned.) When this initial space is
  exhausted, additional memory will be obtained from the system.
  Destroying this space will deallocate all additionally allocated
  space (if possible) but not the initial base.
*/
/* DLMALLOC_EXPORT */ mspace create_mspace_with_base(void* base, size_t capacity, int locked);

/*
  mspace_track_large_chunks controls whether requests for large chunks
  are allocated in their own untracked mmapped regions, separate from
  others in this mspace. By default large chunks are not tracked,
  which reduces fragmentation. However, such chunks are not
  necessarily released to the system upon destroy_mspace.  Enabling
  tracking by setting to true may increase fragmentation, but avoids
  leakage when relying on destroy_mspace to release all memory
  allocated using this space.  The function returns the previous
  setting.
*/
/* DLMALLOC_EXPORT */ int mspace_track_large_chunks(mspace msp, int enable);


/*
  mspace_malloc behaves as malloc, but operates within
  the given space.
*/
/* DLMALLOC_EXPORT */ void* mspace_malloc(mspace msp, size_t bytes);

/*
  mspace_free behaves as free, but operates within
  the given space.

  If compiled with FOOTERS==1, mspace_free is not actually needed.
  free may be called instead of mspace_free because freed chunks from
  any space are handled by their originating spaces.
*/
/* DLMALLOC_EXPORT */ void mspace_free(mspace msp, void* mem);

/*
  mspace_realloc behaves as realloc, but operates within
  the given space.

  If compiled with FOOTERS==1, mspace_realloc is not actually
  needed.  realloc may be called instead of mspace_realloc because
  realloced chunks from any space are handled by their originating
  spaces.
*/
/* DLMALLOC_EXPORT */ void* mspace_realloc(mspace msp, void* mem, size_t newsize);

/*
  mspace_calloc behaves as calloc, but operates within
  the given space.
*/
/* DLMALLOC_EXPORT */ void* mspace_calloc(mspace msp, size_t n_elements, size_t elem_size);

/*
  mspace_memalign behaves as memalign, but operates within
  the given space.
*/
/* DLMALLOC_EXPORT */ void* mspace_memalign(mspace msp, size_t alignment, size_t bytes);

/*
  mspace_independent_calloc behaves as independent_calloc, but
  operates within the given space.
*/
/* DLMALLOC_EXPORT */ void** mspace_independent_calloc(mspace msp, size_t n_elements,
                                 size_t elem_size, void* chunks[]);

/*
  mspace_independent_comalloc behaves as independent_comalloc, but
  operates within the given space.
*/
/* DLMALLOC_EXPORT */ void** mspace_independent_comalloc(mspace msp, size_t n_elements,
                                   size_t sizes[], void* chunks[]);

/*
  mspace_footprint() returns the number of bytes obtained from the
  system for this space.
*/
/* DLMALLOC_EXPORT */ size_t mspace_footprint(mspace msp);

/*
  mspace_max_footprint() returns the peak number of bytes obtained from the
  system for this space.
*/
/* DLMALLOC_EXPORT */ size_t mspace_max_footprint(mspace msp);


#if !NO_MALLINFO
/*
  mspace_mallinfo behaves as mallinfo, but reports properties of
  the given space.
*/
/* DLMALLOC_EXPORT */ struct mallinfo mspace_mallinfo(mspace msp);
#endif /* NO_MALLINFO */

/*
  malloc_usable_size(void* p) behaves the same as malloc_usable_size;
*/
/* DLMALLOC_EXPORT */ size_t mspace_usable_size(const void* mem);

/*
  mspace_malloc_stats behaves as malloc_stats, but reports
  properties of the given space.
*/
/* DLMALLOC_EXPORT */ void mspace_malloc_stats(mspace msp);

/*
  mspace_trim behaves as malloc_trim, but
  operates within the given space.
*/
/* DLMALLOC_EXPORT */ int mspace_trim(mspace msp, size_t pad);

/*
  An alias for mallopt.
*/
/* DLMALLOC_EXPORT */ int mspace_mallopt(int, int);

#endif /* MSPACES */

#ifdef __cplusplus
}  /* end of extern "C" */
#endif /* __cplusplus */
