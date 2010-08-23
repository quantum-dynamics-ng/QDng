#ifndef QDLIBTMPFILE_H
#define QDLIBMPFILE_H

#include "tools/GlobalParams.h"
#include "tools/fstools.h"

#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define QDNG_TMPFILE_BASE "QDng_tmp_XXXXXX"

namespace QDLIB {

   /**
    * Temporary file.
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
         
         void Seek(size_t pos);
         size_t Read(T* buf, size_t count);
         void Write(T* buf, size_t count);
         
         
         bool isOpen() { return _open; }
         size_t Size() { return _size; }
   };

   
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
         Remove(_fname);
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
      char templ[] = QDNG_TMPFILE_BASE;
      char* name = mktemp(templ);
      if (name == NULL)
         throw (EIOError("Can't create tmp file name", name));
      
      _fname = tmpdir + name;
      
      _fd = open(_fname.c_str(), O_CREAT|O_RDWR, 0600);
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
            Remove(_fname);
            throw(EIOError(errno, _fname));
         }
         _mmap = true;
         return (T*) _mapbase;
      }
      
      return NULL;
   }
   
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
         Remove(_fname);           
         throw (EIOError(errno, _fname));
      }

       
      /* Remap memory */
      if (_mmap) {        
        _mapbase = mremap(_mapbase, oldsize*sizeof(T), size*sizeof(T), MREMAP_MAYMOVE);
        
        if (_mapbase == MAP_FAILED){
           close(_fd);
           _open = false;
           Remove(_fname);
           throw(EIOError(errno, _fname));
        }
        _mmap = true;
        return (T*) _mapbase;
      }
      
      return NULL;
   }
   
   /**
    * Close and remove the tmp file when requeseted.
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
      
      if (remove) Remove(_fname);
      
      _fname.clear();
   }
}


#endif