#include "ChainLoader.h"


#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"
#include "GlobalOpList.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/ODMultistate.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"

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
   Operator* ChainLoader::LoadOperatorChain( XmlNode * Onode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      GlobalOpList& OpList = GlobalOpList::Instance();
      
      ParamContainer pm;
      string name, key, ref;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
      if (pm.isPresent("ref")) {
         pm.GetValue("ref", ref);
	 log.cout() << "Reference to " << ref << endl;
         return OpList[ref];
      }
      
      if (pm.isPresent("key"))
         pm.GetValue("key", key);
      else { /* If no key is given, we produce an internal key with a serial number */
         stringstream ss;
         ss << OP_LIST_ANONYMOUS_PREFIX << name << "#" << OpListAnonKey;
         key = ss.rdbuf()->str();
         OpListAnonKey++;
      }
      
      if (name == "Sum"){ /* Sum operator */
	 log.cout() << "Sum of operators:\n";
	 log.IndentInc();
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 
         Operator *osub;
	 OSum *sum = new OSum();
         
	 while (child->EndNode()){
	    osub = LoadOperatorChain( child );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    sum->Add( osub );
	    child->NextNode();
	 }
	 
	 log.flush();
	 log.IndentDec();
	 
	 O = sum;
      } else if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OGridSystem *gsub;
	 OGridSum *sum = new OGridSum;
	 while (child->EndNode()){
	    osub = LoadOperatorChain( child );
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
	    
	    osub = LoadOperatorChain( child );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    
	    
	    matrix->Add(osub, row, col);
	    child->NextNode();
         }
         O = matrix;
	 
	 log.flush();
	 log.IndentDec();
      } else { 
	 O = mods->LoadOp( name );
	 log.cout() << pm << "---------------\n";
	 O->Init(pm);
      }
      
      /* Register & prepare to exit */
      OpList.Add(key, O);


      return O;
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
    * 
    */
   WaveFunction * ChainLoader::LoadWaveFunctionChain( XmlNode * WFNode, int seqnum)
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      
      ParamContainer pm;
      string name;
      WaveFunction *WF=NULL;
      XmlNode *child;
      bool onoff;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Multistate"){ /* Further recursion for multistate WF */
	 log.cout() << "Multi state wave function:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
         
         WaveFunction *wfsub;
         
         /* Read single WF definitions into Multistate */
         WFMultistate *multi = new WFMultistate();
         while (child->EndNode()){
            string name;
            stringstream ss;
            int state;
            
            name = child->Name();
            ss << name.substr(2);
            ss >> state;
            log.cout() << "-State " << state << endl;
            
            wfsub = LoadWaveFunctionChain( child, seqnum );
            
            multi->Add( wfsub, state);
            child->NextNode();
         }
         
	 multi->Init(pm);
	 pm.GetValue( "normalize", onoff);
	 if ( onoff) {
	    log.cout() << "Normalized\n";
	    multi->Normalize();
	 }
	 log.IndentDec();
	 
	 return multi;
      } else if (name == "LC"){ /* Further recursion for linear combination */
	 log.cout() << "Build linear combination from wave functions:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
	 WaveFunction *wfadd;
	 ParamContainer pm_child;
	 while (child->EndNode()){
            double coeff=0;
            
            pm_child = child->Attributes();
            if (pm_child.isPresent("coeff")){
               pm_child.GetValue("coeff", coeff);
               
            } else if (pm_child.isPresent("coeff2")){
               pm_child.GetValue("coeff2", coeff);
               coeff = sqrt(coeff);
            }
            if(coeff != 0){
               log.cout().precision(8);
               log.cout() << fixed << "Coefficient = " << coeff << endl;
               log.cout() << fixed << "Coefficient^2 = " << coeff*coeff << endl;
            }
                    
            wfadd = LoadWaveFunctionChain( child, seqnum );
            
            if(coeff != 0)
               MultElements((cVec*) wfadd, coeff);
            
	    if (WF == NULL){
	       WF = wfadd->NewInstance();
	       *((cVec*) WF) = dcomplex(0,0);
	    }
	    
	    *WF += wfadd;
	    delete wfadd;
	    child->NextNode();
	 }
	 pm.GetValue( "normalize", onoff);
	 if ( onoff) {
	    log.cout() << "Normalized\n";
	    WF->Normalize();
	 }
	 log.IndentDec();
	 return WF; 
      } else { /* load a specific wf */
	 string fname;
	 
	 if ( name.length() != 1 ){ /* modules name is given */
	    WF = mods->LoadWF( name );
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
	 
	 if ( name.length() == 1 ) log.cout() << WF->Params() << endl;
	 log.cout() << pm << "------------------\n" << endl;
      }
      return WF;
   }
   
   
   /**
    * Load a propagator module and depending operators.
    * 
    * \param Unode   The XmlNode containing the parameters.
    * \param Hamiltonian  Constains the loaded Hamiltonian on exit.
    * \return The pre-initialized propagator (still need a ReInit)
    */
   OPropagator* ChainLoader::LoadPropagator( XmlNode *Unode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      GlobalOpList& OpList = GlobalOpList::Instance();
      
      ParamContainer pm, needs;
      string name, s, key, ref;
      
      Operator *h=NULL;
      OPropagator *U=NULL;
      
      /* get the Parameters for the propagator */
      pm = Unode->Attributes();

      if (pm.isPresent("ref")) {
         pm.GetValue("ref", ref);
         U = dynamic_cast<OPropagator*>(OpList[ref]);
         
         if (U == NULL)
            throw (EIncompatible("Operator in reference is not a propagator", ref));
         
	 log.cout() << "Reference to " << ref << endl;
         return U;
      }
      
      if (!pm.isPresent("name"))
	 throw (EParamProblem ("Missing propagator name") );
      
      pm.GetValue("name", name);
      
      if (pm.isPresent("key"))
         pm.GetValue("key", key);
      else { /* If no key is given, we produce an internal key with a serial number */
         stringstream ss;
         ss << OP_LIST_ANONYMOUS_PREFIX << name << "#" << OpListAnonKey;
         key = ss.rdbuf()->str();
         OpListAnonKey++;
      }
      
      /* Load the module */
      h = mods->LoadOp( name );
      U = dynamic_cast<OPropagator*>(h);
      if (U == NULL)
         throw ( EIncompatible("This is not a propagator", h->Name()) );
      
      h = NULL;
      
      /* Initialize */
      U->Init( pm );
      needs = U->TellNeeds();
      needs.ResetPosition();
      
      /* For the logfile... */
      log.cout() << "Loading : " << U->Name() << endl << endl;
      
      
      XmlNode *child = Unode->NextChild();
      XmlNode *ops;
      
      /* Search for needs and add it */      
      log.cout() << "Intialize Operators:\n\n";
      
      while (needs.GetNextValue( name, s )){
	 ops = child->FindNode( name );
	 if ( ops == NULL && s != "opt") /* N error if need is an option */
	    throw ( EParamProblem ("Can't find an operator for the propagation", name) );
	 if ( ops != NULL ) { 
	    log.Header( name, Logger::SubSection );
	    log.IndentInc();
	    h = LoadOperatorChain( ops );
	    log.IndentDec();
	    log.cout() << endl;
	    U->AddNeeds( name, h );
	    delete ops;
	 }
      }      
      OpList.Add(key, U);
      
      log.flush();
      log.IndentDec();
      
      return U;
   }
   
   int ChainLoader::OpListAnonKey = 0;

}
