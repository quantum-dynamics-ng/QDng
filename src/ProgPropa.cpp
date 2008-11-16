#include "ProgPropa.h"

namespace QDLIB {

   ProgPropa::ProgPropa(XmlNode & PropaNode): _propaNode(PropaNode), _wcycle(DEFAULT_WRITE_CYCLE)
   {
   }
   
   
   ProgPropa::~ProgPropa()
   {
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
    * Run the propagation program.
    */
   void ProgPropa::Run()
   {
      _InitParams();
   }

}


