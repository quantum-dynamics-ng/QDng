#ifndef FILESINGLE_H
#define FILESINGLE_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "config.h"

#ifdef HAVE_LIBZ
  #include <zlib.h>
#endif

#ifdef HAVE_LIBBZ2
  #include <bzlib.h>
#endif

#define ZLIB_MAGIC "ZLIBCOM"
#define BZIP_MAGIC "BZIPCOM"

/** Length of the magic + terminating zero */
#define ZLIB_MLEN 8


#include "ZCopyStream.h"
#include "FileSingleHeader.pb.h"

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>

#include "ParamContainer.h"
#include "Exception.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"

#include <stdint.h>

#define BINARY_WF_SUFFIX ".wf"
#define BINARY_O_SUFFIX ".op"

#define ASCII_WF_SUFFIX ".awf"
#define ASCII_O_SUFFIX ".aop"

#define METAFILE_SUFFIX ".meta"

#define PBUF_MAGIC "QDngPrtoBuf"
#define PBUF_VERSION 0x1


using namespace std;
using namespace ::google::protobuf;
using namespace ::google::protobuf::io;

namespace QDLIB
{

   /**
    * Wrapper to the original message header.
    *
    * This couples a ParamContiener object to the meta_data entry.
    */
   class FileSingleHeaderPm : public FileSingleHeader
   {
      private:
         ParamContainer _params;
         bool _has_params;
      public:
         FileSingleHeaderPm() : _has_params(false) {}


         void Clear()
         {
            FileSingleHeader::Clear();
            clear_params();
         }

         bool has_params () const { return _has_params; }
         void clear_params()
         {
            _params.clear();
            _has_params = false;
         }

         const ParamContainer& params()
         {
            if (!_has_params && has_meta_data()){
               _params.Parse(meta_data());
               _has_params = true;
            }

            return _params;
         }

         void set_params(ParamContainer& pm)
         {
            _params = pm;

            stringstream ss;
            _params.Write(ss, true);
            FileSingleHeader::set_meta_data(ss.str());

            string classname;
            pm.GetValue("CLASS", classname);
            FileSingleHeader::set_class_(classname);

         }

         void set_meta_data(const ::std::string& value)
         {
            _params.Parse(value);
            FileSingleHeader::set_meta_data(value);
         }

         void set_meta_data(const char* value)
         {
            set_meta_data(string(value));
         }

         void set_meta_data(const char* value, size_t size)
         {
            string s;
            s.assign(value, size);
            set_meta_data(s);
         }


   };


   /**
    * Defines to a identify the begin of stream as such.
    */
   struct PbufHeader
   {
         char magic[sizeof(PBUF_MAGIC)];
         unsigned int version;
         PbufHeader() :
            version(PBUF_VERSION)  { memcpy(magic, PBUF_MAGIC, sizeof(PBUF_MAGIC)); }
   };


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
   template<class C>
   class FileSingle
   {
//      friend class FileSingleTest;
      public:
         /**
          * Defines the compression method.
          */
         typedef enum
         {
            ZLIB, BZIP, UNCOMPRESSED, INVALID
         } compMethod_t;

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
            hdf5,
            /** autodetect **/
            autodetect
         };

      private:
         FileSingleHeaderPm MetaData;

         StorageType _type;

         string _name;
         string _suffix;

         bool _sequence;
         bool _sequence_init; /* indicates that this is the first step of the sequence */

         int _counter;
         int _increment;

         istream* _sin;            /* I/O streams used for StorageType streams */
         bool _our_in_stream;
         bool _intro_read;

         ostream* _sout;
         bool _our_out_stream;
         bool _intro_written;

         char* _buffer;
         size_t _buffer_size;

         bool _drop_meta; // Ignore metadata
         int _counter_last;
         int _fsize; /* Specify write size other than suggested by vector etc. */
         size_t _freadsize; /* Contains number of bytes from last read operation */

#ifdef HAVE_LIBZ
         void _CompressZLIB(C * data);
         void _DecompressZLIB(istream *in, size_t size, C* data);
#endif

#ifdef HAVE_LIBBZ2
         void _CompressBZIP(C * data);
         void _DecompressBZIP(istream *in, size_t size, C* data);
#endif

         FileSingleHeader_Compression DetectCompressionInPayload();

//         void _ReadMeta(C *data);
//         void _WriteMeta(C *data);
//
//         void _WriteMetaStrided(C *data, int stride);
//
//         void _WriteFileStream(C *data);
//         void _ReadFileStream(C *data);
//
//         void _WriteFileBinary(C *data);
//         void _WriteFileBinaryStrided(C *data);
//
//         bool _WriteFileASCII(C *data);
//
//         void _ReadFileBinary(C *data);
//         bool _ReadFileASCII(C *data);
      protected:


