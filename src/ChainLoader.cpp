#include "ChainLoader.h"

#include "sys/FileSingleDefs.h"
#include "modules/ModuleLoader.h"
#include "sys/QDGlobalClock.h"

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
    * Should recognize collective operators (OSum, OGridSum, OMultistate)
    */
   Operator* ChainLoader::LoadOperatorChain( XmlNode * Onode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      ParamContainer pm;
      string name;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Sum"){ /* Sum operator */
	 cout << "Sum of operators: " << endl;
	 child = Onode->NextChild();
	 Operator *osub;
	 OSum *sum = new OSum();
	 while (child->EndNode()){
	    osub = LoadOperatorChain( child );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    sum->Add( osub );
	    child->NextNode();
	 }
	 return sum;	 
      } else if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
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
	 throw (EParamProblem ("Multistate operator not implementet yet") );
	 child = Onode->NextChild();
	 Operator *osub;
	 OMultistate *matrix;
	 while (child != NULL){
	    //osub = _LoadOperatorChain( child );
	   // matrix(x,y) = osub
	 }
	 return matrix;
      } else { 
	 O = mods->LoadOp( name );
	 cout << pm << "---------------\n";
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
    */
   WaveFunction * ChainLoader::LoadWaveFunctionChain( XmlNode * WFNode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      ParamContainer pm;
      string name;
      WaveFunction *WF=NULL;
      XmlNode *child;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Multistate"){
	 cout << "Multi state wave function:" << endl;
	 child = WFNode->NextChild();
	 WaveFunction *wfsub;
	 WFMultistate *multi = new WFMultistate();
	 while (child->EndNode()){
 	    wfsub = LoadWaveFunctionChain( child );
	    multi->Add( wfsub );
	    child->NextNode();
	 }
	 return multi;
      } else if (name == "LC"){
	 cout << "Build linear combination from wave functions:" << endl;
	 child = WFNode->NextChild();
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
	       *wfadd *= coeff;
	    }
	    *WF += wfadd;
	    delete wfadd;
	    child->NextNode();
	 }
	 if ( pm.isPresent("normalize") ) WF->Normalize();
	 return WF; 
      } else {
	 
	 WF = mods->LoadWF( name );
	 if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
	 
	 pm.GetValue( "file", name );
	 FileWF file;
	 file.Suffix(BINARY_WF_SUFFIX);
	 file.Name(name);
	 cout << pm << "------------------\n" << endl;
	 file >> WF;
	 
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
      cout << "\tPropagator: " << U->Name() << endl << endl;
      
      
      XmlNode *child = Unode->NextChild();
      XmlNode *ops;
      
      /* Search for needs and add it */
      int i=0;
      OSum *sum = new OSum();
      
      cout << "Intialize Operators:\n\n";
      while (needs.GetNextValue( name, s )){
	 if (i > 0) sum->Add(h);
	 ops = child->FindNode( name );
	 if ( ops == NULL )
	    throw ( EParamProblem ("Can't find an operator for the propagation", name) );
	 cout << "->" << name <<endl;
	 h = LoadOperatorChain( ops );
	 cout << endl;
	 U->AddNeeds( name, h );
	 delete ops;
	 i++;
      }
      if ( i > 1 ) {
	 sum->Add(h);
	 *Hamiltonian = sum;
      } else {
	 *Hamiltonian = h;
	 delete sum;
      }
      return U;
   }
   


}
