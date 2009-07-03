#ifndef QDLIBPROGPROPA_H
#define QDLIBPROGPROPA_H

#include "tools/Exception.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"
#include "qdlib/OPropagator.h"
#include "Reporter.h"
#include "FiltersList.h"

#define DEFAULT_WRITE_CYCLE 10
#define DEFAULT_BASENAME "WF"
#define DEFAULT_NORMFILE "norm"

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
    * \li norm    true/false  print norm in the report.
    * \li energy  true/false  print the Energy in the report.
    * \li proj0   true/false  print the projection against in the initial wave function.
    * \li proj0Abs  true/false  print the absolute square of the projection against in the initial wave function.
    * \li spectrum   filename  if given a the projection against the inital wavefunction is turned on.
    *                          The power spectrum of the propagagation will be written.
    * \li nfile   Write Norm to extra file.
    * 
    * @author Markus Kowalewski
    */
   class ProgPropa {
      private:
	 XmlNode &_propaNode;
	 XmlNode *_ContentNodes;
 
	 int _wcycle;     /* Write ratio */
	 Reporter _reporter;
	 string _fname;
	 string _dir;
	 
	 string _nfile;
	 bool _writenorm;
	 
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
	 	
	void SetDirectory(string &directory) {_dir = directory;}
	
	void Run();
	
   };

}

#endif
