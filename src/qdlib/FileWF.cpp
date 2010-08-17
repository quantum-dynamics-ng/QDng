#include "FileWF.h"
#include "WFMultistate.h"
#include "modules/ModuleLoader.h"
#include "tools/GlobalParams.h"
#include "config.h"

#ifdef HAVE_LIBZ
  #include "zlib.h"
#endif

#define ZLIB_MAGIC "ZLIBCOM"
/** Length of the magic + terminating zero */
#define ZLIB_MLEN 8 

namespace QDLIB {

   FileWF::FileWF() : _compress(FILEWF_COMPRESSION), _compLevel(1), _compTolerance(FILEWF_COMPRESSION_TOLERANCE),
                  _buf(NULL)
   {
      _suffix = BINARY_WF_SUFFIX;
      
#ifdef HAVE_LIBZ
      /* Compression options from global params */
      ParamContainer& gp = GlobalParams::Instance();
      gp.GetValue("compress", _compress, FILEWF_COMPRESSION);
      
      if ( _compress ) {
         if ( gp.isPresent("complevel") )
            gp.GetValue("complevel", _compLevel);
         if ( gp.isPresent("comptol") )
            gp.GetValue("comptol", _compTolerance);
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
   void QDLIB::FileWF::_ReadFromFile(WaveFunction * data)
   {
      FileSingle<WaveFunction>::ReadFile(data);
      
      /* Check if file is compressed */
      string magic(ZLIB_MAGIC);
      char lastbytes[ZLIB_MLEN];
      
      /* Cut out the magic bytes for clean string comparison */
      if (FileSize() > ZLIB_MLEN) {
	 char *c_magic = reinterpret_cast<char*>(data->begin(0));
	 strncpy(lastbytes, &(c_magic[FileSize()-ZLIB_MLEN]), ZLIB_MLEN);
      } else
	 lastbytes[0] = '\0';
      
#ifdef HAVE_LIBZ
      if ( magic == lastbytes ) { /* Decompress */
         z_stream strm;
         dcomplex *in, *out;
            
         strm.zalloc = Z_NULL;
         strm.zfree = Z_NULL;
         strm.opaque = Z_NULL;
         
         if ( inflateInit(&strm) != Z_OK ) /* Init stream */
            throw (EIOError ("z-lib init failed") );
               
         in = data->begin(0);                     /* compressed input is in real space */
         out = data->GetSpaceBuffer()->begin(0);  /* decompress into k-space buffer */
         /* input */
         strm.next_in = (Bytef*) in;
	 strm.avail_in = FileSize()-ZLIB_MLEN;             /* Buffer - magic bytes */
         /* output buffer */
         strm.next_out = (Bytef*) out;
         strm.avail_out = data->sizeBytes();
         
         if ( inflate(&strm, Z_FINISH) != Z_STREAM_END ) { /* Compression */
            throw (EIOError ("decompression failed") );
         }
         
         inflateEnd(&strm);

         data->Restore();
      } else {
         if ( FileSize() < data->sizeBytes() )
            throw ( EIOError("WaveFunction truncated!") );
      }
#else
      if (magic == &(c_magic[FileSize()-ZLIB_MLEN]))
         throw (EIncompatible ("Reading of compressed wave functions is not supported. Please recompile with zlib support") );
      
      if ( FileSize() < data->SizeBytes() )
         throw ( EIOError("WaveFunction truncated!") );
#endif
   }
   
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
         
         int states;
         p.GetValue("states", states); /* Grab the states from params. wfm->Init() must be executed after wfm->Add() ! */
         if (states < 1)
            throw ( EParamProblem("Init from meta file: Invalid number of states", wfm->States()) );
         
         int counter = Counter();
         for (int i=0; i < states; i++){ /* Loop over States */
            Counter(counter);    /* Every ReadFile() increases counter => restore it when using it diff. states */
            stringstream ss;
            ss << "-" << i;
            _name = basename + ss.str();
            wfm->Add( LoadWaveFunctionByMeta(), i ); /* Load module */
         }
         wfm->Init(p); 
         wf = wfm;
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
         for (int i=0; i < wfm->States(); i++){ /* Loop over States */
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
         for (int i=0; i < wfm->States(); i++){
            Counter(counter);    /* Every WriteFile() increases counter => restore it when using it diff. states */
            stringstream ss;
            ss << "-" << i;
            _name = basename + ss.str();
            WriteFile(wfm->State(i));
         }
         _name = basename; /* We modfied the name => switch back to original */
         /* Write meta file for Multistate container */
         ParamContainer p;
         p = wfm->Params();
         p.SetValue("CLASS", wfm->Name() );
         WriteMeta(p);
      } else { /* Write single file */
#ifdef HAVE_LIBZ
         /* Compression */
         if (_compress) {
            data->Reduce(_compTolerance); /* Data reduction */

            bool cfail = true;
            size_t fsize=0;
            
            /* zlib compression */
            if (_compLevel > 0) {
               /* zlib-compression */
               z_stream strm;
               dcomplex *in, *out;
            
               strm.zalloc = Z_NULL;
               strm.zfree = Z_NULL;
               strm.opaque = Z_NULL;
               if ( deflateInit(&strm, _compLevel) != Z_OK ) /* Init stream */
                  throw (EIOError ("z-lib init failed") );
               
               CheckBuf(data);
               
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
               if ( deflate(&strm, Z_FINISH) != Z_STREAM_END ) { /* Compression */
                  cfail = true;
               }
               
               deflateEnd(&strm);
               
               fsize = data->sizeBytes()-strm.avail_out;
               FileSize(fsize+8); /* data + magic */
	       if (strm.avail_out < ZLIB_MLEN ) cfail = true; /* compressd size + magic must be smaller than original size */
            }
            
            
            if (!cfail){
               ParamContainer& p = data->Params();  /* indicate compression in meta data */
               p.SetValue("compress", true);
               p.SetValue("compressLevel", _compLevel);
               p.SetValue("compressTol", _compTolerance);
               data->swap(*_buf);
               /* Append magic bytes */
               string magic(ZLIB_MAGIC);
               char *c_magic = reinterpret_cast<char*>(data->begin(0));
               
	       magic.copy( &(c_magic[fsize]), ZLIB_MLEN-1);
	       c_magic[fsize+ZLIB_MLEN-1] = '\0';
            } else 
               data->IsKspace(false); /* Do nothing if compression had failed */
               
            /* Write and switch back to real space */
            FileSingle<WaveFunction>::WriteFile(data);
            
            if (!cfail){
               data->swap(*_buf);
               data->IsKspace(false);
            }
            
            FileSize(0);
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










