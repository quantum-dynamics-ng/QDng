#include "FileWF.h"
#include "WFMultistate.h"
#include "modules/ModuleLoader.h"
#include "tools/GlobalParams.h"
#include "tools/Logger.h"
#include "tools/helpers.h"
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

namespace QDLIB {

   FileWF::FileWF() : _compress(FILEWF_COMPRESSION), _compLevel(1), _compTolerance(FILEWF_COMPRESSION_TOLERANCE),
                      _compMethod(ZLIB), _buf(NULL)
   {
      _suffix = BINARY_WF_SUFFIX;

#if defined(HAVE_LIBZ)  || defined (HAVE_LIBBZ2)
      /* Compression options from global params */
      ParamContainer& gp = GlobalParams::Instance();
      gp.GetValue("compress", _compress, FILEWF_COMPRESSION);
      
      if ( _compress ) {
         if ( gp.isPresent("complevel") )
            gp.GetValue("complevel", _compLevel);

         if ( gp.isPresent("comptol") )
            gp.GetValue("comptol", _compTolerance);

         if ( gp.isPresent("compmethod") ) {
            string s;
            gp.GetValue("compmethod", s);
#ifdef HAVE_LIBZ
            if (s == "ZLIB")
               _compMethod = ZLIB;
#endif
#ifdef HAVE_LIBBZ2
            if (s == "BZIP")
               _compMethod = BZIP;
#endif
            else
               _compMethod = INVALID;
         }
      }
#endif
   }
   
   
   FileWF::~FileWF()
   {
      if (_buf != NULL) delete _buf;
   }

   /**
    * Check the compression buffer for right initalization.
    */
   void FileWF::CheckBuf(WaveFunction * data)
   {
      if (_buf == NULL) _buf = new cVec();
      
      _buf->newsize(data->size(), data->strides());
   }

   /** 
    * Read a single WF from file and handle compression.
    */
   void FileWF::_ReadFromFile(WaveFunction * data)
   {
      FileSingle<WaveFunction>::ReadFile(data); /* Read the raw data set */
      
      /* Check if file is compressed */
      string zlibmagic(ZLIB_MAGIC);
      string bzipmagic(BZIP_MAGIC);
      char lastbytes[ZLIB_MLEN];
      
      /* Cut out the magic bytes for clean string comparison */
      if (FileSize() > ZLIB_MLEN) {
	 char *c_magic = reinterpret_cast<char*>(data->begin(0));
	 strncpy(lastbytes, &(c_magic[FileSize()-ZLIB_MLEN]), ZLIB_MLEN);
      } else
	 lastbytes[0] = '\0';
      
      bool runcomp = false;

#ifdef HAVE_LIBZ
      if (zlibmagic == lastbytes){
         _DecompressZLIB(data);
         runcomp = true;
      }
#else
      if (zlibmagic == lastbytes)
         throw (EIncompatible ("Reading of zlib compressed wave functions is not supported. Please recompile with zlib support") );
#endif

#ifdef HAVE_LIBBZ2
      if (bzipmagic == lastbytes){
         _DecompressBZIP(data);
         runcomp = true;
      }
#else
      if (bzipmagic == lastbytes)
         throw (EIncompatible ("Reading of bzip compressed wave functions is not supported. Please recompile with bzip2 support") );
#endif
      if (FileSize() < data->sizeBytes()&& !runcomp)  /* Check if size OK if using uncompressed WF*/
         throw(EIOError("WaveFunction truncated!"));
   }
   
#ifdef HAVE_LIBZ
   /**
    * Compress data with ZLIB
    */
   void FileWF::_CompressZLIB(WaveFunction *data)
   {
      data->Reduce(_compTolerance); /* Data reduction */

      bool cfail = true;
      size_t fsize = 0;

      /* zlib compression */
      if (_compLevel > 0)
      {
         /* zlib-compression */
         z_stream strm;
         dcomplex *in, *out;

         strm.zalloc = Z_NULL;
         strm.zfree = Z_NULL;
         strm.opaque = Z_NULL;
         if (deflateInit(&strm, _compLevel) != Z_OK) /* Init stream */
            throw(EIOError("z-lib init failed"));

         CheckBuf( data);

         cfail = false;
         out = _buf->begin(0);
         in = data->begin(0);
         /* input */
         strm.next_in = (Bytef*) in;
         strm.avail_in = data->sizeBytes();
         /* output buffer */
         strm.next_out = (Bytef*) out;
         strm.avail_out = data->sizeBytes();

         /* hack to handle bad compression which is larger than input - just ignore compressed data */
         if (deflate(&strm, Z_FINISH) != Z_STREAM_END)
         { /* Compression */
            cfail = true;
         }

         deflateEnd(&strm);

         fsize = data->sizeBytes() - strm.avail_out;
         FileSize(fsize + 8); /* data + magic */
         if (strm.avail_out < ZLIB_MLEN)
            cfail = true; /* compressd size + magic must be smaller than original size */
      }

      if (!cfail)
      {
         ParamContainer& p = data->Params(); /* indicate compression in meta data */
         p.SetValue("compress", true);
         p.SetValue("compressLevel", _compLevel);
         p.SetValue("compressTol", _compTolerance);
         p.SetValue("compressMethod", "ZLIB");
         data->swap(*_buf);
         /* Append magic bytes */
         string magic(ZLIB_MAGIC);
         char *c_magic = reinterpret_cast<char*> (data->begin(0));

         magic.copy(&(c_magic[fsize]), ZLIB_MLEN - 1);
         c_magic[fsize + ZLIB_MLEN - 1] = '\0';
      }
      else data->IsKspace(false); /* Do nothing if compression had failed */

      /* Write and switch back to real space */
      FileSingle<WaveFunction>::WriteFile( data);

      if (!cfail)
      {
         data->swap(*_buf);
         data->IsKspace(false);
      }

      FileSize(0);
   }

