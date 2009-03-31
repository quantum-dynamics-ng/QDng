#include "ChainLoader.h"

#include "tools/FileSingleDefs.h"
#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"

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
    * Works recursive.
    * 
    * \todo clean up meta data handling of multistate wfs.
    */
   WaveFunction * ChainLoader::LoadWaveFunctionChain( XmlNode * WFNode, bool empty)
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      
      ParamContainer pm;
      string name;
      WaveFunction *WF=NULL;
      XmlNode *child;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Multistate"){ /* Further recursion for multistate WF */
	 log.cout() << "Multi state wave function:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
	 WaveFunction *wfsub;
	 WFMultistate *multi = new WFMultistate();
	 while (child->EndNode()){
	    string name;
	    stringstream ss;
	    int state;
	    
 	    wfsub = LoadWaveFunctionChain( child );
	    
	    name = child->Name();
	    ss << name.substr(2);
	    ss >> state;
	    multi->Add( wfsub, state);
	    child->NextNode();
	 }
	 multi->Init(pm);
	 if ( pm.isPresent("normalize") ) {
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
	 double coeff;
	 ParamContainer pm_child;
	 while (child->EndNode()){
	    wfadd = LoadWaveFunctionChain( child );
	    pm_child = child->Attributes();
	    if (WF == NULL){
	       WF = wfadd->NewInstance();
	       *((cVec*) WF) = dcomplex(0,0);
	    }
	    if (pm_child.isPresent("coeff")){
	       pm_child.GetValue("coeff", coeff);
	       MultElements((cVec*) wfadd, coeff);
	    }
	    
	    *WF += wfadd;
	    delete wfadd;
	    child->NextNode();
	 }
	 if ( pm.isPresent("normalize") ) {
	    log.cout() << "Normalized\n";
	    WF->Normalize();
	 }
	 log.IndentDec();
	 return WF; 
      } else { /* load a specific wf */
	 
	 WF = mods->LoadWF( name );
	 if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
	 
	 if (!pm.isPresent("file") && !empty)
	    throw ( EParamProblem("No file for loading wave function given") );
	 
	 /* load wf */
	 if (!empty){
	    pm.GetValue( "file", name );
	    FileWF file;
	    file.Suffix(BINARY_WF_SUFFIX);
	    file.Name(name);
	    file >> WF;
	    if ( pm.isPresent("normalize") ) {
	       log.cout() << "Normalizing...\n";
	       WF->Normalize();
	    }
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
      QDClock *clock = QDGlobalClock::Instance();
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
	 if (i > 0) sum->Add(h);
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
