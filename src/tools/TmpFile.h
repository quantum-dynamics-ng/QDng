#ifndef QDLIBTMPFILE_H
#define QDLIBMPFILE_H

#include "tools/GlobalParams.h"
#include "tools/fstools.h"

#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define QDNG_TMPFILE_BASE "QDng_tmp_XXXXXX"  /* File name template. Must be exactly 6 X's a the end ! */

namespace QDLIB {

   /**
    * Temporary file.
    * This class works like an array which is transparantly buffer on disk.
    * If used with memory maped IO the temporary file is projected into memory.
    *
    * In none mmaped mode, simple read, write, seek methods are provided for IO.
    */
   template<typename T>
   class TmpFile {
      private:
         string _fname;
         int _fd;          /* File descriptor */
         size_t _size;
         size_t _fpos;
         bool _open;
         bool _mmap;
         void *_mapbase;
         
         void _Upsize(size_t size);
      public:
         TmpFile() : _fd(-1), _size(0), _fpos(0), _open(false), _mmap(false), _mapbase(NULL) {}
         
         ~TmpFile();
         
         T* Open(size_t size = 0, bool memmap = false);
         T* Resize(size_t size);
         
         void Close(bool remove = true);
         void Flush();
         
         void Seek(size_t pos);
         size_t Read(T* buf, size_t count);
         void Write(T* buf, size_t count);
         
         
         bool isOpen() { return _open; }
         size_t Size() { return _size; }
   };

   /**
    * If the file is still open at destruction, it will be deleted.
    */
   template<typename T>
   TmpFile<T>::~TmpFile()
   {
       Close();
   }

   template<typename T>
   void TmpFile<T>::_Upsize(size_t size)
   {  
      int res = lseek(_fd, size*sizeof(T)-1, SEEK_SET);
      if (res == -1){
         close(_fd);
         _open = false;
         throw(EIOError("Seek error"));
      }
      
      res = write(_fd, "", 1); /* Make a dummy write force the OS to enlarge the file */
      if (res == -1){
         close(_fd);
         _open = false;
         FS::Remove(_fname);
         throw(EIOError(errno, _fname));
      }         
            
      _size = size;
   }
   
   /**
    * Open a new temporary file.
    * 
    * If there is already a file opened the old one is closes and removed.
    *
    * \param size    Create a file with a defined size in units of elements (Specially needed for mmap)
    * \param memmap  Map the file into memory. 
    * \return        The pointer to the mmap start address, otherwise NULL
    */
   template<typename T>
   T* TmpFile<T>::Open(size_t size, bool memmap)
   {
      string tmpdir;
      ParamContainer& gp = GlobalParams::Instance();
      
      if (_open) Close();
     
      _size = size;
      _mmap = memmap;
      
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
         _fname = tmpdir + "/" + QDNG_TMPFILE_BASE;
      else
         _fname = QDNG_TMPFILE_BASE;
      
      char *name = (char*) malloc(_fname.size()+1);
      strcpy(name, _fname.c_str());
      _fd = mkstemp(name); /* Open the file and replace X's in the file name template */
      _fname = name;
      free(name);
      
      if (_fd == -1)
         throw (EIOError(errno, _fname));
   
      _open = true;
      
      /* Re-size the file */
      if (size > 0) _Upsize(size);
    
      /* Create memory mapping */
      if (_mmap) {
         _mapbase = mmap(NULL, size*sizeof(T), PROT_WRITE|PROT_READ, MAP_SHARED, _fd, 0);
         
         if (_mapbase == NULL){
            close(_fd);
            _open = false;
	    FS::Remove(_fname);
            throw(EIOError(errno, _fname));
         }
         _mmap = true;
         return (T*) _mapbase;
      }
      
      return NULL;
   }
   
   /**
    * Resize the file and the mmaped range if used.
    * \return a new pointer to mmaped range. The old one might be invalid
    */
   template<typename T>
   T* TmpFile<T>::Resize(size_t size)
   {
     size_t oldsize = _size;
      
      if (_size == size || !_open) return (T*) _mapbase;

      /* Resize */
      int res = ftruncate(_fd, size*sizeof(T));
      if (res != 0){
         close(_fd);
         _open = false;
	 FS::Remove(_fname);
         throw (EIOError(errno, _fname));
      }

       
      /* Remap memory */
      if (_mmap) {
         msync(_mapbase, oldsize*sizeof(T), MS_SYNC);
       _mapbase = mremap(_mapbase, oldsize*sizeof(T), size*sizeof(T), MREMAP_MAYMOVE);
        
        if (_mapbase == MAP_FAILED){
           close(_fd);
           _open = false;
	   FS::Remove(_fname);
           throw(EIOError(errno, _fname));
        }
        
        _mmap = true;
        return (T*) _mapbase;
      }
      
      return NULL;
   }
   
   /**
    * Close and remove the tmp file when requested.
    */
   template<typename T>
   void TmpFile<T>::Close(bool remove)
   {
      if (!_open) return;
      
      /* clear mmap */
      if (_mmap)
      {         
         munmap(_mapbase, _size * sizeof (T));
         _mapbase = NULL;
         _mmap = false;
      }
      
      /* close file*/
      if (_fd != -1){
         close(_fd);
         _fd = -1;
      }
      
      _size = 0;
      _open = false;
      
      if (remove) FS::Remove(_fname);
      
      _fname.clear();
   }
   
   /**
    * Flush buffer to disk.
    * 
    * For mmaped files the buffer is synced.
    */
   template<typename T>
   void TmpFile<T>::Flush()
   {
      if (_mmap) msync(_mapbase, _size*sizeof(T), MS_SYNC);
   }
   
   /**
    * Seek to position in file.
    */
   template<typename T>
   void TmpFile<T>::Seek(size_t pos)
   {
      int res = lseek(_fd, pos*sizeof(T), SEEK_SET);
      if (res == -1){
         close(_fd);
         _open = false;
         throw(EIOError("Seek error"));
      }
   }
   
   /**
    * Read from actual file position.
    */
   template<typename T>
   size_t TmpFile<T>::Read(T* buf, size_t count)
   {
      ssize_t res = read(_fd, (T*) buf, sizeof(T) * count);
      if (res == -1 || (size_t) res != count * sizeof(T) ){
         close(_fd);
         _open = false;
         throw(EIOError("Read error"));
      }
      return (size_t) res;
   }
   
   /**
    * Write to actual file position.
    */
   template<typename T>
   void TmpFile<T>::Write(T* buf, size_t count)
   {
      ssize_t res = write(_fd, (T*) buf, sizeof(T) * count);
      if (res == -1 || (size_t) res != count * sizeof(T) ){
         close(_fd);
         _open = false;
         throw(EIOError("Write error"));
      }
   }
}


#endif