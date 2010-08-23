#include "WFBuffer.h"
#include "tools/GlobalParams.h"

#include <unistd.h>
#include <sys/mman.h>




namespace QDLIB {

   WFBuffer::WFBuffer() : _size(0), _wfsize(0), _LastLocks(WFBUFFER_LOCK_LAST),
                          _inmem(0), _maxmem(SIZE_MAX), _MaxBuf(0) ,_ondisk(0), _diskbuf(NULL)
   {
      ParamContainer& gp = GlobalParams::Instance();
            
      if (gp.isPresent("MaxBufferSize"))
         gp.GetValue("MaxBufferSize", (long int&) _MaxBuf);
   }


   WFBuffer::~WFBuffer()
   {
      Clear();
   }   
  
   /**
    * Returns a free slot of the diskbuffer.
    * Resizes the diskbuffer if needed.
    */
   size_t WFBuffer::_FreeDiskPos()
   {
      for (int i=0; i < _diskmap.size(); i++){
         if (_diskmap[i] == -1) return i;
      }
      
      _diskmap.push_back(-1);
      if (_File.isOpen())
         _diskbuf = _File.Resize(_diskmap.size()*_wfsize);
      else
         _diskbuf = _File.Open(_diskmap.size()*_wfsize,true);
      
      return _diskmap.size()-1;
   }
   
   bool WFBuffer::_IsLocked(size_t mempos)
   {
      for (int i=0; i < _LastAccess.size(); i++){
         if ( _LastAccess[i] == mempos || _Locked == mempos ) return true;
      }
      return false;
   }
   
   /**
    * Copy a membuf element into the disk buffer.
    * \param lpos Don't move this buffer.
    */
   void WFBuffer::_MoveToDisk(size_t lpos)
   {
      for (size_t i = _buf.size()-1; i > 0; i--){ /* walk through membuf - backwards */
         if (i != lpos && _buf[i].BufPos == -1 && _buf[i].Psi != NULL && !_IsLocked(i)){ /* This is an Element in memory */
            size_t dpos = _FreeDiskPos();
            _diskmap[dpos] = i;
            memcpy(reinterpret_cast<void*>( & (_diskbuf[_wfsize*dpos])), 
                   reinterpret_cast<void*>(_buf[i].Psi->begin(0)), _buf[i].Psi->sizeBytes());
            _ondisk++;
            _inmem--;
            _buf[i].BufPos = dpos;
            DELETE_WF(_buf[i].Psi);
            _buf[i].Psi = NULL;
            return;
         }
      }
   }
   
   /**
    * Move an element from diskspace to mem.
    * \param pos the pos in memory.
    */
   void WFBuffer::_MoveToMem(size_t pos)
   {
      memcpy(reinterpret_cast<void*>(_buf[pos].Psi->begin(0)),
             reinterpret_cast<void*>(&(_diskbuf[_wfsize*_buf[pos].BufPos])), _buf[pos].Psi->sizeBytes());
      /* Invalidate disk buffer element */ 
      _diskmap[_buf[pos].BufPos] = -1;
      _buf[pos].BufPos = -1;
      _ondisk--;
      _inmem++;
   }
   
   /**
    * Size of the buffer.
    */
   void WFBuffer::Size(size_t size)
   {
      if (_size == 0) _size = size;
      else if (size < 0) Clear();
   }

   /**
    * Init with a WF.
    */
   void WFBuffer::Init(WaveFunction* Psi)
   {
      if (_MaxBuf > 0){
         _maxmem = _MaxBuf / Psi->sizeBytes();
         
         if (_maxmem < 1) _maxmem = 0;
            throw (EParamProblem("Not enough memory for buffer available"));
            
         if (_maxmem < _size)
            _diskbuf = _File.Open(_diskmap.size()*_wfsize,true);
         
      }
      _wfsize = Psi->size();
   }
   
   /**
    * Set an element.
    */
   void WFBuffer::Set(size_t pos, WaveFunction *Psi)
   {           
      if (_inmem >= _maxmem)/* Move an element to disk storage */
         _MoveToDisk(pos);
        
      
      if (_buf[pos].Psi == NULL) {
         _buf[pos].Psi = Psi->NewInstance();
         _inmem++;
         if (_buf[pos].BufPos != -1){  /* Invalidate disk buf */
            _diskmap[_buf[pos].BufPos] = -1;
            _buf[pos].BufPos = -1;
            _ondisk--;
         }
      }
      
      *(_buf[pos].Psi) = Psi;
   }
   
   WaveFunction* WFBuffer::Get(size_t pos)
   {
      if (pos >= _buf.size())
         throw (EIncompatible("Invalid WFBuffer Element"));
      
      if (_buf[pos].Psi == NULL){
         if (_inmem >= _maxmem)/* Move an element to disk storage */
            _MoveToDisk(pos);

         _MoveToMem(pos);
      }
      
      _LastAccess.push_front(pos);
      if (_LastAccess.size() > _LastLocks) _LastAccess.pop_back();

      return _buf[pos].Psi;
   }
  
   /**
    * Lock a buffer element explictly to mem.
    * Note that only one postion can be excplitly locked.
    */
   void WFBuffer::Lock(size_t pos)
   {
      _Locked = pos;
   }

   /**
    * Unlock a buffer position.
    */
   void WFBuffer::UnLock(size_t pos)
   {
      _Locked = pos;
   }
   
   void WFBuffer::Clear()
   {
      _File.Close();
      
      while (! _buf.empty() ){
         DELETE_WF(_buf.back().Psi);
         _buf.pop_back();
      }
      
      while (! _LastAccess.empty())
         _LastAccess.pop_back();
      
      _diskmap.clear();
      
      _Locked = -1;
      _size = 0;
      _inmem = 0;
      _maxmem = 0;
      _ondisk = 0;
      _diskbuf = NULL;
   }


}
