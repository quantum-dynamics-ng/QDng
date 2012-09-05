#include "WFBuffer.h"
#include "tools/GlobalParams.h"
#include "qdlib/FileWF.h"

#include <unistd.h>
#include <sys/mman.h>


namespace QDLIB {

   WFBuffer::WFBuffer() : _size(0), _wfsize(0), _LastLocks(WFBUFFER_LOCK_LAST),
                      _inmem(0), _locked(0), _maxmem(SIZE_MAX), _ondisk(0), _MaxBuf(DEFAULT_BUFFER_SIZE), _diskbuf(NULL)
   {
      ParamContainer& gp = GlobalParams::Instance();
            
      if (gp.isPresent("MaxBufferSize")){
         string s;
         
         gp.GetValue("MaxBufferSize", s);
         _MaxBuf = Memory::ReadFromString(s);

         if (_MaxBuf == 0)
            throw(EParamProblem("Invalid buffer size given"));
      } else {
         /* Use two third for free space for buffering */
         _MaxBuf = Memory::Instance().CurrentSizeAvail() * 2 / 3;
      }
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
      for (size_t i=0; i < _diskmap.size(); i++){
         if (_diskmap[i] == (size_t) -1) return i;
      }
      
      _diskmap.push_back(-1);
      if (_File.isOpen())
         _diskbuf = _File.Resize(_diskmap.size()*_wfsize);
      else
         _diskbuf = _File.Open(_diskmap.size()*_wfsize,false);
      
      return _diskmap.size()-1;
   }

   /**
    * Search for a valid buffer entry.
    */
   WaveFunction* WFBuffer::_ValidEntry()
   {
      for (size_t i=0; i < _buf.size(); i++)
         if (_buf[i].Psi != NULL) return _buf[i].Psi;
      
      return NULL;
   }   
   
   bool WFBuffer::_IsLocked(size_t mempos)
   {
      for (size_t i=0; i < _LastAccess.size(); i++){
         if ( _LastAccess[i] == mempos || _buf[i].Locked ) return true;
      }
      return false;
   }
   
