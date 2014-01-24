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

      if (classname.size() <= 1)
         throw (EParamProblem("No CLASS information found. LoadWaveFunctionByMeta failed."));

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
            wfm->SyncStrides();
            wfm->Init(p);
         } else if (Format() == stream) { /* Read packed format from stream */
            ReadDataFromStream(wfm);  // Do a dummy read

            int counter = Counter();

            unsigned int i=0;
            while (GetMeta().more_files_follow()) { /* Try to get all WFs in list */
               Counter(counter);
               wfm->Add( LoadWaveFunctionByMeta(), i);
               i++;
            }

            wfm->Init(i);
         }


         wf = wfm;
      } else { /* handling for Single state WFs */
         wf = ModuleLoader<WaveFunction>::Instance()->Load(classname);
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

            ParamContainer p = meta.params();
            wfm->Init(p);

         } else if (Format() == stream) { /* Read packed format from stream */
            ReadDataFromStream(data);  // Do a dummy read

            unsigned int i=0;
            while (GetMeta().more_files_follow()) {
               Counter(counter);
               ReadWaveFunction(wfm->State(i));
               i++;
            }

            wfm->Init(i);
         }

         wfm->SyncStrides();
      } else { /* Load Single WaveFunction */
         WaveFunction* tmp_data = data;

         FileSingleHeader_Compression compress = ReadMeta().compression();
         if (compress != FileSingleHeader_Compression_UNCOMPRESSED){
            tmp_data = data->NewInstance();
         }

         ReadData(tmp_data);

         if (compress != FileSingleHeader_Compression_UNCOMPRESSED){
            data->Restore(tmp_data);
            DELETE_WF(tmp_data);
         }
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

            WriteMeta(data, true, false); /* This is the Multistate header */

            for (int i=0; i < wfm->States(); i++){
               Counter(counter);
               if (i == wfm->States()-1 && more_files_follow == false)
                  WriteWaveFunction(wfm->State(i), false);
               else
                  WriteWaveFunction(wfm->State(i), true);
            }

         }
      } else { /* Write single file */
         WaveFunction* tmp_data = data;
         if (_compress) {
            if (_compMethod == INVALID)
               throw( EParamProblem("Invalid file compression method given") );
            else
               tmp_data = data->Reduce(_compTolerance);
         }

         WriteMeta(tmp_data, more_files_follow);
         WriteData(tmp_data);

         if (_compress)
            DELETE_WF(tmp_data);
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










