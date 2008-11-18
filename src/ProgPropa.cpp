#include "modules/ModuleLoader.h"

#include "ProgPropa.h"

#include "sys/FileSingleDefs.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"

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
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
 	    sum->Add( osub );
	    child->NextNode();
	 }
	 return sum;	 
      } else if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
	 Operator *osub;
	 OGridSum *sum = new OGridSum;
	 while (child->EndNode()){
	    osub = _LoadOperatorChain( child );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );	    
 	    sum->Add( dynamic_cast<OGridSystem*>(osub) );
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
	 cout << "Loading: " << name << endl;
	 cout << pm;
	 O->Init(pm);
      }
      return O;
      

      

   }


   /**
    * Load all Wave functions from the input.
    * 
    * This method also recognizes Multistate. Works recursive
    * 
    */
   WaveFunction * QDLIB::ProgPropa::_LoadWaveFunctionChain( XmlNode * WFNode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      ParamContainer pm;
      string name;
      WaveFunction *WF=NULL;
      XmlNode *child;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Multistate"){
	 child = WFNode->NextChild();
	 WaveFunction *wfsub;
	 WFMultistate *multi = new WFMultistate();
	 while (child->EndNode()){
	    wfsub = _LoadWaveFunctionChain( child );
	    multi->Add( wfsub );
	    child->NextNode();
	 }
	 return multi;
      } else {
	 
	 WF = mods->LoadWF( name );
	 if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
	 
	 pm.GetValue( "file", name );
	 FileWF file;
	 file.Suffix(BINARY_WF_SUFFIX);
	 file.Name(name);
	 cout << "WF from file: " << name << endl;
	 file >> WF;
	 
      }
      return WF;
   }
   
   
   /**
    * Init all the global propa values needed including the reporter values.
    */
   void ProgPropa::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
   
      attr = _propaNode.Attributes();
   
      double dt;
      int steps;
      string s;
   
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
      
      /* Check the reporter values */
      if ( attr.isPresent("norm") ) {
	 attr.GetValue("norm", s);
	 if (s == "yes") _reporter.Norm( true );
	 else _reporter.Norm( false );
      }
      if ( attr.isPresent("energy") ) {
	 attr.GetValue("energy", s);
	 if (s == "yes") _reporter.Energy( true );
	 else _reporter.Energy(  false );
      }
      if ( attr.isPresent("proj0") ) {
	 attr.GetValue("proj0", s);
	 if (s == "yes") _reporter.Proj0( true );
	 else _reporter.Proj0( false );
      }
      if ( attr.isPresent("spectrum") ) {
	 attr.GetValue("spectrum", s);
	 _reporter.Spectrum( s );
      }

      
   }

   /**
    * Load a propagator module and depending operators.
    */
   void ProgPropa::_LoadPropagator( XmlNode *Unode )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      QDClock *clock = QDGlobalClock::Instance();
      
      ParamContainer pm, needs;
      string name, s;
      
      
      /* get the Parameters for the propagator */
      pm = Unode->Attributes();
      
      if (!pm.isPresent("name"))
	 throw (EParamProblem ("Missing propagator name") );
      
      pm.GetValue("name", name);
      
      /* Load the module */
      _U = dynamic_cast<OPropagator*>(mods->LoadOp( name ));
      
      /* Initialize */
      _U->Init( pm );
      needs = _U->TellNeeds();
      needs.ResetPosition();
      
      
      XmlNode *child = Unode->NextChild();
      XmlNode *ops;
      
      /* Search for needs and add it */
      while (needs.GetNextValue( name, s )){
	 ops = child->FindNode( name );
	 if ( ops == NULL )
	    throw ( EParamProblem ("Can't find an operator for the propagation") );
	 _H = _LoadOperatorChain( ops );
	 delete ops;
	 _U->AddNeeds( name, _H );
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
      
      
      /* Load the initial Wavefunction */
      WaveFunction *Psi;
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      
      Psi = _LoadWaveFunctionChain( section );
      delete section;
      
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      
      /* Give the reporter module what it needs */
      _reporter.PsiInitial( Psi );
      _reporter.Hamilton( _H );
      
      /* Make sure our hamiltonian is initalized */
      _H->Expec(Psi);
      
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _U->Forward();
      _U->ReInit();
      
      /* The propagation loop */
      for (lint i=0; i < clock->Steps(); i++){
	 if (clock->TimeStep() % _wcycle == 0)  _reporter.Analyze( Psi );
	*_U *= Psi;
	++(*clock);
      }
   }

}





