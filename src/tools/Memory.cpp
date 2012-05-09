/*
 * Memory.cpp
 *
 *  Created on: 22.10.2011
 *      Author: markus
 */

#include "Memory.h"
#include <sstream>
#include "tools/GlobalParams.h"

#include <sys/sysinfo.h>

#ifdef HAVE_AVX
#include <immintrin.h>
#endif

#ifdef HAVE_SSE2
#include <emmintrin.h>
#endif

#include <string.h>


#ifdef HAVE_AVX
 #define QDLIB_DATA_ALIGNMENT 32    /* 32 byte - Alignment for SIMD (AVX) */
#else
 #define QDLIB_DATA_ALIGNMENT 16    /* 16 byte - Alignment for SIMD (SSE2) */
#endif

#define MEMORY_SLOTS 1024    /* Size of slot array */

namespace QDLIB
{

   Memory::Memory() : _nslots(MEMORY_SLOTS), _nused(0), _nfree(0), _used(0), _MaxUsed(0)
   {
       Reconfigure();

      /* Initialize Slot DB */
      _Slots = new SlotEntry[MEMORY_SLOTS];
      _SlotsF = new SlotEntry*[MEMORY_SLOTS];
      for (int i=0; i < MEMORY_SLOTS; i++)
         _SlotsF[i] = NULL;

   }

   Memory::~Memory()
   {
      /* Free all memory still in use. */
      for (int i=0; i < _nslots; i++) {
         if (_Slots[i].p != NULL) free(_Slots[i].p);
      }
      delete[] _Slots;
      delete[] _SlotsF;
   }

   /**
    *  Elongate the slot buffer
    */
   void Memory::ResizeSlotBuffer()
   {
      SlotEntry *newbuf = new SlotEntry[_nslots+MEMORY_SLOTS];
      SlotEntry **newindex = new SlotEntry*[_nslots+MEMORY_SLOTS];

      for (int i=0; i < _nslots+MEMORY_SLOTS; i++)
        newindex[i] = NULL;


      /* Copy to new storage */
      int ind = 0;
      for (int i=0; i < _nslots; i++) {
         newbuf[i] = _Slots[i];
         if ( newbuf[i].free && newbuf[i].p != NULL ){
            newindex[ind] = &(newbuf[i]);
            ind++;
         }
      }

      _nslots += MEMORY_SLOTS;

      delete[] _Slots;
      delete[] _SlotsF;
      _Slots = newbuf;
      _SlotsF = newindex;
   }

   /**
    * Comparison function for slot sorting.
    *
    */
   bool Memory::Compare_SlotEntry(const SlotEntry &a, const SlotEntry &b)
   {
      if ( a.free && !b.free ) return true;
      else return false;
   }

    /**
     * Get an instance of the singleton.
     */
    Memory& Memory::Instance()
    {
       static Memory ref; /* Make it singleton */
       return ref;
    }

    /**
     * Get the maximum possible size/upper limit.
     */
    size_t Memory::MaximumSize() const
    {
       return _MaxMem;
    }

    /**
     * Re-read maximum size from global params.
     */
    void Memory::Reconfigure()
    {
       /* Set Limits */
            ParamContainer& params = GlobalParams::Instance();

            if ( params.isPresent("MaxMem") ){ /* Get limit from user defined entry */
               string s;
               params.GetValue("MaxMem", s);
               _MaxMem = ReadFromString(s);
            } else {                           /* Default: Half of system RAM */
               struct sysinfo info;
               sysinfo(&info);
               _MaxMem = info.totalram / 2;
            }

    }

    /**
     * Get the maximum size used since programm has started.
     *
     * This is a statistical value
     */
    size_t Memory::MaximumSizeUsed() const
    {
       return _MaxUsed;
    }

    /**
     * Current available size.
     */
    size_t Memory::CurrentSizeAvail() const
    {
       return _MaxMem - _used;
    }

    /**
     * Current amount of used memory.
     */
    size_t Memory::CurrentSizeUsed() const
    {
       return _used;
    }

