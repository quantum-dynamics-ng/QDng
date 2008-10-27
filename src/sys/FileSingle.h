#ifndef FILESINGLE_H
#define FILESINGLE_H

#include <string>

#include "paramcontainer.h"
#include "KeyValFile.h"
#include "Exception.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"


#define BINARY_WF_SUFFIX ".wf"
#define BINARY_POT_SUFFIX ".pot"
#define BINARY_O_SUFFIX ".op"

#define ASCII_POT_SUFFIX ".apot"
#define ASCII_WF_SUFFIX ".awf"
#define ASCII_O_SUFFIX ".aop"

#define METAFILE_SUFFIX ".meta"

namespace QDLIB {

   /**
    * Defines the disk storage type of the data.
    */
   enum StorageType
   {
      /** unformated raw binary file */
      binary,
      /** human readable ascii format */
      ascii,
      /** HDF5 specific format */
      hdf5
   };

   
   /**
    * I/O base class for a single data file.
    * This is a template class which can handle all general
    * classes which support a basic interface, with at least
    * the folowing methods:
    * 
    * - T* C.begin()
    * - size_t C.sizeByte()
    * - ParamContainer& C.GetParams()
    * - C.Init(ParamContainer)
    * 
    * \todo implement ASCII+HDF5
    * \todo merge this back to FileSingle
    */
   template <class C>
   class FileSingle {
      private:
         bool _drop_meta;      // Ignore metadata
         void _WriteFileBinary(C *data);
// 	 bool _WriteFileASCII(WaveFunction &wf);
// 	 bool _WriteFileHDF(WaveFunction &wf);
	 
         void _ReadFileBinary(C *data);
// 	 bool _ReadFileASCII(WaveFunction &wf);
// 	 bool _ReadFileHDF(WaveFunction &wf);
      protected:
	StorageType _type;
	string _name;
      public:
	 FileSingle();
	 FileSingle(const StorageType type);
	 FileSingle(const StorageType type, const string name);
	 
         void Format(const StorageType type);
         StorageType Format();
         
	 void Name(const string name);
         string Name();
         
         void DropMeta(bool drop);
         bool DropMeta();
         
         void ReadFile(C *data);
         void WriteFile(C *data);
	
         void operator<<(C *out);
   };

   /**
    * Operator for reading from file.
    */
   template <class C> 
   void operator>>(FileSingle<C> file, C *in)
   {
      file->WriteFile(in);
   }
      
   
   /**
    * Default constructor.
    * 
    * Default type is binary.
    */
   template <class C>
   FileSingle<C>::FileSingle() : _type(binary) {}
   
   /**
    * Constructor with type initilisation.
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type): _type(type) {}
   
   /**
    * Constructor with type and name initilisation.
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, const string name): FileSingle(binary), _name(name) {}
   
   
   /**
    * Set disk storage format.
    * 
    * \param type Disk storage format
    */
   template <class C>
   void FileSingle<C>::Format(const StorageType type)
   {
      _type = type;
   }
   
   /**
    * Set the file name.
    */
   template <class C>
   void FileSingle<C>::Name(const string name)
   {
      _name = name;
   }
   
   /**
    * Write raw binary data.
    */
   template <class C>
   void FileSingle<C>::_WriteFileBinary(C *data)
   {
      ParamContainer p;
      ofstream file;
      string s;
      
      /* Write meta file */
      if (!_drop_meta){
         KeyValFile meta_file(_name + METAFILE_SUFFIX);
         if ( !meta_file.Write(data->GetParams()) ) EIOError("Can not write meta file");
      }
      
      p = data->GetParams();
      
      
      /* Determine the suffix */
      if ( typeid(WaveFunction) == typeid(data) ) {
         s = _name + BINARY_WF_SUFFIX;
      } else if ( typeid(Operator) == typeid(data) ) {
         s = _name + BINARY_O_SUFFIX;
      }
      
      /* Write binary data */
      file.open(_name.c_str(), ofstream::binary);
      if( ! file.is_open() ) throw( EIOError("Can not open binary file for writing") );
      file.write((char*) data->begin(), data->sizeBytes() );
      if( file.bad() ) throw( EIOError("Can not write binary file") );
      
   
   }
   
   /**
    * Read raw binary data.
    */
   template <class C>
   void FileSingle<C>::_ReadFileBinary(C *data)
   {
     
      string s;
      ifstream file;
      
      /* Read meta file */
      if (!_drop_meta){
         KeyValFile file(_name + METAFILE_SUFFIX);
         ParamContainer p;
         if ( !file.Parse(p) ) throw( EIOError("Can not read meta file") );
         data.Init(p);
      }
      
      
      /* We need some parameters for reading */
      if (data->sizeByte <= 0) throw( EParamProblem("Wrong size") ) ;
      
      /* Determine the suffix */
      if ( typeid(WaveFunction) == typeid(data) ) {
         s = _name + BINARY_WF_SUFFIX;
      } else if ( typeid(Operator) == typeid(data) ) {
         s = _name + BINARY_O_SUFFIX;
      }      
      
      
      file.open(s.c_str(), ifstream::binary);
      if( ! file.is_open() ) throw ( EIOError("Can not open binary file for reading") );
      file.read(data.begin(), data.sizeByte());
      if( file.bad() ) throw( EIOError("Can not read binary file") );
      
   
   }

   /**
    * File writer method.
    * 
    * \param data Data to save.
    * 
    */
   template <class C>
   void FileSingle<C>::WriteFile(C *data)
   {
      switch(_type){
         binary:
               return _WriteFileBinary(data);
         break;
	 ascii:
               throw(Exception("ASCII Unsupported"));
	 hdf5:
               throw(Exception("HDF5 Unsupported"));
      }
   }

   /**
    * File reader method.
    * 
    * \param data Data to save.
    * 
    */
   template <class C>
         void FileSingle<C>::ReadFile(C *data)
   {
      switch(_type){
         binary:
               return _ReadFileBinary(data);
         break;
	 ascii:
               throw(Exception("ASCII Unsupported"));
	 hdf5:
               throw(Exception("HDF5 Unsupported"));
      }
   }   
   
   /**
    * Write data into file.
    */
   template <class C>
   void FileSingle<C>::operator<<(C *out)
   {
      WriteFile(out);
   }
   

   
} /* namespace QDLIB */


#endif /* #ifndef FILESINGLE_H */
