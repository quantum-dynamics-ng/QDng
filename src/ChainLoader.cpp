#include "ChainLoader.h"


#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/ODMultistate.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"

#include "GlobalWFList.h"

namespace QDLIB
{
   
   /**
    * Recursive method to load an operator chain.
    * 
    * Should recognize special collective operators (OSum, OGridSum, OMultistate)
    *
    * \param Onode       The XML node conataining the operator definition
    *
    * Registering the operator means that the GlobalOpList takes responsibility for destruction.
    */
   Operator* ChainLoader::LoadOperatorChain( XmlNode * Onode, bool persist )
   {
      ModuleLoader<Operator>* mods = ModuleLoader<Operator>::Instance();
      Logger& log = Logger::InstanceRef();
      GlobalOpList& OpList = GlobalOpList::Instance();
      
      ParamContainer pm;
      string name, key, ref, label;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
      if (pm.isPresent("ref")) {
         pm.GetValue("ref", ref);
	 log.cout() << "Reference to " << ref << endl << endl;
         return OpList[ref];
      }

      pm.GetValue("key", key);

      if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OGridSystem *gsub;
	 OGridSum *sum = new OGridSum;
	 while (child->EndNode()){
	    osub = LoadOperatorChain( child, persist );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    if ( (gsub = dynamic_cast<OGridSystem*>(osub) ) == NULL )
	       throw ( EIncompatible("Tried to load into GridSum"), osub->Name() );
	    sum->Add( gsub );
	    child->NextNode();
	 }
	 
	 log.flush();
	 log.IndentDec();
	 
	 O = sum;
      } else if (name == "Multistate" || name == "DMultistate") { /* Matrix of operators */
	 log.cout() << "Multistate operator:\n";

	 bool unity; pm.GetValue("unity", unity);
	 if (unity) log.cout() << "default diagonal : 1\n";
	 else log.cout() << "default diagonal : 0\n";

	 bool nonhermitian; pm.GetValue("nonhermitian", nonhermitian);
	 if (nonhermitian) log.cout() << "default offdiagonal : 0\n";
	 else log.cout() << "default offdiagonal : Hij = Hji+ \n";

	 log.IndentInc();
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OMultistate *matrix = NULL;
         if (name == "Multistate") matrix = new OMultistate();
         if (name == "DMultistate") matrix = new ODMultistate();
	 
	 matrix->Init(pm);
	 while (child->EndNode()){
	    string name;
	    stringstream ss_row, ss_col;
	    int row, col;
	    
	    name = child->Name();
	    ss_row << name.substr(1, name.find('.')-1);
	    ss_col << name.substr(name.find('.')+1);
	    ss_row >> row;
	    ss_col >> col;
	    
	    log.cout() << "H(" << row << "," << col << ")\n";
	    
	    osub = LoadOperatorChain( child, persist );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    
	    
	    matrix->Add(osub, row, col);
	    child->NextNode();
         }
         O = matrix;
	 
	 log.flush();
	 log.IndentDec();
      } else { 
	 O = mods->Load( name );
         
         if (dynamic_cast<OList*>(O) != NULL){ /* Sum/Product operator and propagators */
            log.cout() << O->Name()<<endl;
            log.IndentInc();
            log.flush();
            child = Onode->NextChild();

            O->Init(pm);

            if (child != NULL){ /* Check if sub operators are given */
               child->AdjustElementNode();
         
               Operator *osub;
               OList *list = dynamic_cast<OList*>(O);


               while (child->EndNode()){
                  osub = LoadOperatorChain( child, persist );
                  if (osub == NULL)
                     throw ( EParamProblem("Can't load operator") );
                  list->Add(child->Name(), osub );
                  child->NextNode();
               }
            }

            log.flush();
            log.IndentDec();
         } else {
         
            log.cout() << pm << "---------------\n";
            O->Init(pm);
            log.flush();
         }
      }
      
      /* Register & prepare to exit */
      OpList.Add(key, O, persist);

      if (pm.isPresent("label")){
         pm.GetValue("label", label);
         OpList.SetLabel(key, label);
      }

