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
#include <list>
#include <stdio.h>
#include <stdlib.h>

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
               void *p;
               size_t size;
               bool free;
               SlotEntry() : p(NULL), size(0), free(true) {}
               SlotEntry(size_t s) : p(NULL), size(s), free(false) {}
         };

         static bool Compare_SlotEntry(const SlotEntry &a, const SlotEntry &b);

         void ResizeSlotBuffer();

         Memory();
         Memory(Memory &ref) {};
         ~Memory();

         SlotEntry *_Slots;      /* Memory slots allocated with OS  */
         SlotEntry **_SlotsF;      /* Memory slots allocated with OS* - Free slots index */
         int _nslots;             /* Size of slots array */
         int _nused;              /* num of used slots */
         int _nfree;              /* num of free slots */
         int _freemax;            /* maximum of free index */
         size_t _used;            /* amount of allocated mem */
         size_t _MaxMem;          /* upper bound set user or default value */
         size_t _MaxUsed;         /* Maximum used memory (peak value) */

      public:

         static Memory& Instance();

         size_t MaximumSize() const;
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
