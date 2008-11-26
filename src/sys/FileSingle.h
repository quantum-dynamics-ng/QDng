#ifndef FILESINGLE_H
#define FILESINGLE_H

#include <string>

#include "ParamContainer.h"
#include "KeyValFile.h"
#include "Exception.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"


#define BINARY_WF_SUFFIX ".wf"
#define BINARY_O_SUFFIX ".op"

#define ASCII_WF_SUFFIX ".awf"
#define ASCII_O_SUFFIX ".aop"

#define METAFILE_SUFFIX ".meta"

namespace QDLIB {



   
   /**
    * I/O base class for a single data file.
    * This is a template class which can handle all general
    * classes which support a basic interface, with at least
    * the following methods:
    * 
    * - T* C.begin()
    * - size_t C.sizeByte()
    * - ParamContainer& C.GetParams()
    * - C.Init(ParamContainer)
    * - string& C.Name()
    * 
    * The class needs a base name and file suffix.
    * In the case of binary or ASCII files a meta will be created
    * carrying the information about its content.
    * 
    * \todo implement ASCII+HDF5
    */
   template <class C>
   class FileSingle {
      public:
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
      private:
         bool _drop_meta;      // Ignore metadata
	 
	 void _ReadMeta(C *data);
	 void _WriteMeta(C *data);
	 
         void _WriteFileBinary(C *data);
	 bool _WriteFileASCII(C *data);
// 	 bool _WriteFileHDF(WaveFunction &wf);
	 
         void _ReadFileBinary(C *data);
	 bool _ReadFileASCII(C *data);
// 	 bool _ReadFileHDF(WaveFunction &wf);
      protected:
	StorageType _type;
	string _name;
	string _suffix;
	bool _sequence;
	int _counter;
      public:
	 	 
	 FileSingle();
	 FileSingle(const StorageType type, bool Sequence = false);
	 FileSingle(const StorageType type, const string &name, const string &suffix, bool Sequence = false);
	 
	 
         void Format(const StorageType type);
         StorageType Format();
         
	 void Name(const string &name);
         string Name();
         
	 void Suffix(const string &suffix);
	 
         void DropMeta(bool drop);
         bool DropMeta();
         
	 
	 void ActivateSequence();
	
	 void ResetCounter();
         
	 int Counter();
	 void Counter(int counter);
	 
         void ReadFile(C *data);
         void WriteFile(C *data);
	
         void operator<<(C *out);
   };

   /**
    * Operator for reading from file.
    */
   template <class C> 
   void operator>>(FileSingle<C> &file, C *in)
   {
      file.ReadFile(in);
   }
      
   
   /**
    * Default constructor.
    * 
    * Default type is binary.
    */
   template <class C>
   FileSingle<C>::FileSingle() :
	 _drop_meta(false), _type(binary),  _name("default"), _suffix(""), _sequence(false),  _counter(0) {}
   
   /**
    * Constructor with type initialisation.
    *
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, bool Sequence) :
	 _type(type), _drop_meta(false), _name("default"), _suffix(""), _sequence(Sequence), _counter(0) {}
   
   /**
    * Constructor with type and name initialisation.
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, const string &name, const string &suffix, bool Sequence) :
	 _type(type), _name(name), _suffix(suffix), _sequence(Sequence), _counter(0){}
   
   
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
   void FileSingle<C>::Name(const string &name)
   {
      _name = name;
   }
   
   /**
    * Set the file name suffix.
    * 
    * Is appended to all file names.
    */
   template <class C>
   void FileSingle<C>::Suffix(const string &suffix)
   {
      _suffix = suffix;
   }
   
   /**
    * Ignore meta data information.
    * 
    * This holds for reading and writing.
    * Warning: For reading this means that the target object have to already be initialized (incl. correct size)
    * 
    * \param drop if true, meta data will be ignored.
    */
   template <class C>
   void FileSingle<C>::DropMeta(bool drop)
   {
      _drop_meta = drop;
   }
   
   /**
    * Ignore meta data information.
    * \return true if meta data information is ignored.
    */
   template <class C>
   bool FileSingle<C>::DropMeta()
   {
      return _drop_meta;
   }
   
