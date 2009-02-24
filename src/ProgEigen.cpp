#include "ProgEigen.h"

namespace QDLIB
{

   ProgEigen::ProgEigen() : _U(NULL), _H(NULL), _Nef(DEFAULT_NUMBER_EFS),
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

      clock->Dt(dt);
      clock->Steps(steps);
      
      /* Number of eigenfunctions */
      if ( attr.isPresent("Nef") ) {
	 attr.GetValue("Nef", _Nef);
	 if ( steps < 1)
	    throw ( EParamProblem ("Less than one eigenfunction given requested") );
      }
      
      /* Convergence */
      if ( attr.isPresent("conv") ) {
	 attr.GetValue("conv", _convergence);
	 if ( steps > 1)
	    throw ( EParamProblem ("Convergence critera larger than one doesn't make sense") );
      }
      
      /* Maximum number of steps */
      if ( attr.isPresent("steps") ) {
	 attr.GetValue("steps", _MaxSteps);
	 if ( steps < 1)
	    throw ( EParamProblem ("Maximum number of time steps smaller than one requested") );
      }

      /* Normalization & convergence check cycle */
      if ( attr.isPresent("ncycle") ) {
	 attr.GetValue("ncycle", _ncycle);
	 if ( steps < 1)
	    throw ( EParamProblem ("Check cycle smaller than one requested") );
      }
      
      /* ef base name */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
	 if ( trim(_fname).length() == 0)
	    throw ( EParamProblem ("Empty base name for file output defined") );
      }

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
      /* Load propagator */
      
      /* Load intial WF*/
      
      
      /* Calc loop */
      
      /* Write energy dat */
   }


}


