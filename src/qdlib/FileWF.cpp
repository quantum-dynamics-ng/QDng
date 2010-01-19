#include "FileWF.h"
#include "WFMultistate.h"
#include "modules/ModuleLoader.h"


namespace QDLIB {

   FileWF::FileWF()
   {
      _suffix = BINARY_WF_SUFFIX;
   }
   
   
   FileWF::~FileWF()
   {
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
         FileSingle<WaveFunction>::ReadFile(wf); /* Load data */
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
         FileSingle<WaveFunction>::ReadFile(data);
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
      } else {
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