         compMethod_t _compMethod; /* Compression method */

         void BuildMetaFileName(string &name);
         void BuildDataFileName(string &name);

         /* Stream related functions */
         void WriteMetaToStream(C* data, size_t payload_size, bool more_files_follow = false, bool ingore_data = false);
         void WriteIntroToStream();
         void PrepareStreamPayload(C *data, stringstream& buffer);
         void WriteDataToStream(const string &data_buf);
         void FinalizeOutStream();


         void OpenINStream();
         void CheckIntroFromStream();
         const FileSingleHeaderPm& ReadMetaFromStream();

         void ReadDataFromStream(C *data);

         /* Raw binary functions */
         void ReadMetaFromRaw();
         void ReadDataFromRaw(C* data);

         void WriteMetaToRaw(C *data);
         void WriteDataToRaw(C *data);

         /* General */
         void PrepareData(C *data);
         void InitDataStorage(C* data);
         const FileSingleHeaderPm& ReadMeta();

         void ReadMeta(ParamContainer &p);
         void ReadData(C* data);

         void WriteMeta(C* data, bool more_files_follow = false, bool ingore_data = false);
         void WriteData(C* data);


         void WriteMeta(ParamContainer &p);



      public:

         FileSingle();
         ~FileSingle();
//         FileSingle(const StorageType type, bool Sequence = false);
//         FileSingle(const StorageType type, const string &name, const string &suffix,
//                  bool Sequence = false);
//         FileSingle(istream* in, ostream out);

         /**
          * Set input stream for stream storage format.
          *
          * Only relevant if Format = stream.
          */
         void SetInputStream(istream& s)
         {
            _sin = &s;
         }

         /**
          * Set output stream for stream storage format.
          *
          * Only relevant if Format = stream.
          */
         void SetOutputStream(ostream& s)
         {
            _sout = &s;
         }

         void Format(const StorageType type);
         StorageType Format() const { return _type; }

         void DetectFormat();

         void Name(const string &name);
         string Name();

         void Suffix(const string &suffix);
         string Suffix();

         const FileSingleHeaderPm& GetMeta() { return MetaData; }

         void Compression(compMethod_t method) { _compMethod = method; }
         compMethod_t Compression() { return _compMethod; }

         void DropMeta(bool drop);
         bool DropMeta() const;

         void ActivateSequence();
         void ActivateSequence(int increment);
         void StopSequence();

         void ResetCounter();

         void ReverseSequence() { _increment = -1; }
         void ForwardSequence() { _increment = +1; }

         int Counter() const;
         void Counter(int counter);

         /** Get true file size.
          *
          * After reading this returns the true size.
          */
         size_t FileSize() const
         {
            return _freadsize;
         }

         /**
          * Set the true file size for writing.
          *
          * \param size if 0 it is ignored.
          */
         void FileSize(lint size)
         {
            _fsize = size;
         }

         void ReadFile(C *data);
         void WriteFile(C *data);

         void ReadSingleFileFromStream(C *data);
         void ReadSingleFileFromRaw(C *data);

         void WriteSingleFileToStream(C *data);
         void WriteSingleFileToRaw(C *data);


