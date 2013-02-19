#include "FileWF.h"
#include "WFMultistate.h"
#include "modules/ModuleLoader.h"
#include "tools/GlobalParams.h"
#include "tools/Logger.h"
#include "tools/helpers.h"
#include "config.h"


namespace QDLIB {

   FileWF::FileWF() : _compress(FILEWF_COMPRESSION), _compTolerance(FILEWF_COMPRESSION_TOLERANCE)
   {
      Suffix(BINARY_WF_SUFFIX);

#if defined(HAVE_LIBZ)  || defined (HAVE_LIBBZ2)
      /* Compression options from global params */
      ParamContainer& gp = GlobalParams::Instance();
      gp.GetValue("compress", _compress, FILEWF_COMPRESSION);
      
      if ( _compress ) {

         if ( gp.isPresent("comptol") )
            gp.GetValue("comptol", _compTolerance);

         if ( gp.isPresent("compmethod") ) {
            string s;
            gp.GetValue("compmethod", s);

            if (s == "ZLIB"){
               Compress(true);
               CompressMethod(FileSingle<WaveFunction>::ZLIB);
            } else if (s == "BZIP"){
               Compress(true);
               CompressMethod(FileSingle<WaveFunction>::BZIP);
            } else
               _compMethod = INVALID;
         }
      }
#endif
   }
   
   
   FileWF::~FileWF()
   {}


#ifdef HAVE_LIBBZ2

   void FileWF::_CompressBZIP(WaveFunction * data)
   {
//      data->Reduce(_compTolerance); /* Data reduction */
//
//      bool cfail = true;
//      size_t fsize = 0;
//
//      /* bzip2 compression */
//      if (_compLevel > 0) {
//         bz_stream strm;
//         dcomplex *in, *out;
//
//         /* Init bzip2 stream */
//         strm.bzalloc = NULL;
//         strm.bzfree = NULL;
//         strm.opaque = NULL;
//
//         if (BZ2_bzCompressInit(&strm, _compLevel, 0, 0) != BZ_OK) /* Init stream no verbose, workfactor set to default */
//            throw(EIOError("z-lib init failed"));
//
//         CheckBuf(data); /* Check if WF-buffer is intialized */
//
//         cfail = false;
//         out = _buf->begin(0);
//         in = data->begin(0);
//
//         strm.next_in = (char*) in;
//         strm.avail_in = data->sizeBytes();
//         /* output buffer */
//         strm.next_out = (char*) out;
//         strm.avail_out = data->sizeBytes();
//
//         /* hack to handle bad compression which is larger than input or invalid - just ignore compressed data */
//         if (BZ2_bzCompress(&strm, BZ_FINISH) != BZ_STREAM_END)
//         { /* Compression */
//            cfail = true;
//         }
//
//         if (BZ2_bzCompressEnd(&strm) != BZ_OK) cfail = true;
//
//         fsize = data->sizeBytes() - strm.avail_out;
//         FileSize(fsize + 8); /* data + magic */
//
//         if (strm.avail_out < ZLIB_MLEN)
//            cfail = true; /* compressed size + magic must be smaller than original size */
//      }
//
//      if (!cfail)
//      {
//         ParamContainer& p = data->Params(); /* indicate compression in meta data */
//         p.SetValue("compress", true);
//         p.SetValue("compressLevel", _compLevel);
//         p.SetValue("compressTol", _compTolerance);
//         p.SetValue("compressMethod", "BZIP");
////         data->swap(*_buf);
//
//         /* Append magic bytes */
//         string magic(BZIP_MAGIC);
//         char *c_magic = reinterpret_cast<char*> (data->begin(0));
//
//         magic.copy(&(c_magic[fsize]), ZLIB_MLEN - 1);
//         c_magic[fsize + ZLIB_MLEN - 1] = '\0';
//      }
//      else {
//         Logger& log = Logger::InstanceRef();
//         log.coutdbg() << "bzip2 compression failed!" << endl;
//         data->IsKspace(false); /* Do nothing if compression had failed */
//      }
//
//      /* Write and switch back to real space */
//      FileSingle<WaveFunction>::WriteFile( data);
//
//      if (!cfail)
//      {
////         data->swap(*_buf);
//         data->IsKspace(false);
//      }
//
//      FileSize(0);
//
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
      string basename = Name();
      WaveFunction *wf;
      
      FileSingleHeaderPm meta = ReadMeta();
      ParamContainer p = meta.params();
      string classname = meta.class_();
      
      if (classname == "WFMultistate"){ /* Explicit Multistate handling */
         WFMultistate *wfm = new WFMultistate();
         /** \todo register WF with collector? */

         if (Format() == binary){

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
               Name(basename + ss.str());
               wfm->Add( LoadWaveFunctionByMeta(), i ); /* Load module */
            }
            //cout << "rank : " << rank << endl << *wf<< endl;
            wf->SyncStrides();
         } else if (Format() == stream) { /* Read packed format from stream */
            int counter = Counter();

            vector<int> list;
            p.GetArray("StateList", list);

            unsigned int i=0;
            while (GetMeta().more_files_follow()) { /* Try to get all WFs in list */
               Counter(counter);
               if (i < list.size())
                  wfm->Add( LoadWaveFunctionByMeta(), list[i] );
               else
                  break;
               i++;
            }
            if (i != list.size())
               throw(EIOError("Number of states given list doesn't match number of states in files"));
         }
         wfm->Init(p);
         wf = wfm;
      } else { /* handling for Single state WFs */
         wf = ModuleLoader::Instance()->LoadWF(classname);
         ReadData(wf); /* Load data */
      }
      
