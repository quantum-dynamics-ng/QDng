#include "WFBuffer.h"
#include "tools/GlobalParams.h"
#include "qdlib/FileWF.h"

#include <unistd.h>
#include <sys/mman.h>


namespace QDLIB {

   WFBuffer::WFBuffer() : _LastLocks(WFBUFFER_LOCK_LAST),
                       _locked(0), _ondisk(0), _MaxBuf(DEFAULT_BUFFER_SIZE)
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

      string tmpdir;

      /* Look for the tmp path */
      if (gp.isPresent("tmpdir")){
         gp.GetValue("tmpdir", tmpdir);
      } else {
         char *c;
         c = getenv("TMPDIR");
         if (c != NULL)
            tmpdir = c;
      }

      /* Create a unique name and open the file */
      if ( tmpdir.length() > 0 )
         _fname = tmpdir + "/" + DEFAULT_BUFFER_NAME;
      else
         _fname = DEFAULT_BUFFER_NAME;

      /* Get some random to avoid name clashes */
      int fd = open("/dev/random",0);
      if (fd == -1) throw (EIOError("WFBuffer: open /dev/random failed"));
      uint32 num;
      size_t got = read(fd, &num, sizeof(num));

      if (got < sizeof(num)) throw (EIOError("WFBuffer: readin /dev/random failed"));
      close(fd);

      char hex[9];
      sprintf(hex, "%X", num);

      _fname += hex;
   }


   WFBuffer::~WFBuffer()
   {
      Clear();
   }   
  
   string WFBuffer::_GetFileName(size_t pos)
   {
      char hex[9];
      snprintf(hex, 9, "%X", (uint32_t) pos); // This limits us to a disk buffer size of 2^32 elements

      return _fname + string(hex);
   }

   size_t WFBuffer::_BytesInMem()
   {
      size_t inmem = 0;
      for (size_t i=0; i < _buf.size(); i++){
         if (_buf[i].Psi != NULL) inmem += _buf[i].Psi->size_bytes();
      }

      return inmem;
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
         if ( _LastAccess[i] == mempos) return true;
      }

      for (size_t i=0; i < _buf.size(); i++){
         if (_buf[i].Locked) return true;
      }

      return false;
   }
   
   /**
    * Copy a membuf element into the disk buffer.
    */
   void WFBuffer::_MoveToDisk()
   {
      for (size_t i = _buf.size()-1; i > 0; i--){ /* walk through membuf - backwards */
         if (_buf[i].Psi != NULL && !_buf[i].Locked){ /* This is an Element in memory */

            // Dump to file
            ofstream file;
            file.open(_GetFileName(i).c_str(), ios_base::trunc);
            _buf[i].Psi->Serialize(file); // TODO: Error checking!!
            file.close();

            // Book keeping
            _ondisk++;

            _buf[i].ondisk = true;
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

      ifstream file;
      file.open(_GetFileName(pos).c_str());
      _buf[pos].Psi->DeSerialize(file); // TODO: Error checking!!
      file.close();
      
      _ondisk--;
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
            }

            if (_buf[i].ondisk) { /* Remove wf from disk buf */
               FS::Remove(_GetFileName(i));
               _ondisk--;
            }

            _buf.pop_back(); /* remove last element */
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
         if (_MaxBuf < Psi->size_bytes())
            throw (EParamProblem("Not enough memory for WFbuffer available"));
      }

      /* Have at least one valid entry */
      if (_buf.size() == 0){
         _buf.resize(1);
      }

      _buf[0].Psi = Psi->NewInstance();
   }
   
   /**
    * Set an element.
    *
    * The content is copied to the buffer.
    */
   void WFBuffer::Set(size_t pos, WaveFunction *Psi)
   {
      /* Check for right map size */
      if (pos >= _buf.size()){
         _buf.resize(pos+1);
      }

      /* Ensure we have some space */
      if (_BytesInMem() >= _MaxBuf){/* Move an element to disk storage */
         _MoveToDisk();
      }
            
      if (_buf[pos].Psi == NULL) { /* Create in mem element*/
         _buf[pos].Psi = Psi->NewInstance();
      }
      
      *(_buf[pos].Psi) = Psi;

      /* Make sure we're not to large */
      if (_BytesInMem() >= _MaxBuf){/* Move an element to disk storage */
         _MoveToDisk();
      }

      if (_BytesInMem() > _MaxBuf){
         throw(EMemError("WFBuffer: buffer space execeeded. Increase MaxBufferSize."));
      }
   }
   
   void WFBuffer::Add(WaveFunction *Psi)
   {
      Set(_buf.size(), Psi);
   }
   
   WaveFunction* WFBuffer::Get(size_t pos)
   {
      if (pos >= _buf.size())
         throw (EIncompatible("Tried to read invalid WFBuffer element"));

      _buf[pos].Locked = true;
      
      if (_LastLocks > 0 ) _LastAccess.push_front(pos);
      if (_LastAccess.size() > _LastLocks) {
         _buf[_LastAccess.back()].Locked = false;
         _LastAccess.pop_back();
      }

      if (_buf[pos].Psi == NULL){
         if (_BytesInMem() >= _MaxBuf) {/* Move an element to disk storage */
            _MoveToDisk();
         }

         if (!_buf[pos].ondisk){ /* This might happen if we request a postion which haven't been written before */
            _buf[pos].Psi = _ValidEntry()->NewInstance();
         } else
            _MoveToMem(pos);

         if (_BytesInMem() >= _MaxBuf) {/* Move an element to disk storage */
            _MoveToDisk();
         }

         if (_BytesInMem() > _MaxBuf){
            throw(EMemError("WFBuffer: buffer space execeeded. Increase MaxBufferSize."));
         }
      }

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
      
      for (size_t i=0; i < _buf.size(); i++) /* Write down files */
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
      /* try to reduce the size */
      if (_BytesInMem() >= _MaxBuf){
         bool locked = _buf[pos].Locked;
         _buf[pos].Locked = true;
         _MoveToDisk();
         _buf[pos].Locked = locked;
      }

      if (_BytesInMem() > _MaxBuf)
         throw (EMemError("Can't lock another wave function to memory. Increase MaxBufferSize"));

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
      _LastLocks = LastLocks;
   }
   
   /**
    * Clear all Content of the buffer and remove the tmp files.
    */
   void WFBuffer::Clear()
   {

      while (! _buf.empty() ){
         DELETE_WF(_buf.back().Psi);
         if (_buf.back().ondisk) FS::Remove(_GetFileName(_buf.size()-1));
         _buf.pop_back();
      }
      
      _LastAccess.clear();
      _ondisk = 0;
      _locked = 0;
   }


}
