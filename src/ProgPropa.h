#ifndef QDLIBPROGPROPA_H
#define QDLIBPROGPROPA_H

#include "sys/Exception.h"
#include "sys/XmlNode.h"
#include "sys/QDGlobalClock.h"
#include "qdlib/OPropagator.h"
#include "Reporter.h"
#include "FiltersList.h"

#define DEFAULT_WRITE_CYCLE 10
#define DEFAULT_BASENAME "WF"

#define DEFAULT_EXPEC_POST_FILENAME "expec_post.dat"
#define DEFAULT_EXPEC_PRE_FILENAME "expec_pre.dat"


namespace QDLIB {

   /**
    * This is the main module for a propagagation run.
    * 
    * XmlNode Attributes:
    * \li dt      time step, (a negative time step means backward propagagation) [required]
    * \li steps   number of time steps [required]
    * \li wcycle  number of step to make until a wf will be written and the reporter will analyze.
    * \li fname   basename for the propagated files.
    * \li dir     Output directory
    * 
    * printing options:
    * \li norm    yes/no  print norm in the report.
    * \li energy  yes/no  print the Energy in the report.
    * \li proj0   yes/no  print the projection against in the initial wave function.
    * \li proj0Abs  yes/no  print the absolute square of the projection against in the initial wave function.
    * \li spectrum   filename  if given a the projection against the inital wavefunction is turned on.
    *                          The power spectrum of the propagagation will be written.
    * 
    * @author Markus Kowalewski
    */
   class ProgPropa {
      private:
	 XmlNode &_propaNode;
	 XmlNode *_ContentNodes;
 
	 int _wcycle;     /* Write rate */
	 Reporter _reporter;
	 string _fname;
	 string _dir;
	 
	 OPropagator *_U;
	 Operator *_H;
	 
	 FiltersList _postfilter;
	 bool _usepost;
	 
	 FiltersList _prefilter;
	 bool _usepre;

	 void _InitParams();
	 void _InitSumOperator();
	 
      public:
	 ProgPropa(XmlNode &PropaNode);
	 ~ProgPropa();
	 	
	void Run();
	
   };

}

#endif
