#ifndef QDLIBFILTERSLIST_H
#define QDLIBFILTERSLIST_H

#include "qdlib/Operator.h"
#include "tools/XmlNode.h"
#include "tools/QDGlobalClock.h"

#define MAX_FILTERS 32           /* Maximum number of filters */
#define DEFAULT_EXPEC_FILENAME "expec.dat"  /* Default filename for expectation values */

namespace QDLIB {

   /**
    *  This class holds a list of operators.
    * 
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class FiltersList{
      private:
	 typedef enum {expec, apply, expeconly, normalize} _FilterAction;
	 
	 lint _size;
	 bool _initalized;
	 
	 Operator* _olist[MAX_FILTERS];
	 _FilterAction _action[MAX_FILTERS];
	 
	 string _fname;
	 bool _writefile;
	 
	 ofstream _ofile;
	       
	 QDClock *_clock;
	 
	 void _destroy();
      public:
	 FiltersList();
	 ~FiltersList();
	 
	 void SetDefaultName(string &s);
	 
	 void Init(XmlNode* section);
	 
	 void Apply( WaveFunction *Psi );
   };
   
}

#endif
