#include "ProgOCT.h"

#include "tools/FileSingleDefs.h"
#include "tools/Logger.h"

namespace QDLIB {

   ProgOCT::ProgOCT(XmlNode &OCTNode) : _octNode(OCTNode), _ContentNodes(NULL),
   _fname(DEFAULT_BASENAME_LASER), _dir(""),
   _method(krotov), _coupling(dipole), _ttype(ov), _phase(false),  _ntargets(1), _alpha(1)
   {
   }
   
   
   ProgOCT::~ProgOCT()
   {
   }

   /**
    * Initalize the global parameters.
    */
   void QDLIB::ProgOCT::_InitParams( )
   {
      QDClock *clock = QDGlobalClock::Instance();  /* use the global clock */
      ParamContainer attr;
       Logger& log = Logger::InstanceRef();
      
      attr = _octNode.Attributes();
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
      
      /* Init OCT output dir */
      if ( attr.isPresent("dir") && _dir.empty() ) {
	 attr.GetValue("dir", _dir);
	 if (_dir[_dir.length()-1] != '/' && ! _dir.empty())
	    _dir += "/";
      }
      
      /* Init propagation file basename */
      if ( attr.isPresent("fname") ) {
	 attr.GetValue("fname", _fname);
      }
      
      /* Optimization method to use */
      if (attr.isPresent("method")){
	 attr.GetValue("method", s);
	 if (s == "krotov")
	    _method = krotov;
	 else if (s == "rabitz")
	    _method = rabitz;
	 else
	    throw(EParamProblem("Unknown OCT method", s));

      }
      
      /* Type of coupling to use */
      if (attr.isPresent("coup")){
	 attr.GetValue("coup", s);
	 if (s == "dipole")
	    _coupling = dipole;
	 else
	    throw(EParamProblem("Unsupported coupling specified", s));	 
      }

      /* Target type to use */
      if (attr.isPresent("ttype")){
	 attr.GetValue("ttype", s);
	 if (s == "operator")
	    _ttype = op;
	 else if (s == "overlap")
	    _ttype = ov;
	 else
	    throw(EParamProblem("Unknown OCT target type", s));
      }
      
      /* Check for phase sensitive oct*/
      attr.GetValue("phase", _phase);
       
      /* Target type to use */
      if (attr.isPresent("ntargets")){
	 attr.GetValue("ntargets", _ntargets);
	 if (_ntargets < 1)
	    throw (EParamProblem("Number of targets to small", _ntargets));
      }

      if (attr.isPresent("alpha")){
	 attr.GetValue("alpha", _alpha);
      }
      
      /* Get alpha */
      if (abs(_alpha) == 0)
	 throw (EParamProblem("Invalid alpha parameter", _alpha));
      
      
      /* Print parameter summary */
      log.Header("OCT parameters", Logger::Section);
    
      log.IndentInc();
      log.cout() << "Number of steps: " <<  clock->Steps() << endl;
      log.cout().precision(2);
      log.cout() << "Time step: " << fixed << clock->Dt() << endl;
      log.cout() << "Overall time: " << fixed << clock->Steps() * clock->Dt() << endl;
      log.cout() << "Basename for wave function output: " << _fname << endl;
      log.cout() << endl;
      
      log.cout() << "OCT - Style: ";
      switch(_method){
	 case krotov:
	    log.cout() << "Krotov";
	 break;
	 case rabitz:
	    log.cout() << "Rabitz";
	 break;
      }
      
      switch(_ttype){
         case ov:
	    log.cout() << " - Overlapp";
	    break;
	 case op:
	    log.cout() << " - Excpectation values";
	    break;
      }
      
      if (_phase)
	 log.cout() << " - phase sensitive" << endl;
      else 
	 log.cout() << endl;
      
      log.cout().precision(3);
      log.cout() << "alpha : " << _alpha << endl;
      log.cout() << "Number of targets: " << _ntargets << endl;
      
      switch (_coupling){
	 case dipole:
	    log.cout() << "Optimize laserfield for dipole coupling" << endl;
      }
      
      log.flush();
   }
  
   /**
    * Run the OCT-Programm
    */
   void ProgOCT::Run()
   {
      _InitParams( );
   }
}
