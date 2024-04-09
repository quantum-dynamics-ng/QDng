// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

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


#define MEMORY_SLOTS 1024    /* Size of slot array */

namespace QDLIB
{

   Memory::Memory() : _used(0), _MaxUsed(0)
   {
       Reconfigure();
   }

   Memory::~Memory()
   {
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
       if ( CurrentSizeAvail() < size)
          throw(EMemory());

       // calc amounts of blocks needed
       size_t nb = size / QDMEM_BLOCK_SIZE;
       if (size % QDMEM_BLOCK_SIZE > 0) nb++;

       /* look for a free slot in the index */
       multimap<size_t, map<void*, SlotEntry>::iterator>::iterator fit = free_map.find(nb);
       //cout << nb << " "<< slot_map.size() << " " << free_map.size() << " " << CurrentSizeAvail() <<endl;

       if (fit == free_map.end()){
          // Create a new entry
          int ret = ::posix_memalign( p, QDLIB_DATA_ALIGNMENT, nb * QDMEM_BLOCK_SIZE);
          if (ret != 0) throw(EMemory());

          slot_map.insert(pair<void*, SlotEntry>(*p, SlotEntry(*p, nb)));

          _used += nb * QDMEM_BLOCK_SIZE;
       } else {
          // take a free entry
          *p = fit->second->second.p;
          fit->second->second.free = false;

          free_map.erase(fit);
       }

       /* book-keeping */
       if (_used > _MaxUsed)
          _MaxUsed = _used;


    }

    /**
     * Free a memory slot.
     */
    void Memory::Free(void *p)
    {
       map<void*, SlotEntry>::iterator sit = slot_map.find(p);

       if (sit == slot_map.end()) return;

       //cout << "free" << free_map.size() << endl;

#ifdef NDEBUG
       sit->second.free = true;
       free_map.insert(pair< size_t, map<void*, SlotEntry>::iterator >(sit->second.size, sit));
#else
       // If in debug mode, always allocate and free the slots
       // this makes it easier to trace errors with e.g. valgrind
       _used -= sit->second.size * QDMEM_BLOCK_SIZE;
       free(sit->second.p);
       slot_map.erase(sit);
#endif
    }

    /**
     * Deallocate unused slots in the OS.
     */
    void Memory::Cleanup()
    {
       map<void*, SlotEntry>::iterator sit = slot_map.begin();

       while (sit != slot_map.end()){
          if (sit->second.free){
             _used -= sit->second.size * QDMEM_BLOCK_SIZE;
             free(sit->second.p);
             slot_map.erase(sit);
             sit = slot_map.begin();
          } else
             sit++;
       }

       free_map.clear();
    }


}
