#ifndef QDLIBWFBUFFER_H
#define QDLIBWFBUFFER_H

#include <queue>
#include <vector>

#include "qdlib/WaveFunction.h"
#include "tools/TmpFile.h"


#define WFBUFFER_LOCK_LAST 8
#define WFBUFFER_DEFAULT_MEM 3

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
         
         size_t _inmem;         /* Actual num elements in memory */
         size_t _maxmem;        /* Maximum number of elements allowed to keep in mem */
         size_t _ondisk;        /* Actual number of wfs on disk */
         size_t _MaxBuf;        /* Maximum mem size in bytes */
         
         dcomplex* _diskbuf;
         TmpFile<dcomplex> _File;
         
         queue<size_t> _LastAccess; /* Lately accessed positions */
         
         vector<BufMap>  _buf;
         vector<size_t> _diskmap;
      public:
         WFBuffer();
         ~WFBuffer();
          
         void Size(size_t size);
         size_t Size() const;
         
         void Init(WaveFunction* Psi);
         
         WaveFunction* GetBuf(size_t pos);
         void Release(size_t pos);
         
         void SetBuf(size_t pos, WaveFunction *Psi);
         //void Add(WaveFunction *Psi);
         
         void Clear();
   };

}

#endif