   /**
    * Decompress data with zlib.
    */
   void FileWF::_DecompressZLIB(WaveFunction * data)
   {
      z_stream strm;
      dcomplex *in, *out;

      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;

      if (inflateInit(&strm) != Z_OK) /* Init stream */
         throw(EIOError("z-lib init failed"));

      in = data->begin(0); /* compressed input is in real space */
      out = data->GetSpaceBuffer()->begin(0); /* decompress into k-space buffer */
      /* input */
      strm.next_in = (Bytef*) in;
      strm.avail_in = FileSize() - ZLIB_MLEN; /* Buffer - magic bytes */
      /* output buffer */
      strm.next_out = (Bytef*) out;
      strm.avail_out = data->sizeBytes();

      if (inflate(&strm, Z_FINISH) != Z_STREAM_END)
      { /* Compression */
         throw(EIOError("zlib decompression failed"));
      }

      inflateEnd(&strm);

      data->Restore();

   }

#endif

#ifdef HAVE_LIBBZ2

   void FileWF::_CompressBZIP(WaveFunction * data)
   {
      data->Reduce(_compTolerance); /* Data reduction */

      bool cfail = true;
      size_t fsize = 0;

      /* bzip2 compression */
      if (_compLevel > 0) {
         bz_stream strm;
         dcomplex *in, *out;

         /* Init bzip2 stream */
         strm.bzalloc = NULL;
         strm.bzfree = NULL;
         strm.opaque = NULL;

         if (BZ2_bzCompressInit(&strm, _compLevel, 0, 0) != BZ_OK) /* Init stream no verbose, workfactor set to default */
            throw(EIOError("z-lib init failed"));

         CheckBuf(data); /* Check if WF-buffer is intialized */

         cfail = false;
         out = _buf->begin(0);
         in = data->begin(0);

         strm.next_in = (char*) in;
         strm.avail_in = data->sizeBytes();
         /* output buffer */
         strm.next_out = (char*) out;
         strm.avail_out = data->sizeBytes();

         /* hack to handle bad compression which is larger than input or invalid - just ignore compressed data */
         if (BZ2_bzCompress(&strm, BZ_FINISH) != BZ_STREAM_END)
         { /* Compression */
            cfail = true;
         }

         if (BZ2_bzCompressEnd(&strm) != BZ_OK) cfail = true;

         fsize = data->sizeBytes() - strm.avail_out;
         FileSize(fsize + 8); /* data + magic */

         if (strm.avail_out < ZLIB_MLEN)
            cfail = true; /* compressed size + magic must be smaller than original size */
      }

      if (!cfail)
      {
         ParamContainer& p = data->Params(); /* indicate compression in meta data */
         p.SetValue("compress", true);
         p.SetValue("compressLevel", _compLevel);
         p.SetValue("compressTol", _compTolerance);
         p.SetValue("compressMethod", "BZIP");
         data->swap(*_buf);

         /* Append magic bytes */
         string magic(BZIP_MAGIC);
         char *c_magic = reinterpret_cast<char*> (data->begin(0));

         magic.copy(&(c_magic[fsize]), ZLIB_MLEN - 1);
         c_magic[fsize + ZLIB_MLEN - 1] = '\0';
      }
      else {
         Logger& log = Logger::InstanceRef();
         log.coutdbg() << "bzip2 compression failed!" << endl;
         data->IsKspace(false); /* Do nothing if compression had failed */
      }

      /* Write and switch back to real space */
      FileSingle<WaveFunction>::WriteFile( data);

      if (!cfail)
      {
         data->swap(*_buf);
         data->IsKspace(false);
      }

      FileSize(0);

   }

