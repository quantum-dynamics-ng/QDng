#ifndef QDLIBWFBUFFER_H
#define QDLIBWFBUFFER_H

#include <map>
#include <vector>
#include <deque>

#include <limits.h>

#include "qdlib/WaveFunction.h"
#include "tools/TmpFile.h"

#define DEFAULT_BUFFER_SIZE 1 * 1024*1024*1024  /* Default Max buffer size */

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
      friend class WFBufferTest;
      private:
         struct BufMap {
            WaveFunction* Psi;
            bool Locked;
            int BufPos;         /* Position in  disk buffer */
            BufMap() : Psi(NULL), Locked(false), BufPos(-1){}
         };
         
         size_t _size;
         size_t _wfsize;
         size_t _LastLocks;      /* How much of the last access to keep valid */
         
         size_t _inmem;         /* Actual num elements in memory */
         size_t _locked;        /* Actual number of explicit locks */
         size_t _maxmem;        /* Maximum number of elements allowed to keep in mem */
         size_t _ondisk;        /* Actual number of wfs on disk */
         size_t _MaxBuf;        /* Maximum mem size in bytes */
         
         dcomplex* _diskbuf;
         TmpFile<dcomplex> _File;
         
         deque<size_t> _LastAccess; /* Lately accessed positions */
         
         vector<BufMap>  _buf;
         vector<size_t> _diskmap;   /* key = diskpos, value = bufferpos */
         
         size_t _FreeDiskPos();
         WaveFunction* _ValidEntry();
         bool _IsLocked(size_t mempos);
         void _MoveToDisk();
         void _MoveToMem(size_t pos);
      public:
         WFBuffer();
         ~WFBuffer();
          
         void Size(size_t size);
         size_t Size() const {return _size;}
         
         void Init(WaveFunction* Psi);
         
         WaveFunction* Get(const size_t pos);
         void Set(const size_t pos, WaveFunction *Psi);
         void Add(WaveFunction *Psi);
         
         void SaveToFiles(const string&  name);
         void ReadFromFiles(const string&  name, int begin=0, int step=1, int end=0);
         
         /**
          * This is a synoyme for Get.
          */
         WaveFunction* operator[](size_t pos) { return Get(pos) ; }
         
         void Lock(size_t pos);
         void UnLock(size_t pos);
         
         void AutoLock(size_t LastLocks);
         size_t AutoLock() {return _LastLocks; }
         
         void Clear();
   };

}

#endif
