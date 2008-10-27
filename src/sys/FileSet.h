#ifndef FILESET_H
#define FILESET_H

#include <string>
#include "fileSet.h"


namespace QDLIB {

   
   /**
    * I/O  class for a systematic set of files.
    * 
    * \todo formating of file name: think about leading zeroes.
    */
   template <class C>
   class FileSet {
      private:
	 int _counter;
	 string _basename;
         FileSingle<C> _file;
      public:
	 FileSet();
	 FileSet(const string basename);
         FileSet(const string basename, FileSingle<C> *file);
	 
         void BaseName(const string basename);
         string& BaseName(const string basename);
         
         void File(FileSingle<C> *file);
         FileSingle<C>* File();
               
               
	 void Reset();
         
	 int Counter();
         int Counter(int counter);
         
         void ReadNext(C *data);
         void WriteNext(C *in);
         
	 void operator << (C* in);
	 
   };

   template <class C>
   void operator>>(FileSet<C> set, C *out)
   {
      set.ReadNext(out);
   }
         
   template <class C>
   FileSet<C>::FileSet::FileSet(): _counter(0) {}
   
   template <class C>
   FileSet<C>::FileSet(const string basename): FileSet(), _basename(basename),  _file(NULL) {}
   
   template <class C>
   FileSet<C>::FileSet(const string basename, FileSingle<C> *file): FileSet(basename),  _file(file) {}
   
   template <class C>
   void FileSet<C>::BaseName(const string basename)
   {
      _basename = basename;
   }
   
   template <class C>
   string& FileSet<C>::BaseName()
   {
      return _basename;
   }
   
   /**
    * Reset file number counter.
    */
   template <class C>
         void FileSet<C>::Reset()
   {
      _counter=0;
   }

   /**
    * Returns the file counter.
    */
   template <class C>
         int FileSet<C>::Counter()
   {
      return _counter;
   }
   
   /**
    * Set the counter.
    */
   template <class C>
         void FileSet<C>::Counter(int counter)
   {
      _counter = counter;
   }

   
   /**
    * Reads an file from an open FileSet.
    */
   template <class C>
   void FileSet<C>::ReadNext(C *data)
   {
      string name;
      stringstream name;
      
      /* prepare the filename: basename_counter */
      
      
      if (file.Format() == hdf5){
         ss  << _counter;
      } else {
         ss << _basename  << "_" << _counter;
      }
      
      file.Name(ss.str());
      file >> data;
   }
   
   /** 
    * Write a file to an open FileSet
    */
   void FileSet<C>::WriteNext(C *in)
   {
      
      stringstream name;
      
      if (file.Format() == hdf5){
         ss  << _counter;
      } else {
         ss << _basename  << "_" << _counter;
      }
      
      file.Name(ss.str());
      
      file << in;
   }
   
   template <class C>
   void FileSet<C>::operator<<(C* in)
   {
      WriteNext(in);
   }

   
} /* namespace QDLIB */

#endif /* #ifndef FILESET_H */
