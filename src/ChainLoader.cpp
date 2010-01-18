#include "ChainLoader.h"


#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"

namespace QDLIB
{
   
   /**
    * Recursive method to load an operator chain.
    * 
    * Should recognize special collective operators (OSum, OGridSum, OMultistate)
    */
   Operator* ChainLoader::LoadOperatorChain( XmlNode * Onode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      ParamContainer pm;
      string name;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
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
	 return sum;	 
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
	 return sum;
      } else if (name == "Multistate") { /* Matrix of operators */
	 log.cout() << "Multistate operator:\n";
	 log.IndentInc();
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OMultistate *matrix = new OMultistate();
	 
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
	 log.flush();
	 log.IndentDec();
	 return matrix;
      } else { 
	 O = mods->LoadOp( name );
	 log.cout() << pm << "---------------\n";
	 O->Init(pm);
      }
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
      if (name.length() == 1 && pm.isPresent("file")){ /* Lets try to load blind via file name & meta files */
         pm.GetValue( "file", name );
         FileWF file;
         file.Suffix(BINARY_WF_SUFFIX);
         file.Name(name);

         log.cout() << "Loading wavefunction from file definition: " << name;
         if (seqnum > -1 || pm.isPresent("num") ){ /* Read file from a sequence => only basename is given */
            file.ActivateSequence();
            int num = seqnum;
            
            if (pm.isPresent("num")){
               pm.GetValue("num", num);
            }
            file.Counter(num);
            log.cout() << " ("<< num << ")";
         }
         log.cout()  << endl;
         log.flush();
         
         file >> &WF;
         
         return WF;
      }
      
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
	 
	 WF = mods->LoadWF( name );
	 if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
	 
	 if (!pm.isPresent("file"))
	    throw ( EParamProblem("No file for loading wave function given") );
	 
	 /* load wf */
         pm.GetValue( "file", name );
         FileWF file;
         file.Suffix(BINARY_WF_SUFFIX);
         file.Name(name);
         if (seqnum > -1){ /* Read file from a sequence => only basename is given */
            file.ActivateSequence();
            file.Counter(seqnum);
         }
         file >> WF;
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
   OPropagator* ChainLoader::LoadPropagator( XmlNode *Unode, Operator **Hamiltonian)
   {
      ModuleLoader* mods = ModuleLoader::Instance();
//       QDClock *clock = QDGlobalClock::Instance();
      Logger& log = Logger::InstanceRef();
      
      ParamContainer pm, needs;
      string name, s;
      
      Operator *h=NULL;
      OPropagator *U=NULL;
      
      /* get the Parameters for the propagator */
      pm = Unode->Attributes();
      
      if (!pm.isPresent("name"))
	 throw (EParamProblem ("Missing propagator name") );
      
      pm.GetValue("name", name);
      
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
      int i=0;
      OSum *sum = new OSum(); /* Add single parts to sum operator (if more than one part) */
      
      log.cout() << "Intialize Operators:\n\n";
      
      while (needs.GetNextValue( name, s )){
	 ops = child->FindNode( name );
	 if ( ops == NULL && s != "opt") /* N error if need is an option */
	    throw ( EParamProblem ("Can't find an operator for the propagation", name) );
	 if ( ops != NULL ) { 
	    if (i > 0) sum->Add(h);
	    log.Header( name, Logger::SubSection );
	    log.IndentInc();
	    h = LoadOperatorChain( ops );
	    log.IndentDec();
	    log.cout() << endl;
	    U->AddNeeds( name, h );
	    delete ops;
	    i++;
	 }
      }
      if ( i > 1 ) { /* need for a sum or single operator ? */
	 sum->Add(h);
	 *Hamiltonian = sum;
      } else {
	 *Hamiltonian = h;
	 delete sum;
      }
      
      log.flush();
      log.IndentDec();
      
      return U;
   }
   


}