      Name(basename); /* We modfied the name => switch back to original */
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
   void FileWF::ReadWaveFunction(WaveFunction * data)
   {
      WFMultistate *wfm;

      wfm = dynamic_cast<WFMultistate*>(data);

      if (wfm != NULL){ /* Load Multistate set */
         string basename = Name();

         if (wfm->States() < 1)
            throw ( EParamProblem("Reading WaveFunction: Invalid number of states", wfm->States()) );

         // Detect format;
         FileSingleHeaderPm meta;
         meta = ReadMeta();
         ParamContainer pmt = meta.params();

         int counter = Counter();

         if (Format() == binary){ /* The old way of storing files. */
            for (int i=wfm->MPIrank(); i < wfm->States(); i += wfm->MPIsize()){ /* Loop over States */
               Counter(counter);    /* Every ReadFile() increases counter => restore it when using it diff. states */
               stringstream ss;
               ss << "-" << i;
               Name(basename + ss.str());
               ReadWaveFunction(wfm->State(i)); /* Load data */
            }

            Name(basename); /* We modified the name => switch back to original */

         } else if (Format() == stream) { /* Read packed format from stream */
            vector<int> list;
            ParamContainer pm = meta.params();
            pm.GetArray("StateList", list);

            unsigned int i=0;
            while (GetMeta().more_files_follow()) {
               Counter(counter);
               if (i < list.size())
                  ReadWaveFunction(wfm->State(list[i]));
               else
                  break;
               i++;
            }
            if (i != list.size())
               throw(EIOError("Number of states given list doesn't match number of states in files"));
         }

         ParamContainer p = meta.params();
         wfm->Init(p);
         wfm->SyncStrides();
      } else { /* Load Single WaveFunction */

         FileSingleHeader_Compression compress = ReadMeta().compression();
         if (compress != FileSingleHeader_Compression_UNCOMPRESSED){
            data->IsKspace(true); /* In this case the data must be placed in k-space buffer */
         }

         ReadData(data);

         if (compress != FileSingleHeader_Compression_UNCOMPRESSED)
            data->Restore();
      }
     
   }


   /**
    * Write WaveFunction to file.
    * 
    * This method can write multistate WFs.
    */
   void FileWF::WriteWaveFunction(WaveFunction * data, bool more_files_follow)
   {
      WFMultistate *wfm;

      wfm = dynamic_cast<WFMultistate*>(data);

      if (wfm != NULL){ /* Write Multistate set */
         if (wfm->States() < 1)
            throw ( EParamProblem("Writing WaveFunction: Invalid number of states", wfm->States()) );

         if (Format() == autodetect)   /* We have to take this decision. */
            Format(stream);

         int counter = Counter();
         if (Format() == binary) {
            string basename = Name();

            for (int i=wfm->MPIrank(); i < wfm->States(); i+=wfm->MPIsize()){
               Counter(counter);    /* Every WriteFile() increases counter => restore it when using it diff. states */
               stringstream ss;
               ss << "-" << i;
               Name(basename + ss.str());
               WriteWaveFunction(wfm->State(i));
            }

            Name(basename); /* We modfied the name => switch back to original */

            /* Write meta file for Multistate container */
            if (wfm->MPIrank() == 0){
               ParamContainer p;
               p = wfm->Params();
               p.SetValue("CLASS", wfm->Name() );
               WriteMeta(p);
            }
         } else if (Format() == stream){
            int* list = new int[wfm->States()];

            for (int i=0; i < wfm->States(); i++) list[i] = i;
            data->Params().SetArray("StateList", list, (int) wfm->States());  /* State list tells us what is really stored */
            WriteMeta(data, true, true); /* This is the Multistate header */

            for (int i=0; i < wfm->States(); i++){ /* Right now we just save all states \TODO add save threshold */
               Counter(counter);
               if (i == wfm->States()-1 && more_files_follow == false)
                  WriteWaveFunction(wfm->State(i), false);
               else
                  WriteWaveFunction(wfm->State(i), true);
            }
            delete[] list;
         }
      } else { /* Write single file */
         if (_compress) {
            if (_compMethod == INVALID)
               throw( EParamProblem("Invalid file compression method given") );
            else
               data->Reduce(_compTolerance);
         }
         WriteMeta(data, more_files_follow);
         WriteData(data);

         if (_compress)
            data->IsKspace(false);
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
	       
      file.ReadWaveFunction(data);
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