         void operator<<(C *out);
   };

   /**
    * Operator for reading from file.
    */
   template<class C>
   void operator>>(FileSingle<C> &file, C *in)
   {
      file.ReadFile(in);
   }

   /**
    * Default constructor.
    * 
    * Default type is binary.
    */
   template<class C>
   FileSingle<C>::FileSingle() :
     _type(autodetect), _sequence(false), _sequence_init(false),
     _counter(0), _increment(1), _sin(NULL), _our_in_stream(false), _intro_read(false),
     _sout(NULL), _our_out_stream(false), _intro_written(false),
     _buffer(NULL), _buffer_size(0),
     _drop_meta(false), _counter_last(0),  _fsize(0), _freadsize(0), _compMethod(UNCOMPRESSED)
   {
   }

   template<class C>
   FileSingle<C>::~FileSingle()
   {
     if (_buffer != NULL) Memory::Instance().Free(_buffer);
   }

   /**
    * Set disk storage format.
    * 
    * \param type Disk storage format
    */
   template<class C>
   void FileSingle<C>::Format(const StorageType type)
   {
      _type = type;
   }

   /**
    * Set the file name.
    */
   template<class C>
   void FileSingle<C>::Name(const string &name)
   {
      _name = name;
   }

   /**
    * Get the file name.
    */
   template<class C>
   string FileSingle<C>::Name()
   {
      return _name;
   }

   /**
    * Set the file name suffix.
    * 
    * Is appended to all file names.
    */
   template<class C>
   void FileSingle<C>::Suffix(const string &suffix)
   {
      _suffix = suffix;
   }
   /**
    * Get the file name suffix.
    * 
    * Is appended to all file names.
    */
   template<class C>
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
   template<class C>
   void FileSingle<C>::DropMeta(bool drop)
   {
      _drop_meta = drop;
   }

   /**
    * Ignore meta data information.
    * \return true if meta data information is ignored.
    */
   template<class C>
   bool FileSingle<C>::DropMeta() const
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
   template<class C>
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
   template<class C>
   void FileSingle<C>::ActivateSequence(int increment)
   {
      _sequence = true;
      _increment = increment;
      _sequence_init = true;
   }

   /**
    * Stop Sequence writing.
    */
   template<class C>
   void FileSingle<C>::StopSequence()
   {
      _sequence = false;
      _sequence_init = false;
   }

   /**
    * Reset the file sequence counter.
    */
   template<class C>
   void FileSingle<C>::ResetCounter()
   {
      _counter = 0;
      _counter_last = 0;

   }

   /**
    * \return actual number of the file sequence counter.
    */
   template<class C>
   int FileSingle<C>::Counter() const
   {
      return _counter;
   }

   /**
    * Set the file sequence counter.
    */
   template<class C>
   void FileSingle<C>::Counter(int counter)
   {
      _counter = counter;
      _counter_last = _counter;

   }

#ifdef HAVE_LIBZ
   /**
    * Compress output with ZLIB.
    */
   template<class C>
   void FileSingle<C>::_CompressZLIB(C * data)
   {
      /* zlib-compression */
      z_stream strm;

      memset((void*) &strm, 0, sizeof(strm));
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;

      if (deflateInit(&strm, 4) != Z_OK) /* Init stream */
         throw(EIOError("libz init failed"));

      _buffer_size = data->sizeBytes();
      Memory::Instance().Align((void**) &_buffer, _buffer_size);

      /* input buffer */
      strm.next_in = reinterpret_cast<Bytef*>(data->begin(0));
      strm.avail_in = data->sizeBytes();

      /* output buffer */
      strm.next_out = reinterpret_cast<Bytef*>(_buffer);
      strm.avail_out = _buffer_size;

      if (deflate(&strm, Z_FINISH) != Z_STREAM_END)
         throw (EIOError("zlib deflate failed"));

      /* This is what we have */
      _buffer_size = _buffer_size - strm.avail_out;

      deflateEnd(&strm);
   }

   /**
    * Decompress gziped content.
    */
   template<class C>
   void FileSingle<C>::_DecompressZLIB(istream *in, size_t size, C* data)
   {
      z_stream strm;

      memset((void*) &strm, 0, sizeof(strm));
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;

      if (inflateInit(&strm) != Z_OK) /* Init stream */
         throw(EIOError("z-lib init failed"));

      /* Read data */
      char* buffer;
      Memory::Instance().Align((void**) &buffer, size);

      in->read(buffer, size);

      if ((size_t) in->gcount() != size || in->bad()){
         Memory::Instance().Free(buffer);
         throw(EIOError("Can't read zlib compressed data"));
      }

      /* decompress */
      strm.next_in = reinterpret_cast<Bytef*>(buffer);
      strm.avail_in = size; /* Buffer - magic bytes */
      /* output buffer */
      strm.next_out = reinterpret_cast<Bytef*>(data->begin(0));
      strm.avail_out = data->sizeBytes();

      if (inflate(&strm, Z_FINISH) != Z_STREAM_END)
      { /* Compression */
         Memory::Instance().Free(buffer);
         throw(EIOError("zlib decompression failed"));
      }

      inflateEnd(&strm);

      Memory::Instance().Free(buffer);
   }
#endif