      return O;
   }

   WaveFunction* ChainLoader::LoadWFMultistate_( XmlNode *WFNode, ParamContainer& pm, int seqnum)
   {
      Logger& log = Logger::InstanceRef();

      WaveFunction *wfsub;

      /* Read single WF definitions into Multistate */
      WFMultistate *multi = new WFMultistate();
      while (WFNode->EndNode()){
         string name;
         stringstream ss;
         int state;

         name = WFNode->Name();
         ss << name.substr(2);
         ss >> state;
         log.cout() << "-State " << state << endl;

         wfsub = LoadWaveFunctionChain( WFNode, seqnum );

         multi->Add( wfsub, state);
         WFNode->NextNode();
      }

      multi->Init(pm);

      return multi;
   }

   WaveFunction* ChainLoader::LoadWFMultimap_( XmlNode *WFNode, ParamContainer& pm,  int seqnum)
   {
      Logger& log = Logger::InstanceRef();

      WFMultistate *wfin = dynamic_cast<WFMultistate*>(LoadWaveFunctionChain( WFNode, seqnum ));

      if (wfin == NULL)
         throw(EIncompatible("Multimap needs a multi state wave function as input"));

      vector<int> statemap;
      vector<double> coeffs;

      if (pm.isPresent("statemap")){
         pm.GetArray("statemap", statemap);
      }

      if (statemap.size() > wfin->States())
	throw(EParamProblem("More states in statemap than input states available"));


      if (pm.isPresent("coeffs")){
         pm.GetArray("coeffs", coeffs);
      } else {
         coeffs.assign(statemap.size(), 1);
      }

      /* Map Multistate to Multistate*/
      if (statemap.size() > 1) {
	WFMultistate *wfout = new WFMultistate();
	for (size_t i=0; i < statemap.size(); i++){
	   double cf = 1;
	   if (statemap[i] >= wfin->States() || statemap[i] < 0)
	      throw (EParamProblem("Multimap: state index is invalid"));

	   wfout->Add(wfin->State(i), statemap[i]);

	   if (i >= coeffs.size()) {
	      *(wfout->State(statemap[i])) *= 0;
	   } else {
	      *(wfout->State(statemap[i])) *= coeffs[i];
	      log.cout() << coeffs[i] << " * " << i << " -> " << statemap[i] << endl;
	   }
	}

	ParamContainer mspm;
	wfout->Init(mspm);

	return wfout;
      } else { /* Map Multistate to a single state WF */
	   if (statemap[0] >= wfin->States() || statemap[0] < 0)
	      throw (EParamProblem("Multimap: state index is invalid"));

	  WaveFunction *wfout = wfin->State(statemap[0])->NewInstance();
	  *wfout = wfin->State(statemap[0]);
	  *wfout *= coeffs[0];

	  return wfout;
      }
   }
   
   WaveFunction* ChainLoader::LoadWFLC_( XmlNode *WFNode, ParamContainer& pm,  int seqnum)
   {
      Logger& log = Logger::InstanceRef();
      WaveFunction *wfadd;
      WaveFunction *WF = NULL;
      ParamContainer pm_child;

      while (WFNode->EndNode()){
         double coeff=0;

         wfadd = LoadWaveFunctionChain( WFNode, seqnum );

         if(coeff != 0)
            MultElements((cVec*) wfadd, coeff);

         if (WF == NULL){
            WF = wfadd->NewInstance();
            *WF = wfadd;
         } else {
            *WF += wfadd;
         }

         *WF += wfadd;
         DELETE_WF(wfadd);
         WFNode->NextNode();
      }

      return WF;
   }

   /**
    * Load all wave functions from the input.
    * 
    * This method also recognizes Multistate, LC (Linear combination).
    * Works infinetly recursive.
    * 
    * \li Special Parameter:
    * \li normalize:  Works on every level (default false)
    * 
    * \li coeff   Coefficient in LC
    * \li coeff2  Coefficient squared  in LC
    * \li phase   Apply a phase factor (in units of pi)
    * 
    */
   WaveFunction * ChainLoader::LoadWaveFunctionChain( XmlNode * WFNode, int seqnum)
   {
      ModuleLoader<WaveFunction>* mods = ModuleLoader<WaveFunction>::Instance();
      Logger& log = Logger::InstanceRef();
      
      ParamContainer pm;
      string name, key;
      WaveFunction *WF=NULL;
      XmlNode *child;
      bool onoff;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (pm.isPresent("key"))      /* Check for a key to store in the Global WF container */
         pm.GetValue("key", key);

      if (name == "Multistate"){ /* Further recursion for multistate WF */
	 log.cout() << "Multi state wave function:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
         
	 WF = LoadWFMultistate_( child, pm, seqnum);

         
         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << WF->Norm() <<endl;
         }
         
	 log.IndentDec();
      } else if (name== "Multimap") { /* Load a Multistate wave function and remap the states */
         log.cout() << "Re-map multi state wave function" << endl;
         log.IndentInc();

         /* load the wf from the sub node */
         child = WFNode->NextChild();
         child->AdjustElementNode();

         WF = LoadWFMultimap_(child, pm, seqnum);
         log.IndentDec();

      } else if (name == "LC"){ /* Further recursion for linear combination */
	 log.cout() << "Build linear combination from wave functions:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();

	 WF = ChainLoader::LoadWFLC_( child,  pm,  seqnum);

         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << WF->Norm() <<endl;
         }

	 log.IndentDec();

      } else { /* load a specific wf */
	 string fname;
	 
	 if ( name.length() != 1 ){ /* modules name is given */
	    WF = mods->Load( name );
	    if (WF == NULL)
	       throw ( EParamProblem("WaveFunction module loading failed") );
	 }
	    
	 if (!pm.isPresent("file"))
	    throw ( EParamProblem("No file for loading wave function given") );
	 
	 /* load wf */
	 FileWF file;
         
	 pm.GetValue( "file", fname );
         file.Suffix(BINARY_WF_SUFFIX);
         file.Name(fname);
         if (seqnum > -1){ /* Read file from a sequence => only basename is given */
            file.ActivateSequence();
            file.Counter(seqnum);
         }
	 
	 if ( name.length() != 1 ) /* modules name is given */
            file >> WF;
	 else                      /* Load by meta */
	    file >> &WF;
	 
         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << WF->Norm() <<endl;
         }
         
	 if ( name.length() == 1 ) log.cout() << WF->Params() << endl;
	 log.cout() << pm << "------------------\n" << endl;
      }  /* end - load a specific wf */

      pm.GetValue( "normalize", onoff);
      if ( onoff) {
         log.cout() << "Normalizing...\n";
         WF->Normalize();
      }

      if (pm.isPresent("phase")){
         double phase;
         pm.GetValue("phase", phase);
         log.cout() << "Apply phase factor: " << phase << " pi\n";
         *WF *= cexpI(phase * M_PI);
      }

      double coeff=0;

      if (pm.isPresent("coeff")){
         pm.GetValue("coeff", coeff);

      } else if (pm.isPresent("coeff2")){
         pm.GetValue("coeff2", coeff);
         coeff = sqrt(coeff);
      }

      if(coeff != 0){
         log.cout().precision(8);
         log.cout() << fixed << "Coefficient = " << coeff << endl;
         log.cout() << fixed << "Coefficient^2 = " << coeff*coeff << endl;
         MultElements((cVec*) WF, coeff);
      }

      /* Add to global container */
      if (! key.empty()){
         GlobalWFList& wflist = GlobalWFList::Instance();
         wflist.Add(key, WF, true);
      }

      return WF;
   }
   
   
   /**
    * Load a propagator module and depending operators.
    * 
    * \param Unode   The XmlNode containing the parameters.
    * \param persist Make the Propagator persistent in the GlobalOpList.
    * \return The pre-initialized propagator (still need a Init(WF) )
    */
   OPropagator* ChainLoader::LoadPropagator( XmlNode *Unode, bool persist)
   {
      Operator* O = LoadOperatorChain(Unode, persist);
      OPropagator* U = dynamic_cast<OPropagator*>(O);

      if (U == NULL)
         throw (EIncompatible("This is not a propagator!"), O->Name());

      return U;
   }
   
   

}
