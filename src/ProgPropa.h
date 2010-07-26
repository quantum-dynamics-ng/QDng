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
#define DEFAULT_EXPEC_INIT_FILENAME "expec_init.dat"

namespace QDLIB {

   /**
    * This is the main module for a propagagation run.
    * 
    * XmlNode Attributes:
    * \li dt      time step, (a negative time step means backward propagagation) [required]
    * \li steps   number of time steps [required]
    * \li wcycle  number of step to make until a wf will be written and the reporter will analyze.
    * \li fname   basename for the propagated files.
    * \li start   File Sequence number to begin with. If > 0 then the initial WF will not be written.
    * \li dir     Output directory
    * 
    * printing options:
    * \li norm    true/false  print norm in the report. [true]
    * \li energy  true/false  print the Energy in the report. [true]
    * \li proj0   true/false  print the projection against in the initial wave function.
    * \li proj0Abs  true/false  print the absolute square of the projection against in the initial wave function.
    * \li spectrum   filename  if given a the projection against the inital wavefunction is turned on.
    *                          The power spectrum of the propagagation will be written.
    * \li nfile   Write Norm to extra file.
    * \li scinorm   Print norm in scientific format instead of fixed point values
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
	 int    _start;   /* Start writing at number */
	 string _dir;
	 
	 string _nfile;
	 bool _writenorm;
         
         bool _scinorm;
	 
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