#ifdef HAVE_LIBBZ2
   /**
    * Compress output with BZIP.
    */
   template<class C>
   void FileSingle<C>::_CompressBZIP(C * data)
   {
      /* bzip2-compression */
      bz_stream strm;

      memset((void*) &strm, 0, sizeof(strm));
      strm.bzalloc = Z_NULL;
      strm.bzfree = Z_NULL;
      strm.opaque = Z_NULL;

      if (BZ2_bzCompressInit(&strm, 4, 0, 0) != BZ_OK) /* Init stream */
         throw(EIOError("libz init failed"));

      _buffer_size = data->sizeBytes();
      Memory::Instance().Align((void**) &_buffer, _buffer_size);

      /* input buffer */
      strm.next_in = reinterpret_cast<char*>(data->begin(0));
      strm.avail_in = data->sizeBytes();

      /* output buffer */
      strm.next_out = _buffer;
      strm.avail_out = _buffer_size;

      if (BZ2_bzCompress(&strm, BZ_FINISH) != BZ_STREAM_END)
         throw (EIOError("bzip2 compress failed"));

      /* This is what we have */
      _buffer_size = _buffer_size - strm.avail_out;

      if (BZ2_bzCompressEnd(&strm) != BZ_OK)
         throw (EIOError("bzip2 compress failed"));

   }

   /**
    * Decompress bziped content.
    */
   template<class C>
   void FileSingle<C>::_DecompressBZIP(istream *in, size_t size, C* data)
   {
      bz_stream strm;

      memset((void*) &strm, 0, sizeof(strm));
      strm.bzalloc = Z_NULL;
      strm.bzfree = Z_NULL;
      strm.opaque = Z_NULL;

      if (BZ2_bzDecompressInit(&strm,0 ,0) != BZ_OK) /* Init stream */
         throw(EIOError("bzip init failed"));

      /* Read data */
      char* buffer;
      Memory::Instance().Align((void**) &buffer, size);

      in->read(buffer, size);

      if ((size_t) in->gcount() != size || in->bad()){
         Memory::Instance().Free(buffer);
         throw(EIOError("Can't read bzip compressed data"));
      }

      /* decompress */
      strm.next_in = buffer;
      strm.avail_in = size; /* Buffer - magic bytes */
      /* output buffer */
      strm.next_out = reinterpret_cast<char*>(data->begin(0));
      strm.avail_out = data->sizeBytes();

      if (BZ2_bzDecompress(&strm) != BZ_STREAM_END)
      { /* Compression */
         Memory::Instance().Free(buffer);
         throw(EIOError("zlib decompression failed"));
      }

      BZ2_bzDecompressEnd(&strm);

      Memory::Instance().Free(buffer);
   }

