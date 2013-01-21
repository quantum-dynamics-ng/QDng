#ifndef FILESINGLE_H
#define FILESINGLE_H

#include <string>
#include <iostream>
#include <fstream>

#include "ParamContainer.h"
#include "Exception.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"


#define BINARY_WF_SUFFIX ".wf"
#define BINARY_O_SUFFIX ".op"

#define ASCII_WF_SUFFIX ".awf"
#define ASCII_O_SUFFIX ".aop"

#define METAFILE_SUFFIX ".meta"

using namespace std;

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
	    /** meta data+binary raw data stream format **/
	    stream,
	    /** human readable ascii format */
	    ascii,
	    /** HDF5 specific format */
	    hdf5
	 };
      private:
	      istream* _sin;    /* I/O streams used for StorageType streams */
	      ostream* _sout;

         bool _drop_meta;      // Ignore metadata
         int _counter_last;
         int _fsize;          /* Specify write size other than suggested by vector etc. */
         size_t _freadsize;      /* Contains number of bytes from last read operation */
         
         void _ReadMeta(C *data);
         void _WriteMeta(C *data);
         void _WriteMetaStrided(C *data, int stride);
	 
         void _WriteFileStream(C *data);
         void _ReadFileStream(C *data);

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
            FileSingle(istream* in, ostream out);
	 
	 
            /**
             * Set input stream for stream storage format.
             *
             * Only relevant if Format = stream.
             */
            void SetInputStream(istream& s) { _sin = &s; }

            /**
             * Set output stream for stream storage format.
             *
             * Only relevant if Format = stream.
             */
            void SetOutputStream(ostream& s) { _sout = &s; }

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
	 
         /** Get true file size.
          *
          * After reading this returns the true size.
          */
         size_t FileSize() const { return  _freadsize; }
         
         /**
          * Set the true file size for writing.
          * 
          * \param size if 0 it is ignored.
          */
         void FileSize(lint size) { _fsize = size; }
         
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
   FileSingle<C>::FileSingle() : _sin(NULL), _sout(NULL),
         _drop_meta(false), _counter_last(0), _fsize(0), _freadsize(0), _type(binary),  _name("default"), _suffix(""), _sequence(false),
                    _sequence_init(false), _counter(0), _increment(1) {}
   
   /**
    * Constructor with type initialisation.
    *
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, bool Sequence) : _sin(NULL), _sout(NULL),
         _type(type), _drop_meta(false), _counter_last(0), _fsize(0), _freadsize(0), _name("default"), _suffix(""), _sequence(Sequence),
               _sequence_init(false), _counter(0),_increment(1) {}
   
   /**
    * Constructor with type and name initialisation.
    */
   template <class C>
   FileSingle<C>::FileSingle(const StorageType type, const string &name, const string &suffix, bool Sequence) :
         _sin(NULL), _sout(NULL),
         _counter_last(0), _fsize(0), _freadsize(0), _type(type), _name(name), _suffix(suffix), _sequence(Sequence),
                       _sequence_init(false), _counter(0), _increment(1) {}
   
   
   template <class C>
   FileSingle<C>::FileSingle(istream* in, ostream out) : _sin(in), _sout(out),
   _counter_last(0), _fsize(0), _freadsize(0), _type(stream), _name(), _suffix(), _sequence(false),
                         _sequence_init(false), _counter(0), _increment(1)  {}

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
	 
         if ( ! p.WriteToFile(_name + METAFILE_SUFFIX) ) EIOError("Can not write meta file");
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
	 
	 if ( !p.WriteToFile(_name + "-" + ss.str() +METAFILE_SUFFIX) ) EIOError("Can not write meta file");
      }
      
   }
   
   /**
    * Write object to stream.
    */
   template <class C>
   void FileSingle<C>::_WriteFileStream(C *data)
   {
      if (_sout != NULL){
         /* Punch Metadata */
         ParamContainer& pm = data->Params();
         pm.SetValue("CLASS", data->Name());
         *_sout << "BEGINMETA\n";
         *_sout << data->Params();
         *_sout << "ENDMETA\n";

         /* Punch binary data */
         size_t size =  data->sizeBytes();
         if (_fsize != 0) size = _fsize;


         *_sout << "BEGINDATA\n";
         *_sout << size << endl;

         _sout->write(reinterpret_cast<char*>(data->begin(0)), size);
         *_sout << endl;
      } else
         throw(EIncompatible("No output stream set!"));

   }

   /**
    * Read object from stream storage format.
    *
    */
   template <class C>
   void FileSingle<C>::_ReadFileStream(C *data)
   {
      char* buf = NULL;
      size_t size = 0;

      if (_sin != NULL ){
         while (1==1){
            string line;
            *_sin >> line;

            cout << "dbg: " << line << endl;
            if (string("BEGINMETA") == line){ /* Extract meta data*/
               _sin->rdbuf()->sputn(line.c_str(), line.length());
               ParamContainer pm;
               ReadMeta(pm);
               if (!_drop_meta) data->Init(pm);
            } else if (string("BEGINDATA") == line){ /* extract binary data */
               *_sin >> size;
               cout << "size: " <<size << endl;
               buf = (char*) malloc(size);
               _sin->read(buf, size);
               cout << "read: " <<_sin->gcount() << endl;
               /** \bug if we get kicked out by throw, there will be a mem leak */
            } else
               break;
         }

         /* Copy binary data to object */
         if (buf != NULL){
            if (size > data->sizeBytes()){
               free(buf);
               throw(EParamProblem("Provided data to large for object"));
            }
            /* clear memory if necessary */
            if (size < data->sizeBytes())
               memset((char*) data->begin(0) + size, 0, data->sizeBytes() - size);

            memcpy((char*) data->begin(0), buf, size);

            free(buf);

            _freadsize = size;
         }
      } else
         throw(EIncompatible("No input stream set!"));
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
      file.open(s.c_str(), ios_base::binary);
      if( ! file.is_open() ) throw( EIOError("Can not open binary file for writing", s) );
      
      if (_fsize == 0)
         file.write((char*) data->begin(0), data->sizeBytes() );
      else 
         file.write((char*) data->begin(0), _fsize );
      
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
	 file.open(s.c_str(), ios_base::binary);
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
	 ParamContainer& p = data->Params();
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
      
      if (_type == stream){
         /* strip metadata from stream */
         if (_sin != NULL) p.Parse(*_sin, "ENDMETA");
      } else { /* Try to obtain meta data from associated file */
         string name;

         name = _name + METAFILE_SUFFIX;

         if ( !p.ReadFromFile(name) ) {
            /* try again, but remove trailing underscore + further chars */
            if (_name.rfind('_') != string::npos) {
               name = _name.substr(0,_name.rfind('_')) + METAFILE_SUFFIX;

               if ( !p.ReadFromFile(name) )
                  throw( EIOError("Can not read meta file", name + " or " + _name) );
            } else {
               throw( EIOError("Can not read meta file", name + " or " + _name) );
            }
         }
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
      
#ifdef HAVE_MPI
      int rank = data->MPIrank();
      int size = data->MPIsize();
#endif
      /* Read multiple strides */
      for (int i = 0; i < data->strides(); i++) {
         /* Build file name */
         if (_sequence) {
            stringstream ss;

            if (data->strides() > 1)
               ss << "-" << i << "_" << _counter;
            else ss << "_" << _counter;

            s = _name + ss.str() + _suffix;
            _counter_last = _counter;
            _counter += _increment;
         } else {
            if (data->strides() > 1) {
               stringstream ss;
               ss << "-" << i;
               s = _name + ss.str() + _suffix;
            } else {
               s = _name + _suffix;
            }
         }

         file.open(s.c_str(), ios::binary);
         if (file.fail()) {
            throw(EIOError("Can not open binary file for reading", s));
         }

#ifdef HAVE_MPI
         if (i%size == rank){ /* This is our stride */
#endif
            file.read((char*) data->begin(i), data->sizeBytes());
            _freadsize = file.gcount();

            if (_freadsize <= 0) {
               throw(EIOError("Can not read binary file", s));
            }

#ifdef HAVE_MPI
         }
#endif

         file.close();
      }
      data->SyncStrides();
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
      
       switch (_type) {
            case binary:
               if (data->strides() == 1)
                  _WriteFileBinary(data);
               else _WriteFileBinaryStrided(data);
               break;
            case stream:
               if (data->strides() > 1) throw(EIncompatible("Stream storage not implemented for strided data"));
               _WriteFileStream(data);
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
            case stream:
               _ReadFileStream(data);
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
