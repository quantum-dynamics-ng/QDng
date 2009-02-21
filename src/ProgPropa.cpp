#include "ProgPropa.h"

#include "sys/FileSingleDefs.h"
#include "ChainLoader.h"


namespace QDLIB {

   ProgPropa::ProgPropa(XmlNode & PropaNode) :
	 _propaNode(PropaNode), _ContentNodes(NULL),
	 _wcycle(DEFAULT_WRITE_CYCLE), _fname(DEFAULT_BASENAME), _U(NULL), _H(NULL)
   {
   }
   
   
   ProgPropa::~ProgPropa()
   {
      if (_U != NULL) delete _U;
      if (_H != NULL) delete _H;
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
	 attr.GetValue("wcycle", _wcycle);
	 if ( steps < 1)
	    throw ( EParamProblem ("Write cycle less than one defined") );
	 _reporter.WriteCycle(_wcycle);
      }
      
      /* Init propagation file basename */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
      }
      
      cout << "Propagation parameters:\n\n";
      cout << "\tNumber of steps: " <<  clock->Steps() << endl;
      cout.precision(2); cout << "\tTime step: " << fixed << clock->Dt() << endl;
      cout.precision(2); cout << "\tWrite cycles: " << _wcycle << endl;
      cout.precision(2); cout << "\tOverall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      cout << "\tBasename for wave function output: " << _fname << endl;
      cout.precision(6); cout << scientific;
      
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
    * Run the propagation program.
    */
   void ProgPropa::Run()
   {
      XmlNode *section;
      Operator *_h;
      
      /* Init global Propa parameters */
      _InitParams();
      
      /* fetch the ChildNodes */
      _ContentNodes = _propaNode.NextChild();
      
      /* Load & Init the propagator */
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section, &_h );
      delete section;
            
      /* Load the initial Wavefunction */
      cout << "Initalize Wave function:\n";
      WaveFunction *Psi;
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      
      Psi = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
           
      /* Make sure our hamiltonian is initalized */
      _h->Init(Psi);
      cout << "Initial engergy: " << _h->Expec(Psi) << endl;
      
      /* Copy, since the propagator will propably scale it etc. */
      _H = _h->NewInstance();
      *_H = _h; 

      cout << "Initial engergy: " << _H->Expec(Psi) << endl;
      /* Give the reporter module what it needs */
      _reporter.PsiInitial( Psi );
      _reporter.Hamilton( _H );
      
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _H->UpdateTime();
      _U->Init(Psi);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      cout << "Propagators init parameters:\n\n" << Upm << endl;
      
      /* Init file writer for wf output */
      FileWF wfile;
      
      wfile.Name(_fname);
     
      wfile.Suffix(BINARY_WF_SUFFIX);
      wfile.ActivateSequence();
      wfile << Psi;
      
      
      /* The propagation loop */
      for (lint i=0; i <= clock->Steps(); i++){
	 _reporter.Analyze( Psi );      /* propagation report. */
	_U->Apply(Psi);                     /* Propagate */
	if (i % _wcycle == 0) wfile << Psi;  /* Write wavefunction */
	++(*clock);                     /* Step the clock */
      }
      
      _reporter.Finalize();
      delete Psi;
   }

}





