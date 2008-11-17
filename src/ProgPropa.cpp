#include "modules/ModuleLoader.h"
#include "ProgPropa.h"


#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"

namespace QDLIB {

   ProgPropa::ProgPropa(XmlNode & PropaNode) :
	 _propaNode(PropaNode), _ContentNodes(NULL),
         _wcycle(DEFAULT_WRITE_CYCLE), _U(NULL)
   {
   }
   
   
   ProgPropa::~ProgPropa()
   {
      if (_U != NULL) delete _U;
   }


   /**
    * Recursive method to load an operator chain.
    * 
    * Should recognize collective operators (OSum, OGridSum, OMultistate)
    */
   Operator* ProgPropa::_LoadOperatorChain( XmlNode * Onode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      ParamContainer pm;
      string name;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Sum"){ /* Sum operator */
	 cout << "Loading Sum: " << name << endl;
	 child = Onode->NextChild();
	 Operator *osub;
	 OSum *sum = new OSum();
	 while (child->EndNode()){
	    osub = _LoadOperatorChain( child );
// 	    sum->Add( osub );
	    child->NextNode();
	 }
	 return sum;	 
      } else if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
	 Operator *osub;
	 OGridSum *sum = new OGridSum;
	 while (child->EndNode()){
	    osub = _LoadOperatorChain( child );
// 	    sum->Add( osub );
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
	 //O = mods->LoadOp( name );
	 cout << "Loading: " << name << endl;
	 //O->Init(pm);
      }
      return O;
      
   }


   
   /**
    * Init all the global propa values needed.
    */
   void ProgPropa::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance();
      ParamContainer attr;
   
      attr = _propaNode.Attributes();
   
      double dt;
      int steps;
   
   
      /* Init the clock */
      attr.GetValue("dt", dt);
      attr.GetValue("steps", steps);
   
      if (dt == 0)
	 throw ( EParamProblem("Zero time step defined") );
      if (steps == 0)
	 throw ( EParamProblem("Zero number of time step defined") );
   
      clock->Dt(dt);
      clock->Steps(steps);
   
      /* Init write cycles */
      if ( attr.isPresent("wcycle") ) {
	 attr.GetValue("wcycle", steps);
	 if ( steps < 1)
	    throw ( EParamProblem ("Write cycle less than one defined") );
      }
   }

   /**
    * Load a propagator module and depending operators.
    */
   void ProgPropa::_LoadPropagator( XmlNode *Unode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      ParamContainer pm, needs;
      string name, s;
      
      
      /* get the Parameters for the propagator */
      pm = Unode->Attributes();
      
      if (!pm.isPresent("name"))
	 throw (EParamProblem ("Missing propagator name") );
      
      pm.GetValue("name", name);
      
      /* Load the module */
      //_U = dynamic_cast<OPropagator*> mods->LoadOp( name );
      cout << "Loading: " << name << endl;
      
      /* Initialize */
      //_U->Init( *pm );
      // needs = _U->TellNeeds();
      needs.SetValue( "hamilton", "" ); //DBG
      needs.ResetPosition();
      
      
      XmlNode *child = Unode->NextChild();
      XmlNode *ops;
      
      while (needs.GetNextValue( name, s )){
	 cout << "propagator wants: " << name << endl;
	 ops = child->FindNode( name );
	 if ( ops == NULL )
	    throw ( EParamProblem ("Can't find an operator for the propagtion") );
	 _LoadOperatorChain( ops );
	 delete ops;
      }
      
      
   }

   
   /**
    * Run the propagation program.
    */
   void ProgPropa::Run()
   {
      XmlNode *section;
      /* Init global Propa parameters */
      _InitParams();
      
      /* fetch the ChildNodes */
      _ContentNodes = _propaNode.NextChild();
      
      /* Load & Init the propagator */
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _LoadPropagator( section );
      delete section;
      
      
   }

}



