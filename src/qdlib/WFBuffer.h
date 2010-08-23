#ifndef QDLIBWFBUFFER_H
#define QDLIBWFBUFFER_H

#include <map>
#include <vector>
#include <deque>

#include <limits.h>

#include "qdlib/WaveFunction.h"
#include "tools/TmpFile.h"


#define WFBUFFER_LOCK_LAST 2

#ifndef SIZE_MAX
#define SIZE_MAX LONG_MAX
#endif

namespace QDLIB {

   /**
    * WaveFunction Buffer 
    *   @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class WFBuffer : private cVec {
      private:
         struct BufMap {
            WaveFunction* Psi;
            int BufPos;         /* Position in  disk buffer */
            BufMap() : Psi(NULL), BufPos(-1){}
         };
         
         size_t _size;
         size_t _wfsize;
         size_t _LastLocks;      /* How much of the last access to keep valid */
         size_t _Locked;        /* */
         
         size_t _inmem;         /* Actual num elements in memory */
         size_t _maxmem;        /* Maximum number of elements allowed to keep in mem */
         size_t _ondisk;        /* Actual number of wfs on disk */
         size_t _MaxBuf;        /* Maximum mem size in bytes */
         
         dcomplex* _diskbuf;
         TmpFile<dcomplex> _File;
         
         deque<size_t> _LastAccess; /* Lately accessed positions */
         
         vector<BufMap>  _buf;
         vector<size_t> _diskmap;   /* key = diskpos, value = bufferpos */
         
         size_t _FreeDiskPos();
         bool _IsLocked(size_t mempos);
         void _MoveToDisk(size_t lpos);
         void _MoveToMem(size_t pos);
      public:
         WFBuffer();
         ~WFBuffer();
          
         void Size(size_t size);
         size_t Size() const;
         
         void Init(WaveFunction* Psi);
         
         WaveFunction* Get(size_t pos);
         void Set(size_t pos, WaveFunction *Psi);
         
         void Lock(size_t pos);
         void UnLock(size_t pos);
         //void Add(WaveFunction *Psi);
         
         void Clear();
   };

}

#endif
