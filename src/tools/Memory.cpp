/*
 * Memory.cpp
 *
 *  Created on: 22.10.2011
 *      Author: markus
 */

#include "Memory.h"
#include <sstream>
#include "tools/GlobalParams.h"

/** \todo autoconfig */
#include <sys/sysinfo.h>

#ifdef HAVE_AVX
 #define QDLIB_DATA_ALIGNMENT 32    /* 32 byte - Alignment for SIMD (AVX) */
#else
 #define QDLIB_DATA_ALIGNMENT 16    /* 16 byte - Alignment for SIMD (SSE2) */
#endif

namespace QDLIB
{

   Memory::Memory() : _used(0), _MaxUsed(0)
   {
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

   Memory::~Memory()
   {
      // TODO Auto-generated destructor stub
   }

    /* */
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
     * Allocate Aligned Memory.
     */
    void Memory::Align(void **p, size_t size)
    {
       /* Check if we have enough free space */
       if ( CurrentSizeAvail() < size )
          throw(EMemory());

       list<SlotEntry>::iterator it;

       for( it = Slots.begin(); it != Slots.end(); it++) { /* find a free slot*/
          if ( it->free && it->size == size)
             break;
       }

       SlotEntry* slot;
       if (it == Slots.end()){ /* create a new slot */
          Slots.push_back(SlotEntry(size));
          slot = &(Slots.back());

          int ret = posix_memalign(& (slot->p), QDLIB_DATA_ALIGNMENT, size);

          if (ret != 0)
             throw(EMemory());
       } else {
          slot = &(*it);
       }

       /* book-keeping */
       slot->free = false;
       _used += size;
       if (_used > _MaxUsed)
          _MaxUsed = _used;

       *p = slot->p;
    }

    /**
     * Free a memory slot.
     */
    void Memory::Free(void *p)
    {
       list<SlotEntry>::iterator it;

       for( it = Slots.begin(); it != Slots.end(); it++) { /* find the corresponding slot */
          if ( it->p == p) {
             it->free = true;
             _used -= it->size;
          }
       }

    }

    /**
     * Deallocate unused slots in the OS.
     */
    void Memory::Cleanup()
    {
       list<SlotEntry>::iterator it;

       for( it = Slots.begin(); it != Slots.end(); it++) { /* find  free slots */
          if ( it->free ){
             free(it->p);
             _used -= it->size;
             Slots.erase(it);
          }
       }
    }


}