    /**
     * Parse a formated string with a suffix convert to raw byte value.
     *
     * Valid suffixes are: K,M,G,T  (kilo, mega, giga, terra)
     * The binary definiton is used: 1K = 1024
     */
    size_t Memory::ReadFromString(const string value)
    {

       size_t factor;
       string s = value;

       if (s[s.length()-1] == 'B')
          s.erase(s.length()-1, 1); /* Remove suffix B */

       /* Search for unit suffix K, M or G and convert to factor */
       char suffix = s[s.length()-1];
       switch(suffix){
          case 'K':
             factor = 1024l;
             break;
          case 'M':
             factor = 1024l*1024l;
             break;
          case 'G':
             factor = 1024l*1024l*1024l;
             break;
          case 'T':
             factor = 1024l*1024l*1024l*1024l;
             break;
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
             factor = 1;
             break;
          default:
             throw( EParamProblem ("Wrong format for memory type number") );
       }

       if (factor > 1)
          s.erase(s.length()-1, 1); /* Remove suffix */


       /* Check validity */
       size_t found=s.find_first_not_of("0123456789");
       if (found!=string::npos)
          throw( EParamProblem ("Wrong format for memory type number") );

       /* Convert to number */
       size_t result;
       sscanf(s.c_str(), "%lu", (size_t*) &result);
       result *= factor;

       return result;
    }


    /**
     * Format a raw byte memory value to human readable format.
     *
     * The nearest suffix is chosen.
     */
    string Memory::Format(size_t value)
    {
       stringstream ss;

       if (value <= 1024)
          ss << value;
       else {
          ss << fixed;
          ss.precision(1);
          if (value > 1024l*1024l*1024l*1024l)
             ss << double(value)/double(1024l*1024l*1024l*1024l) << "TB";
          else if (value > 1024l*1024l*1024l)
             ss << double(value)/double(1024l*1024l*1024l) << "GB";
          else if (value > 1024l*1024l)
             ss << double(value)/double(1024l*1024l) << "MB";
          else if (value > 1024l)
             ss << double(value)/double(1024l) << "KB";
       }

       return ss.str();
    }


    /**
     * Memcopy on speed.
     *
     * If data is aligned to QDLIB_DATA_ALIGNMENT boundaries
     * then an optimized copy procedure can be applied.
     */
    void Memory::Copy(char *dst, char* src, size_t size)
    {
       memcpy(dst, src, size);
    }

    /**
     * Allocate Aligned Memory.
     */
    void Memory::Align(void **p, size_t size)
    {
       /* Check if we have enough free space */
       if ( CurrentSizeAvail() < size )
          throw(EMemory());

       if (_nused == _nslots)
          ResizeSlotBuffer();

       SlotEntry *entry = NULL;

       /* look for a free slot in the index */
       for(int i=0; i < _nslots; i++) {
          if (_SlotsF[i] != NULL)
             if ( _SlotsF[i]->size == size ){
                entry = _SlotsF[i];
                _SlotsF[i] = NULL;      /* remove from "free index" */
                _nfree--;
                break;
             }
       }

       /* create a new slot */
       if (entry == NULL){
          /* search usable entry for allocation */
          for(int i=0; i < _nslots; i++) {
             if (_Slots[i].free && _Slots[i].p == NULL){
                entry = &_Slots[i];
                _nused++;
                break;
             }
          }

          int ret = posix_memalign( & (entry->p), QDLIB_DATA_ALIGNMENT, size);

          if (ret != 0)
             throw(EMemory());

          entry->size = size;
       }

       /* book-keeping */
       *p = entry->p;
       entry->free = false;
       _used += size;
       if (_used > _MaxUsed)
          _MaxUsed = _used;


    }

    /**
     * Free a memory slot.
     */
    void Memory::Free(void *p)
    {
       for (int i=_nused-1; i >= 0; i--){
          if (_Slots[i].p == p){
             _Slots[i].free = true;
             _used -= _Slots[i].size;
             _nfree++;
             /* push free slot on index */
             for (int j=0; j < _nslots; j++){
                if (_SlotsF[j] == NULL){
                   _SlotsF[j] = &(_Slots[i]);
                   break;
                }
             }
             break;
          }
       }
    }

    /**
     * Deallocate unused slots in the OS.
     */
    void Memory::Cleanup()
    {
       for (int i=0; i < _nslots; i++){
          if ( _Slots[i].free && _Slots[i].size > 0){
             free(_Slots[i].p);
             _used -= _Slots[i].size;
             _Slots[i].size = 0;
             _Slots[i].p = NULL;
             _Slots[i].free = true;
             _nused--;
          }

          _SlotsF[i] = NULL;
       }
    }


}