   /**
    * Decompress data with bzip2.
    */
   void FileWF::_DecompressBZIP(WaveFunction * data)
   {
      bz_stream strm;
      dcomplex *in, *out;

      /* Init bz stream */
      strm.bzalloc = NULL;
      strm.bzfree = NULL;
      strm.opaque = NULL;

      if ( BZ2_bzDecompressInit ( &strm, 0, 0) != BZ_OK) { /* Init with no verbose, no alternate algo */
         throw(EIOError("Error while intializing bzDecompress"));
      }

      in = data->begin(0); /* compressed input is in real space */
      out = data->GetSpaceBuffer()->begin(0); /* decompress into k-space buffer */

      strm.next_in = (char*) in;
      strm.avail_in = FileSize() - ZLIB_MLEN; /* Buffer - magic bytes */
      /* output buffer */
      strm.next_out = (char*) out;
      strm.avail_out = data->sizeBytes();

      if (BZ2_bzDecompress(&strm) != BZ_STREAM_END)
         throw(EIOError("bz2 decompression failed"));

      BZ2_bzDecompressEnd(&strm);

      data->Restore();
   }
#endif

   /**
    * Create WF Instance based on meta file information.
    * 
    * Reads also the full data from files.
    * This method works recursive for Multistate WFs.
    */
   WaveFunction * FileWF::LoadWaveFunctionByMeta()
   {
      string basename = _name;
      string classname;
      WaveFunction *wf;
      ParamContainer p;
      ModuleLoader* mods = ModuleLoader::Instance();
      
      ReadMeta(p);
      p.GetValue("CLASS", classname);
      
      if (classname == "WFMultistate"){ /* Excplicit Multistate handling */
         WFMultistate *wfm = new WFMultistate();
         /** \todo register WF with collector? */
         
         int states;
         p.GetValue("states", states); /* Grab the states from params. wfm->Init() must be executed after wfm->Add() ! */
         if (states < 1)
            throw ( EParamProblem("Init from meta file: Invalid number of states", wfm->States()) );
         
         int counter = Counter();
#ifdef HAVE_MPI
         int rank =  MPI::COMM_WORLD.Get_rank();
         int msize = MPI::COMM_WORLD.Get_size();
#else
         int rank = 0; int msize = 1;
#endif
         for (int i=rank; i < states; i +=msize ){/* Loop over states */
            Counter(counter);    /* Every ReadFile() increases counter => restore it when using it diff. states */
            stringstream ss;
            ss << "-" << i;
            _name = basename + ss.str();
            wfm->Add( LoadWaveFunctionByMeta(), i ); /* Load module */
         }
         wfm->Init(p); 
         wf = wfm;
         //cout << "rank : " << rank << endl << *wf<< endl;
         wf->SyncStrides();
      } else { /* handling for Single state WFs */
         wf = mods->LoadWF(classname);
         _ReadFromFile(wf); /* Load data */
      }
      
      _name = basename; /* We modfied the name => switch back to original */
      return wf;
   }

