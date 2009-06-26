#ifndef QDLIBPROGOCT_H
#define QDLIBPROGOCT_H

#include "tools/Exception.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"
#include "qdlib/OPropagator.h"


#define DEFAULT_BASENAME_LASER "laser"

namespace QDLIB {

   /**
   * 
   * Main module for optimal control theory.
   * 
   * Multi-target mode is supported.
   * 
   * XmlNode Attributes:
   * \li dt       time step
   * \li steps    number of time steps
   * \li dir      Output directory
   * \li method   OCT method to use (krotov, rabiz)
   * \li coup     The type of coupling to optimize (dipole)
   * \li ttype    target type (operator,overlap)
   * \li phase    phase sensitive objective (true|false, default false)
   * \li ntargets Number of targets
   * \li alpha    The Field restriction value
   * 
   * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class ProgOCT{
      private:
	 typedef enum {krotov, rabitz} _method_t;
	 typedef enum {dipole} _coupling_t;
	 typedef enum {op, ov} _ttype_t;
	 
	 XmlNode &_octNode;
	 XmlNode *_ContentNodes; 
	 	 
	 string _fname;
	 string _dir;
	 
	 _method_t _method;
	 _coupling_t _coupling;
	 _ttype_t _ttype;
	 bool _phase;
	 
	 int _ntargets;
	 double _alpha;
	 
	 void _InitParams();
	 
      protected:
	 void CalcLaserField();
	 
      public:
	 ProgOCT(XmlNode &OCTNode);
	 ~ProgOCT();

	 void SetDirectory(string &directory) {_dir = directory;}
	    
	 void Run();

   };

}

#endif
