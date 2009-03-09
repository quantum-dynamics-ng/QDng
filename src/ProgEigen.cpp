#include "ProgEigen.h"

#include "sys/QDGlobalClock.h"
#include "sys/FileSingleDefs.h"
#include "sys/helpers.h"
#include "ChainLoader.h"

#include "sys/FileSingleDefs.h"
#include "sys/QDGlobalClock.h"

#include "ChainLoader.h"

namespace QDLIB
{

   ProgEigen::ProgEigen(XmlNode &EigenNode) : _EigenNode(EigenNode),
                            _U(NULL), _H(NULL), _Nef(DEFAULT_NUMBER_EFS),
			    _convergence(DEFAULT_CONVERGENCE_EF),
			    _MaxSteps(DEFAULT_MAXSTEPS), _ncycle(DEFAULT_NCYCLE),
		            _fname(DEFAULT_EF_BASE_NAME)
   {
   }


   ProgEigen::~ProgEigen()
   {
      if (_U != NULL) delete _U;
      if (_H != NULL) delete _H;
   }

   /**
    * Init the propramm parameters.
    * 
    */
   void ProgEigen::_InitParams()
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
   
      attr = _EigenNode.Attributes();
      
      /* Init the clock */
      if ( !attr.isPresent("dt") ) 
	 throw ( EParamProblem ("No Delta t given") );
      
      attr.GetValue("dt", _dt);
      
      /* Number of eigenfunctions */
      if ( attr.isPresent("Nef") ) {
	 attr.GetValue("Nef", _Nef);
	 if ( _Nef < 1)
	    throw ( EParamProblem ("Less than one eigenfunction given requested") );
      }
      
      /* Convergence */
      if ( attr.isPresent("conv") ) {
	 attr.GetValue("conv", _convergence);
	 if ( _convergence > 1)
	    throw ( EParamProblem ("Convergence critera larger than one doesn't make sense") );
      }
      
      /* Maximum number of steps */
      if ( attr.isPresent("steps") ) {
	 attr.GetValue("steps", _MaxSteps);
	 if ( _MaxSteps < 1)
	    throw ( EParamProblem ("Maximum number of time steps smaller than one requested") );
      }

      /* Init the clock */
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      /* Normalization & convergence check cycle */
      if ( attr.isPresent("ncycle") ) {
	 attr.GetValue("ncycle", _ncycle);
	 if ( _ncycle < 1)
	    throw ( EParamProblem ("Check cycle smaller than one requested") );
      }
      
      /* ef base name */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
	 if ( _fname.length() == 0)
	    throw ( EParamProblem ("Empty base name for file output defined") );
      }

      /* Init propagation output dir */
      if ( attr.isPresent("dir") && _dir.empty()) {
	 attr.GetValue("dir", _dir);
	 if (_dir[_dir.length()-1] != '/' && ! _dir.empty())
	    _dir += "/";
      }
      
      clock->Dt(_dt);
      clock->Steps(_MaxSteps);
      
      cout << "Eigenfunction calculation parameters:\n\n";
      cout << "\tNumber of steps: " <<  clock->Steps() << endl;
      cout.precision(2); cout << "\tTime step: " << fixed << clock->Dt() << endl;
      cout.precision(2); cout << "\tCheck cycles: " << _ncycle << endl;
      cout.precision(2); cout << "\tMaximum propagation  time: " 
	                      << fixed << _MaxSteps * clock->Dt() << endl;
      cout << "\tBasename for wave function output: " << _fname << endl;
      cout.precision(6); cout << scientific;
   }

   
   void ProgEigen::Run()
   {
      
      XmlNode *section;
      
      WaveFunction *Psi_old, *Psi;
      Operator *h;
      
      FileWF efile;
      
      QDClock *clock = QDGlobalClock::Instance();
      
      
      _InitParams();
      
      /* Load & Init the propagator */
      _ContentNodes = _EigenNode.NextChild();
      
      section = _ContentNodes->FindNode( "propagator" );
      if (section == NULL)
	 throw ( EParamProblem ("No propagator found") );
      
      _U = ChainLoader::LoadPropagator( section, &h );
      delete section;
      
      /* Load the initial Wavefunction */
      cout << "Initalize Wave function:\n";
      section = _ContentNodes->FindNode( "wf" );
      if (section == NULL)
	 throw ( EParamProblem ("No inital wave function found") );
      
      Psi = ChainLoader::LoadWaveFunctionChain( section );
      delete section;
      
      
      /* Make sure our hamiltonian is initalized */
      h->Init(Psi);
      cout << "Initial engergy: " << h->Expec(Psi) << endl;
      
      /* Copy, since the propagator will propably scale it/modify etc. */
      _H = h->NewInstance();
      *_H = h;
      
      /* Let the Propagator do it's initalisation */
      _U->Clock( clock );
      _H->UpdateTime();
      _U->Init(Psi);
      
      /* Report what the propagator has chosen */
      ParamContainer Upm;
    
      Upm = _U->Params();
      cout << "Propagators init parameters:\n\n" << Upm << endl;
      
      /* Init file writer for wf output */
      efile.Name(_dir+_fname);
      efile.Suffix(BINARY_WF_SUFFIX);
      efile.ActivateSequence();
      
      
      cout << "Eigenfunction\tEnergy\n";
      
      Psi_old = Psi->NewInstance();
      
      /* EF loop */
      for (int i=0; i < _Nef; i++){
	 Psi->Normalize(); 
	 /* propagation loop */
	 int s=0;
	 double diff=1;
	 while (s < _MaxSteps && diff > _convergence){
	    *Psi_old = Psi;
	    _U->Apply(Psi);
	    /* Remove lower states */
	    if ( i>0 ){
	       _P.Apply(Psi);
	    }
	    /* normalization and convergence check */
	    if (s % _ncycle == 0){
	       Psi->Normalize();
	       diff = cabs(1 - *Psi_old * Psi);
	    }
	    ++(*clock);                     /* Step the clock */
	    s++;
	 }
	 _P.Add(Psi);
	 efile << Psi;
	 cout << i << "\t" << _H->Expec(Psi) << endl;
      }
      
      /* Write energy dat */
      delete Psi_old;
   }


}