   /**
    * Read WF from File.
    * 
    * This method can handle multistate WFs
    * 
    * \param data WF instance of correct type
    * 
    */
   void FileWF::ReadFile(WaveFunction * data)
   {
      WFMultistate *wfm;
      
      wfm = dynamic_cast<WFMultistate*>(data);
      if (wfm != NULL){ /* Load Multistate set */
         string basename = _name;
         
         if (wfm->States() < 1)
            throw ( EParamProblem("Reading WaveFunction: Invalid number of states", wfm->States()) );
         
         int counter = Counter();
         for (int i=wfm->MPIrank(); i < wfm->States(); i += wfm->MPIsize()){ /* Loop over States */
            Counter(counter);    /* Every ReadFile() increases counter => restore it when using it diff. states */
            stringstream ss;
            ss << "-" << i;
            _name = basename + ss.str();
            ReadFile(wfm->State(i)); /* Load data */
         }
         _name = basename; /* We modfied the name => switch back to original */
         
         ParamContainer p;
         ReadMeta(p);
         
         wfm->Init(p);
         wfm->SyncStrides();
      } else { /* Load Single WaveFunction */
         _ReadFromFile(data);
      }
     
   }


   /**
    * Write WaveFunction to file.
    * 
    * This method can write multistate WFs.
    */
   void FileWF::WriteFile(WaveFunction * data)
   {
      WFMultistate *wfm;
      
      wfm = dynamic_cast<WFMultistate*>(data);
      if (wfm != NULL){ /* Write Multistate set */
         if (wfm->States() < 1)
            throw ( EParamProblem("Writing WaveFunction: Invalid number of states", wfm->States()) );
        
         string basename = _name;
         int counter = Counter();
         for (int i=wfm->MPIrank(); i < wfm->States(); i+=wfm->MPIsize()){
            Counter(counter);    /* Every WriteFile() increases counter => restore it when using it diff. states */
            stringstream ss;
            ss << "-" << i;
            _name = basename + ss.str();
            WriteFile(wfm->State(i));
         }
         _name = basename; /* We modfied the name => switch back to original */
         /* Write meta file for Multistate container */
         if (wfm->MPIrank() == 0){
            ParamContainer p;
            p = wfm->Params();
            p.SetValue("CLASS", wfm->Name() );
            WriteMeta(p);
         }
      } else { /* Write single file */
#if defined(HAVE_LIBZ) || defined(HAVE_LIBBZ2)
         /* Compression */
         if (_compress) {
#ifdef HAVE_LIBZ
            if (_compMethod == ZLIB)
               _CompressZLIB(data);
#endif
#ifdef HAVE_LIBBZ2
            if (_compMethod == BZIP)
               _CompressBZIP(data);
#endif
            if (_compMethod == INVALID)
               throw( EParamProblem("Invalid file compression method given") );
         } else /* w/o compression */
#endif
            FileSingle<WaveFunction>::WriteFile(data);
      }
   }
   
   /**
    * Streamoperator for file reading.
    * 
    * \see ReadFile
    */
   void operator>>(FileWF &file,  WaveFunction *data)
   {
      if (data == NULL)
         throw EIncompatible("Can't WF read into NULL pointer");
	       
      file.ReadFile(data);
   }

   /**
    * Stream operator for reading with a not instanciated WaveFunction.
    * 
    * \see LoadWaveFunctionByMeta
    */
   void operator >>(FileWF & file, WaveFunction ** data)
   {
      *data = file.LoadWaveFunctionByMeta();
   }

}










