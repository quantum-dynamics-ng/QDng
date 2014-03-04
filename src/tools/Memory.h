/*
 * Memory.h
 *
 *  Created on: 22.10.2011
 *      Author: markus
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "tools/Exception.h"
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
 #define QDMEM_BLOCK_SIZE 4096
#else
 #define QDMEM_BLOCK_SIZE 1    // In debug mode we want valgrind to detect possible segfaults
#endif

#ifdef HAVE_AVX
 #define QDLIB_DATA_ALIGNMENT 32    /* 32 byte - Alignment for SIMD (AVX) */
#else
 #define QDLIB_DATA_ALIGNMENT 16    /* 16 byte - Alignment for SIMD (SSE2) */
#endif

namespace QDLIB
{

   /**
    * Memory management class.
    *
    * This class is intended to be used for allocating memory
    * for numerical storage. Large scale memory is efficiently handled here.
    * Also Limitations are handled here.
    */
   class Memory
   {
      private:
         class SlotEntry {
            public:
               void *p;       // pointer to block
               size_t size;   //  number of blocks
               bool free;    // used/unused

               SlotEntry() : p(NULL), size(0), free(true) {}
               SlotEntry(void* ptr, size_t s) : p(ptr), size(s), free(false) {}
         };

         Memory();
         Memory(Memory &ref) {};
         ~Memory();

         map<void*, SlotEntry>   slot_map; /* Memory slots allocated with OS  */

         /* Memory slots allocated with OS* - Free slots index */
         multimap<size_t, map<void*, SlotEntry>::iterator> free_map;

         size_t _used;            /* amount of allocated mem */
         size_t _MaxMem;          /* upper bound set user or default value */
         size_t _MaxUsed;         /* Maximum used memory (peak value) */

      public:

         /**
          * Get an instance of the singleton.
          */
         static Memory& Instance()
         {
            static Memory ref; /* Make it singleton */
            return ref;
         }

         size_t MaximumSize() const;
         void Reconfigure();
         size_t CurrentSizeAvail() const;
         size_t CurrentSizeUsed() const;

         size_t SetMaximumSize();

         size_t MaximumSizeUsed() const;

         static size_t ReadFromString(string s);
         static string Format(size_t value);

         static void Copy(char *dst, char* src, size_t size);
         void Align(void **p, size_t size);
         void Free(void *p);
         void Cleanup();
   };

}

#endif /* MEMORY_H_ */