#endif

   /**
    * Check for the compression type in raw binary file.
    *
    * We keep this here for compat reasons. This is the old
    * format. The new protobuf based format stores the information
    * in the header and not after the payload.
    */
   template<class C>
   FileSingleHeader_Compression FileSingle<C>::DetectCompressionInPayload()
   {
      string name;
      BuildDataFileName(name);

      ifstream file;

      file.open(name.c_str());

      if (!file.is_open())
           throw(EIOError("Can not open binary file for reading", name));

      /* Let's check if we find a compression tag */
      char lastbytes[ZLIB_MLEN];

      file.seekg(0, ios::end);
      size_t len = file.tellg();
      file.seekg(len-ZLIB_MLEN);

      file.read(reinterpret_cast<char*>(lastbytes), ZLIB_MLEN);

      if (file.gcount() < ZLIB_MLEN)
         throw (EIOError("Binary file truncated!? Can't check compression tag.", name));

      file.close();

      /* Check what we got */
      if (strncmp(lastbytes, ZLIB_MAGIC, ZLIB_MLEN ) == 0) MetaData.set_compression(FileSingleHeader_Compression_ZLIB);
      else if (strncmp(lastbytes, BZIP_MAGIC, ZLIB_MLEN ) == 0) MetaData.set_compression(FileSingleHeader_Compression_BZIP);
      else MetaData.set_compression(FileSingleHeader_Compression_UNCOMPRESSED); /* If no magic applies it can only be uncompressed */

      return MetaData.compression();
   }


   /** Write named meta file from params */
   template<class C>
   void FileSingle<C>::WriteMeta(ParamContainer &p)
   {
      /* Write meta file. In a sequence only for the first file. */
      if (!_drop_meta || _counter != _counter_last) {

         if (!p.WriteToFile(_name + METAFILE_SUFFIX))
            EIOError("Can not write meta file");
      }
   }

   /**
    * Write meta data to stream.
    */
   template<class C>
   void FileSingle<C>::WriteMetaToStream(C* data, size_t payload_size, bool more_files_follow, bool ignore_data)
   {
      string s;

      MetaData.set_class_(data->Name());

      BuildDataFileName(s);
      MetaData.set_name(s);

      ParamContainer pm = data->Params();
      pm.Write(s);
      MetaData.set_meta_data(s);
      MetaData.set_more_files_follow(more_files_follow);
//      MetaData.set_s  set_sequence_number(_counter); /* !? */

      if (ignore_data)
         MetaData.set_payload_size(0);
      else
         MetaData.set_payload_size(payload_size);

      switch (_compMethod){
         case BZIP:
         case ZLIB:
            MetaData.set_compression(FileSingleHeader_Compression_ZLIB);
            break;
         case UNCOMPRESSED:
            MetaData.set_compression(FileSingleHeader_Compression_UNCOMPRESSED);
            break;
         case INVALID:
            throw (EIOError("Can't write meta data to stream since compression is INVALID"));
      }

      uint32_t size = MetaData.ByteSize();
      /* Put message length in front */
      _sout->write(reinterpret_cast<char*>(&size), sizeof(uint32_t));

      if(!MetaData.SerializeToOstream(_sout))
         throw(EIOError("Can't write meta data to stream"));
   }

   /**
    * Prepare content data for writing.
    */
   template<class C>
   void FileSingle<C>::PrepareData(C *data)
   {
      Memory& mem = Memory::Instance();

      switch (_compMethod) {
         case UNCOMPRESSED: /* just copy to buffer */

            if (_buffer != NULL) mem.Free(_buffer);

            mem.Align((void**) &_buffer, data->sizeBytes());
            mem.Copy(_buffer, (char*) data->begin(0), data->sizeBytes());
            _buffer_size = data->sizeBytes();
            break;
         case ZLIB:
            _CompressZLIB(data);
            break;
         case BZIP:
            _CompressBZIP(data);
            break;
         case INVALID:
            throw (EIOError("Can't prepare meta data for stream since compression is INVALID"));

      }
   }

   /**
    * Prepare content data for writing.
    */
   template<class C>
   void FileSingle<C>::PrepareStreamPayload(C *data, stringstream& buffer)
   {
      switch (_compMethod) {
         case UNCOMPRESSED: /* just copy to buffer */
            data->Serialize(buffer);
            break;
         case BZIP:     // There's no BZIP compression for streams, just redirect to ZLIB
         case ZLIB:
            {
               // get raw data
               stringstream outbuf;
               data->Serialize(outbuf);
               outbuf.seekg(0);

               OstreamOutputStream osbuf(&buffer);
               GzipOutputStream gzip(&osbuf);

               int size=0;
               void* buf;
               size_t written=0;
               size_t len=0;

               // Compress
               while (written < outbuf.str().size() ){
                  if(!gzip.Next(&buf, &size))
                     throw(EIOError("GzipOutputStream failed"));

                  len = size;
                  if ((size_t) size > outbuf.str().size() - written) len = outbuf.str().size() - written;
                  outbuf.read(reinterpret_cast<char*>(buf), len);
                  written += len;
               }

               if (((size_t) size - len) > 0) gzip.BackUp(size - len);


               if (!gzip.Close()){
                  throw (EIOError("Error compressing stream", gzip.ZlibErrorMessage()));
               }
            }
            break;
         case INVALID:
            throw (EIOError("Can't prepare meta data for stream since compression is INVALID"));

      }


   }

   /**
    * Write header data to stream.
    */
   template<class C>
   void FileSingle<C>::WriteIntroToStream()
   {
      /* Create file if needed */
      if (_sout == NULL){
         ofstream* out = new ofstream(); // TODO: Fix this. don't go via an ofstream
         _our_out_stream = true;

         string name;
         BuildDataFileName(name);
         out->open(name.c_str(), ios_base::binary);

         if (! out->is_open() ){
            FinalizeOutStream();
            throw(EIOError("Can not open binary stream for writing", name));
         }
         _sout = out;
         _our_out_stream = true;
      }

      if (!_intro_written){
         /* Write intro */
         PbufHeader intro;

         _sout->write(reinterpret_cast<char*> (&intro), sizeof(intro));
         _intro_written = true;
      }
   }


   template<class C>
   void FileSingle<C>::WriteDataToStream(const string &data_buf)
   {
      _sout->write(reinterpret_cast<const char*>(data_buf.data()), data_buf.size());
   }

   /**
    * Cleanup and close the stream.
    */
   template<class C>
   void FileSingle<C>::FinalizeOutStream()
   {
      if (_our_out_stream){
         dynamic_cast<ofstream*>(_sout)->close();
         delete _sout;
         _sout = NULL;
         _our_out_stream = false;
      }

      _intro_written = false;
   }

   /**
    * Write a single file to stream.
    */
   template<class C>
   void FileSingle<C>::WriteSingleFileToStream(C *data)
   {
      WriteIntroToStream();     // Header

      // Get raw data (payload) from class (subheader + data), compress if requested
      stringstream payload;
      PrepareStreamPayload(data, payload);

      // put payload size in header and write header to stream
      WriteMetaToStream(data, payload.str().size());

      // write payload to stream
      const string& s = payload.str();
      WriteDataToStream( s );
      FinalizeOutStream();      // close stream
      if (_sequence) _counter += _increment;
   }


   template<class C>
   void FileSingle<C>::OpenINStream()
   {
      if (_sin == NULL){ /* Open file */
         ifstream* in = new ifstream();


         string name;
         BuildDataFileName(name);

         in->open(name.c_str(), ios_base::binary);

         if (! in->is_open() ){
            throw(EIOError("Can not open binary stream file for reading", name));
         }

         _sin = in;
         _our_in_stream = true;
         _intro_read = false;
      }

   }

   /**
    * Check for a correct header.
    */
   template<class C>
   void FileSingle<C>::CheckIntroFromStream()
   {
      OpenINStream();

      if (_intro_read) return;

      /* Check for correct header */
      PbufHeader intro;

      _sin->read(reinterpret_cast<char*>(&intro), sizeof(PbufHeader));

      if (string(intro.magic) != PBUF_MAGIC)
         throw(EIOError("Malformated stream. No stream magic found."));

      if ((unsigned int) intro.version != PBUF_VERSION)
         throw(EIOError("Wrong stream version"));

      _intro_read = true;
   }

   /**
    * Read the Metadata from stream.
    */
   template<class C>
   const FileSingleHeaderPm& FileSingle<C>::ReadMetaFromStream()
   {
      uint32_t mlen=0;

      /* message/meta data length */
      _sin->read(reinterpret_cast<char*>(&mlen), sizeof(uint32_t));

      char* buf = new char[mlen];
      _sin->read(reinterpret_cast<char*>(buf), mlen);

      // Parse header
      MetaData.Clear();
      if (!MetaData.ParseFromArray(reinterpret_cast<const void*>(buf), mlen)){
         delete[] buf;
         throw(EIOError("Error reading meta data from stream"));
      }

      delete[] buf;

      return MetaData;
   }

   /**
    * Init data storage in data to prepare for reading.
    */
   template<class C>
   void FileSingle<C>::InitDataStorage(C *data)
   {
      ParamContainer p = MetaData.params();
      data->Init(p);
   }

   /**
    * Autodetect which format should be read.
    *
    * This only valid if no file is open.
    */
   template<class C>
   void FileSingle<C>::DetectFormat()
   {
      if (!autodetect) return;

      if (_sin != NULL) { /* having an valid input ...*/
         _type = stream;
         return;
      }

      OpenINStream();

      try {
         CheckIntroFromStream();
         _type = stream;
      } catch (EIOError& e) { /* to fail means it must be _something else_ */
         if (_our_in_stream) {
            dynamic_cast<ifstream*>(_sin)->close();
            delete _sin;
            _sin = NULL;
         }

         _intro_read = false;
         _our_in_stream = false;

         _type = binary;
      }

   }

   /**
    * Read binary data from stream.
    */
   template<class C>
   void FileSingle<C>::ReadDataFromStream(C *data)
   {


      if (MetaData.payload_size() > 0){

         switch(MetaData.compression())
         {
            case FileSingleHeader_Compression_UNCOMPRESSED:
               data->DeSerialize(*_sin);
               break;
            case FileSingleHeader_Compression_BZIP:
            case FileSingleHeader_Compression_ZLIB:
               char* inbuf = new char[MetaData.payload_size()];

               _sin->read(inbuf, MetaData.payload_size());

               if ( (size_t) _sin->gcount() != MetaData.payload_size() )
                  throw(EIOError("Reading from stream failed"));

               ArrayInputStream arrin(inbuf, MetaData.payload_size());
               GzipInputStream gzip(&arrin);
               stringstream outbuf;

               const void* gzbuf;
               int size;

               while (gzip.Next(&gzbuf, &size)){
                  outbuf.write(reinterpret_cast<const char*>(gzbuf), size);
               }

               outbuf.seekg(0);
               data->DeSerialize(outbuf);

               delete[] inbuf;
               break;
         }
      }

      if (! MetaData.more_files_follow()) {
         _intro_read = false;

         /* Close */
         if (_our_in_stream ){
            dynamic_cast<ifstream*>(_sin)->close(); // We know here that it must be an ifstream.
            delete _sin;
            _sin = NULL;
            _our_in_stream = false;
         }
      }
   }


   /**
    * Read a complete file from stream or a protobuf file.
    */
   template<class C>
   void FileSingle<C>::ReadSingleFileFromStream(C *data)
   {
      CheckIntroFromStream();
      ReadMetaFromStream();
      ReadDataFromStream(data);

      if (_sequence) _counter += _increment;
   }


   /**
    * Read meta data from a key-value file.
    */
   template<class C>
   void FileSingle<C>::ReadMetaFromRaw()
   {
      string name;
      BuildMetaFileName(name);

      ParamContainer pm;
      if (!pm.ReadFromFile(name)){
         // Retry stripping by stripping a sequence index
         if (name.rfind('_') != string::npos) {
            name = name.substr(0,name.rfind('_')) + METAFILE_SUFFIX;

            if ( !pm.ReadFromFile(name) )
               throw( EIOError("Can not read meta file", name + " or " + _name) );
         } else {
            throw( EIOError("Can not read meta file", name + " or " + _name) );
         }
      }

      MetaData.set_params(pm);
   }

   /**
    * Read data from raw binary file.
    *
    * Array must be initialized and meta data must be present.
    */
   template<class C>
   void FileSingle<C>::ReadDataFromRaw(C* data)
   {
      string s;
      ifstream file;

      /* We need some parameters for reading */
      if (data->sizeBytes() <= 0)
         throw(EParamProblem("Wrong size"));

      BuildDataFileName(s);

      file.open(s.c_str(), ios::binary);
      if (file.fail()) {
         throw(EIOError("Can not open binary file for reading", s));
      }

      if (MetaData.compression() != FileSingleHeader_Compression_UNCOMPRESSED){
         file.seekg(0, ios::end);
         size_t size = file.tellg();
         MetaData.set_payload_size(size);
         file.seekg(0);
      }


      switch(MetaData.compression()) {
         case FileSingleHeader_Compression_UNCOMPRESSED:
            file.read((char*) data->begin(0), data->sizeBytes());

            if ((size_t) file.gcount() != data->sizeBytes())
               throw(EIOError("Can not read binary file", s));

            break;
         case FileSingleHeader_Compression_ZLIB:
            _DecompressZLIB(&file, MetaData.payload_size(), data);
            break;
         case FileSingleHeader_Compression_BZIP:
            _DecompressBZIP(&file, MetaData.payload_size(), data);
            break;
         default:
            break;
      }

      file.close();
   }

   /**
    * Read data from raw file with meta data in extra .meta file.
    */
   template<class C>
   void FileSingle<C>::ReadSingleFileFromRaw(C *data)
   {
      ReadMetaFromRaw();
      InitDataStorage(data);
      DetectCompressionInPayload();
      ReadDataFromRaw(data);

      if (_sequence) _counter += _increment;
   }

   /**
    * Write meta data into .meta file.
    */
   template<class C>
   void FileSingle<C>::WriteMetaToRaw(C *data)
   {
      ParamContainer pm = data->Params();
      MetaData.set_params(pm);
      MetaData.set_class_(data->Name());

      pm.SetValue("CLASS", data->Name());
      pm.WriteToFile(_name + METAFILE_SUFFIX);
   }

   /**
    * Write data to raw binary file.
    */
   template<class C>
   void FileSingle<C>::WriteDataToRaw(C *data)
   {
      ofstream file;
      string name;

      BuildDataFileName(name);

      /* Write binary data */
      file.open(name.c_str(), ios_base::binary);
      if (!file.is_open())
         throw(EIOError("Can not open binary file for writing", name));

      file.write((char*) _buffer, _buffer_size);
      if (file.bad())
         throw(EIOError("Can not write binary file", name));

      Memory::Instance().Free(_buffer);
      _buffer = NULL;
      _buffer_size  = 0;

      if (file.bad())
         throw(EIOError("Can not write binary file", name));

      string magic;
      switch(_compMethod){
         case ZLIB:
            /* Append magic bytes */
            magic = ZLIB_MAGIC;
            file.write((char*) magic.c_str(), magic.size()+1);
            if (file.bad())
               throw(EIOError("Can not write binary file", name));

            break;
         case BZIP:
            /* Append magic bytes */
            magic = BZIP_MAGIC;
            file.write((char*) magic.c_str(), magic.size()+1);
            if (file.bad())
               throw(EIOError("Can not write binary file", name));

            break;
         default:
            break;
      }

      file.close();
   }

   /**
    * Write data into raw binary file + extra .meta file.
    */
   template<class C>
   void FileSingle<C>::WriteSingleFileToRaw(C *data)
   {
      WriteMetaToRaw(data);
      PrepareData(data);
      WriteDataToRaw(data);

      if (_sequence) _counter += _increment;
   }

   /**
    * Create a filename for a sperate meta file.
    *
    * => name.meta
    */
   template<class C>
   void FileSingle<C>:: BuildMetaFileName(string &name)
   {
      name = _name + METAFILE_SUFFIX;
   }

   /**
    * Create a file name for the data container file.
    *
    * \li name.suffix
    * \li name_counter.suffix (if sequence is activated)
    */
   template<class C>
   void FileSingle<C>:: BuildDataFileName(string &name)
   {
      /* Build name */
      if (_sequence) {
         stringstream ss;
         ss << _counter;
         name = _name + "_" + ss.str() + _suffix;
      } else {
         name = _name + _suffix;
      }

   }


   /**
    * Read meta data.
    *
    * The source depends on the given format.
    */
   template<class C>
   const FileSingleHeaderPm& FileSingle<C>::ReadMeta()
   {
      if (autodetect) DetectFormat();

      switch (_type){
         case stream:
               CheckIntroFromStream();
               ReadMetaFromStream();
            break;
         case binary:
               ReadMetaFromRaw();
               DetectCompressionInPayload();
            break;
         default:
            throw (EIOError("Failed to get meta data (format not determined)."));
            break;
      }

      return MetaData;
   }

   /**
    * Read in the data.
    *
    * This must be preceded by a ReadMeta call.
    */
   template<class C>
   void FileSingle<C>::ReadData(C* data)
   {
      switch (_type){
         case stream:
            ReadDataFromStream(data);
            break;
         case binary:
            InitDataStorage(data);
            ReadDataFromRaw(data);
            break;
         default:
            throw (EIOError("Failed to read data file (format not determined)."));
            break;
      }

      if (_sequence)_counter += _increment;
   }


   template<class C>
   void FileSingle<C>::WriteMeta(C* data, bool more_files_follow, bool ingore_data)
   {
      if (autodetect) _type = stream;

      switch (_type){
         case stream:
         {
            WriteIntroToStream();

            stringstream payload;
            PrepareStreamPayload(data, payload);

            // put payload size in header and write header to stream
            WriteMetaToStream(data, payload.str().size(), more_files_follow, ingore_data);

            // write payload to stream
            WriteDataToStream(payload.str());

            if (!MetaData.more_files_follow()) FinalizeOutStream();
            break;
         }
         case binary:
            if (!ingore_data) PrepareData(data);
            WriteMetaToRaw(data);
            break;
         default:
            throw (EIOError("Failed to get meta data (format not determined)."));
            break;
      }
   }

   template<class C>
   void FileSingle<C>::WriteData(C* data)
   {
         switch (_type){
            case stream:
               {
                  // Data is written in WriteMeta(),
                  // since meta data and payload can't be separated in a stream

                  if (!MetaData.more_files_follow()){
                     FinalizeOutStream();
                  }
               }
               break;
            case binary:
               break;
            default:
               throw (EIOError("Failed to write data file (format not determined)."));
               break;
         }

         if (_sequence) _counter += _increment;
   }

   /**
    * File writer method.
    * 
    * \param data Data to save.
    * 
    */
   template<class C>
   void FileSingle<C>::WriteFile(C *data)
   {

      if (data->strides() > 1)
         throw(EIncompatible("Strided data handling not implemented"));

      switch (_type) {
         case binary:
            if (data->strides() == 1)
               WriteSingleFileToRaw(data);
            break;
         case autodetect:
         case stream:
            WriteSingleFileToStream(data);
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
   template<class C>
   void FileSingle<C>::ReadFile(C *data)
   {
      if (data->strides() > 1)
         throw(EIncompatible("Strided data handling not implemented"));

      DetectFormat();

      switch (_type) {
         case autodetect:
         case binary:
            ReadSingleFileFromRaw(data);
            break;
         case stream:
            ReadSingleFileFromStream(data);
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
   template<class C>
   void FileSingle<C>::operator<<(C *out)
   {
      WriteFile(out);
   }

} /* namespace QDLIB */

#endif /* #ifndef FILESINGLE_H */