   /**
    * Copy a membuf element into the disk buffer.
    * \param lpos Don't move this buffer.
    */
   void WFBuffer::_MoveToDisk()
   {
      for (size_t i = _buf.size()-1; i > 0; i--){ /* walk through membuf - backwards */
         if (_buf[i].BufPos == -1 && _buf[i].Psi != NULL && !_IsLocked(i)){ /* This is an Element in memory */
            size_t dpos = _FreeDiskPos();
            _diskmap[dpos] = i;
            
            /* Be sure to move all strides */
            for (int s=0; s < _buf[i].Psi->strides(); s++){
/*               memcpy(reinterpret_cast<void*>( & (_diskbuf[_wfsize*dpos + s * _buf[i].Psi->lsize()])),
                      reinterpret_cast<void*>(_buf[i].Psi->begin(s)), _buf[i].Psi->sizeBytes());*/
               _File.Seek(_wfsize*dpos + s * _buf[i].Psi->lsize());
               _File.Write(_buf[i].Psi->begin(s), _buf[i].Psi->lsize());
            }
                   
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
      /* Initialize buffer position */
      if (_buf[pos].Psi == NULL)
         _buf[pos].Psi = _ValidEntry()->NewInstance();
      
      for (int s=0; s < _buf[pos].Psi->strides(); s++){
/*         memcpy(reinterpret_cast<void*>(_buf[pos].Psi->begin(s)),
                reinterpret_cast<void*>(&(_diskbuf[_wfsize*_buf[pos].BufPos + s * _buf[pos].Psi->lsize()])), _buf[pos].Psi->sizeBytes());*/
         _File.Seek(_wfsize*_buf[pos].BufPos + s * _buf[pos].Psi->lsize());
         _File.Read(_buf[pos].Psi->begin(s), _buf[pos].Psi->lsize());
      }
      
      /* Invalidate disk buffer element */ 
      _diskmap[_buf[pos].BufPos] = -1;
      _buf[pos].BufPos = -1;
      
      _ondisk--;
      _inmem++;
   }

   /**
    * Set the maximum number buffer slots needed.
    */
   void WFBuffer::ResizeBuffer(size_t size)
   {
      size_t oldsize = _buf.size();

      if (oldsize > size) { /* Reduce size */
         for (size_t i=oldsize-1; i >= size; i--){
            if (_buf[i].Psi != NULL){ /* Remove wf from memory */
               DELETE_WF(_buf[i].Psi);
               _inmem--;
            }
            if (_buf[i].BufPos > -1){ /* Remove wf from disk buf */
               _diskmap[_buf[i].BufPos] = -1;
               _ondisk--;
            }
            if (_buf[i].Psi != NULL && _buf[i].BufPos > -1) _size--;
            _buf.pop_back();

         }
         /* Clear all locks */
         _LastAccess.clear();
         _locked = 0;
      } else if (oldsize < size) { /* Enlarge */
         _buf.resize(size);
      }
   }

   /**
    * Init with a WF.
    * This has to be done once.
    */
   void WFBuffer::Init(WaveFunction* Psi)
   {
      if (_MaxBuf > 0){
         _maxmem = _MaxBuf / Psi->sizeBytes();
      
         _wfsize = Psi->size();
         
         if (_maxmem < 1)
            throw (EParamProblem("Not enough memory for buffer available"));
            
         if (_maxmem < _size){
            _diskbuf = _File.Open((_size-_maxmem)*_wfsize,false);
            _diskmap.assign(_size-_maxmem, -1);
         }
         
      }

      /* Have at least one valid entry */
      if (_buf.size() == 0){
         _buf.resize(1);
         _inmem = 1;
      }

      _buf[0].Psi = Psi->NewInstance();
   }
   
   /**
    * Set an element.
    */
   void WFBuffer::Set(const size_t pos, WaveFunction *Psi)
   {
      /* Check for right map size */
      if (pos >= _buf.size()){
         _buf.resize(pos+1);
      }

      if (pos >= _size)
         _size = pos + 1;

      if (_inmem >= _maxmem){/* Move an element to disk storage */
         bool lock = _buf[pos].Locked;
         _buf[pos].Locked = true;
         _MoveToDisk();
         _buf[pos].Locked = lock;
      }
            
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
   
   void WFBuffer::Add(WaveFunction *Psi)
   {
      Set(_size, Psi);
   }
   
   WaveFunction* WFBuffer::Get(const size_t pos)
   {
      if (pos >= _buf.size())
         throw (EIncompatible("Tried to read invalid WFBuffer element"));
      
      if (_buf[pos].Psi == NULL){
         if (_inmem >= _maxmem) {/* Move an element to disk storage */
            _buf[pos].Locked = true;
            _MoveToDisk();
            _buf[pos].Locked = false;
         }

         if (_buf[pos].BufPos == -1){ /* This might happen if we request a postion which haven't been written before */
            _buf[pos].Psi = _ValidEntry()->NewInstance();
         } else
            _MoveToMem(pos);
      }
      
      _LastAccess.push_front(pos);
      if (_LastAccess.size() > _LastLocks) _LastAccess.pop_back();

      return _buf[pos].Psi;
   }
  
   /**
    * Saves the temporary buffer to a series of files.
    */
   void WFBuffer::SaveToFiles(const string&  name)
   {
      FileWF wfile;
      
      /* Init file writer for wf output */
      wfile.Name(name);
      wfile.Suffix(BINARY_WF_SUFFIX);
      wfile.ActivateSequence();
      wfile.ResetCounter();
      
      for (size_t i=0; i < _size; i++) /* Write down files */
         wfile << Get(i);
            
   }
   
   /**
    * Fill the buffer with a wf series from disk.
    */
   void WFBuffer::ReadFromFiles(const string&  name, int begin, int step, int num)
   {
      FileWF wfile;
      WaveFunction* psi;
      
      /* Init file writer for wf output */
      wfile.Suffix(BINARY_WF_SUFFIX);
      wfile.Name(name);
      wfile.ActivateSequence(step);
      wfile.Counter(begin);

      wfile >> &psi; /* Load and init first wf by meta data */
      Add(psi);

      int counter = 1;

      try {
         while (counter < num || num == -1){ /* Run loop until something fails => must be the end of the wf-series (this is dirty) */
            wfile >> psi;
            Add(psi);
            counter++;
         }
      } catch (EIOError& e) {}
   }
  
  
   /**
    * Lock a buffer element explictly to mem.
    * 
    */
   void WFBuffer::Lock(size_t pos)
   {
      if (_LastLocks + _locked + 1 > _maxmem )
         throw (EParamProblem("Can't lock another wave function to memory. Increase MaxBufferSize"));

      if (!_buf[pos].Locked) _locked++;
      _buf[pos].Locked = true;

   }

   /**
    * Unlock a buffer position.
    */
   void WFBuffer::UnLock(size_t pos)
   {
      if (_buf[pos].Locked) _locked--;
      _buf[pos].Locked = false;
   }
   
   /**
    * A buffer element is automatically locked to memory when accessed.
    * This determines how many of the last accessed are locked into memory.
    * 
    */
   void WFBuffer::AutoLock(size_t LastLocks)
   {
      if ( LastLocks > _maxmem)
         throw (EParamProblem("Can't lock enough wave functions to memory. Increase MaxBufferSize"));
         
      _LastLocks = LastLocks;
   }
   
   /**
    * Clear all Content of the buffer and remove the tmp file.
    */
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
      
      _size = 0;
      _inmem = 0;
      _maxmem = 0;
      _ondisk = 0;
      _diskbuf = NULL;
   }


}