   /**
    * Turns on writing an interpretion of counters in file name.
    * 
    * Appends a counter number to the file name.
    * This makes it possible to read write whole a whole set of file
    * without thinking about the file numbers.
    * 
    * If switch between reading and the writing the counter will not be reseted.
    */
   template <class C>
   void FileSingle<C>::ActivateSequence()
   {
      _sequence = true;
   }

   /**
    * Reset the file sequence counter.
    */
   template <class C>
   void FileSingle<C>::ResetCounter()
   {
      _counter = 0;
   }
   
   /**
    * \return actual number of the file sequence counter.
    */
   template <class C>
   int FileSingle<C>::Counter()
   {
      return _counter;
   }
   
   /**
    * Set the file sequence counter.
    */
   template <class C>
   void FileSingle<C>::Counter(int counter)
   {
      _counter = counter;
   }
   
   template <class C>
   void FileSingle<C>::_WriteMeta(C *data)
   {
      ParamContainer p;
      
      /* Write meta file. In a sequence only for the first file. */
      if (!_drop_meta || _counter > 0){
	 p = data->Params();
	 p.SetValue("CLASS", data->Name() );
	 
	 KeyValFile meta_file(_name + METAFILE_SUFFIX);
	 if ( !meta_file.Write(p) ) EIOError("Can not write meta file");
      }
      
   }
   
   
   /**
    * Write raw binary data.
    */
   template <class C>
   void FileSingle<C>::_WriteFileBinary(C *data)
   {
      
      ofstream file;
      string s;
      
      _WriteMeta(data);
      
      /* Build name */
      if (_sequence) {
	 stringstream ss;
	 ss << _counter;
	 s = _name + "_" + ss.str() + _suffix;
	 _counter++;
      } else {
	 s = _name + _suffix;
      }
      
      /* Write binary data */
      file.open(s.c_str(), ofstream::binary);
      if( ! file.is_open() ) throw( EIOError("Can not open binary file for writing") );
      /* Write multiple strides */
      for(int i=0; i < data->strides(); i++){
         file.write((char*) data->begin(i), data->sizeBytes() );
         if( file.bad() ) throw( EIOError("Can not write binary file") );
      }
      
   
   }
   
   template <class C>
   void FileSingle<C>::_ReadMeta(C *data)
   {
      /* Read meta file */
      if (!_drop_meta){
	 KeyValFile file(_name + METAFILE_SUFFIX);
	 ParamContainer p;
	 if ( !file.Parse(p) ) throw( EIOError("Can not read meta file") );
	 data->Init(p);
      }
   }
   
   /**
    * Read raw binary data.
    */
   template <class C>
   void FileSingle<C>::_ReadFileBinary(C *data)
   {
     
      string s;
      ifstream file;
      
      _ReadMeta(data);
      
      /* We need some parameters for reading */
      if (data->sizeBytes() <= 0) throw( EParamProblem("Wrong size") ) ;
      
      /* Build file name */
      if (_sequence) {
	 stringstream ss;
	 ss << _counter;
	 s = _name + "_" + ss.str() + _suffix;
	 _counter++;
      } else {
	 s = _name + _suffix;
      }
      
      file.open(s.c_str(), ios::binary);
      if( file.fail() ) {
	 cout << "can't open\n";
	 throw ( EIOError("Can not open binary file for reading") );
      }
      
      /* Read multiple strides */
      for(int i=0; i < data->strides(); i++){
	 file.read((char*) data->begin(i), data->sizeBytes());
	 if( file.fail() || file.eof() ){
	    throw( EIOError("Can not read binary file") );
	 }
      }
	  
      file.close();
   
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
         case binary:
               _WriteFileBinary(data);
         break;
	 case ascii:
               throw(Exception("ASCII Unsupported"));
	 case hdf5:
               throw(Exception("HDF5 Unsupported"));
      }
   }

   /**
    * File reader method.
    * 
    * \param data Data to save.
    * 
    * \todo Add a check to meta data, for right class name
    */
   template <class C>
         void FileSingle<C>::ReadFile(C *data)
   {
      switch(_type){
         case binary:
               _ReadFileBinary(data);
         break;
	 case ascii:
               throw(Exception("ASCII Unsupported"));
	 case hdf5:
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
