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
    * - strides()
    * 
    * Basic support for strided vectors.
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
	 int _counter_last;
         
	 void _ReadMeta(C *data);
	 void _WriteMeta(C *data);
	 void _WriteMetaStrided(C *data, int stride);
	 
         void _WriteFileBinary(C *data);
	 void _WriteFileBinaryStrided(C *data);
	 
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
        bool _sequence_init; /* indicates that this is the first step of the sequence */
	int _counter;
	int _increment;
        void ReadMeta(ParamContainer &p);
        void WriteMeta(ParamContainer &p);
      public:
	 	 
	 FileSingle();
	 FileSingle(const StorageType type, bool Sequence = false);
	 FileSingle(const StorageType type, const string &name, const string &suffix, bool Sequence = false);
	 
	 
         void Format(const StorageType type);
         StorageType Format();
         
	 void Name(const string &name);
         string Name();
         
	 void Suffix(const string &suffix);
	 string Suffix();
	 
         void DropMeta(bool drop);
         bool DropMeta();
         
	 void ActivateSequence();
         void ActivateSequence(int increment);
	 void StopSequence();
	 
	 void ResetCounter();
         
	 void ReverseSequence(){ _increment = -1; }
	 void ForwardSequence(){ _increment = +1; }
	 
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
         _drop_meta(false), _counter_last(0), _type(binary),  _name("default"), _suffix(""), _sequence(false),
                    _counter(0), _increment(1), _sequence_init(false) {}
   
   /**
    * Constructor with type initialisation.
    *
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, bool Sequence) :
         _type(type), _drop_meta(false), _counter_last(0), _name("default"), _suffix(""), _sequence(Sequence),
               _counter(0),_increment(1), _sequence_init(false) {}
   
   /**
    * Constructor with type and name initialisation.
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, const string &name, const string &suffix, bool Sequence) :
         _counter_last(0), _type(type), _name(name), _suffix(suffix), _sequence(Sequence),
               _counter(0), _increment(1), _sequence_init(false) {}
   
   
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
    * Get the file name.
    */
   template <class C>
   string FileSingle<C>::Name()
   {
      return _name;
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
    * Get the file name suffix.
    * 
    * Is appended to all file names.
    */
   template <class C>
   string FileSingle<C>::Suffix()
   {
      return _suffix;
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
      _sequence_init = true;
   }
 
   /**
    * Turns on writing an interpretion of counters in file name.
    * 
    * Appends a counter number to the file name.
    * This makes it possible to read write whole a whole set of file
    * without thinking about the file numbers.
    * 
    * If switch between reading and the writing the counter will not be reseted.
    * 
    * \param increment The counters increment value
    */
   template <class C>
   void FileSingle<C>::ActivateSequence(int increment)
   {
      _sequence = true;
      _increment = increment;
      _sequence_init = true;
   }
   
   /**
    * Stop Sequence writing.
    */
   template <class C>
   void FileSingle<C>::StopSequence()
   {
      _sequence = false;
      _sequence_init = false;
   }
   
   /**
    * Reset the file sequence counter.
    */
   template <class C>
   void FileSingle<C>::ResetCounter()
   {
      _counter = 0;
      _counter_last = 0;
      
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
      _counter_last = _counter;
      
   }
   
   template <class C>
   void FileSingle<C>::_WriteMeta(C *data)
   {
      ParamContainer p;
      
      p = data->Params();
      p.SetValue("CLASS", data->Name() );
      
      WriteMeta(p);
   }
   
   /** Write named meta file from params */
   template <class C>
   void FileSingle<C>::WriteMeta(ParamContainer &p)
   {
      /* Write meta file. In a sequence only for the first file. */
      if (!_drop_meta || _counter != _counter_last){
	 
         KeyValFile meta_file(_name + METAFILE_SUFFIX);
         if ( !meta_file.Write(p) ) EIOError("Can not write meta file");
      }
   }
   
   template <class C>
   void FileSingle<C>::_WriteMetaStrided(C *data, int stride)
   {
      ParamContainer p;
      
      /* Write meta file. In a sequence only for the first file. */
      if (!_drop_meta || _counter != _counter_last){
	 p = data->Params();
	 p.SetValue("CLASS", data->Name() );
	 
	 stringstream ss;
	 ss << stride;
	 
	 KeyValFile meta_file(_name + "-" + ss.str() +METAFILE_SUFFIX);
	 if ( !meta_file.Write(p) ) EIOError("Can not write meta file");
      }
      
   }
   
   /**
    * Write raw binary data.
    * 
    * Version for single stride of Vector.
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
         _counter_last = _counter;
	 _counter += _increment;
      } else {
	 s = _name + _suffix;
      }
      
      /* Write binary data */
      file.open(s.c_str(), ofstream::binary);
      if( ! file.is_open() ) throw( EIOError("Can not open binary file for writing", s) );
      
      file.write((char*) data->begin(0), data->sizeBytes() );
      if( file.bad() ) throw( EIOError("Can not write binary file", s) );
      
      file.close();
      _sequence_init = false;
   }
   
   /**
    * Write raw binary data.
    * 
    * Version for strided Vector.
    * Every stride is saved as in an own file.
    */
   template <class C>
   void FileSingle<C>::_WriteFileBinaryStrided(C *data)
   {
      
      ofstream file;
      string s;
      
      
      for(int i=0; i < data->strides(); i++){ /* run over strides */
	 stringstream stride;
	 
	 _WriteMetaStrided(data, i);
	 stride << i;
	 
	 /* Build name */
	 if (_sequence) {
	    stringstream ss;
	    ss << _counter;
	    s = _name + "-" + stride.str() + "_" + ss.str() + _suffix;
	 } else {
	    s = _name + "-" + stride.str() + _suffix;
	 }
	 
	 /* Write binary data */
	 file.open(s.c_str(), ofstream::binary);
	 if( ! file.is_open() ) throw( EIOError("Can not open binary file for writing", s) );	 
	 file.write((char*) data->begin(i), data->sizeBytes() );
	 if( file.bad() ) throw( EIOError("Can not write binary file", s) );
	 file.close();
      
      }
      
      if (_sequence) _counter += _increment;
   }
   
   template <class C>
   void FileSingle<C>::_ReadMeta(C *data)
   {
      /* Read meta file & Init */
      if (!_drop_meta){
	 ParamContainer p;
         ReadMeta(p);
	 data->Init(p);
      }
   }
   
   /**
    * Read Metafile & save in ParamContainer.
    */
   template <class C>
   void  FileSingle<C>::ReadMeta(ParamContainer &p)
   {
      string name;
      
      name = _name + METAFILE_SUFFIX;
      KeyValFile file(name);

      if ( !file.Parse(p) ) {
         /* try again, but remove trailing underscore + further chars */
         name = _name.substr(0,_name.rfind('_')) + METAFILE_SUFFIX;
         file.SetName( name );
         if ( !file.Parse(p) ) 
            throw( EIOError("Can not read meta file", name) );
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
      
      
      /* Read multiple strides */
      for (int i=0; i < data->strides(); i++){
	 /* Build file name */
	 if (_sequence) {
	    stringstream ss;
	    
	    if (data->strides() > 1)
	       ss << "-" << i << "_" << _counter;
	     else 
	       ss << "_" << _counter;

	    s = _name +  ss.str() + _suffix;
            _counter_last = _counter;
	    _counter += _increment;
	 } else {
	    if (data->strides() > 1){
	       stringstream ss;
	       ss << "-" << i;
	       s = _name + ss.str() + _suffix;
	    } else {
	       s = _name + _suffix;
	    }
	 }
	 
	 file.open(s.c_str(), ios::binary);
	 if( file.fail() ) {
	    throw ( EIOError("Can not open binary file for reading", s) );
	 }
	 
	 file.read((char*) data->begin(i), data->sizeBytes());
	 if( file.fail() || file.eof() ){
	    throw( EIOError("Can not read binary file", s) );
	 }
	 
	 file.close();
      }
   
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
	        if (data->strides() == 1)
                  _WriteFileBinary(data);
		else _WriteFileBinaryStrided(data);
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
