#ifndef QDLIBFILTERSLIST_H
#define QDLIBFILTERSLIST_H

#include <iostream>
#include <fstream>

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
	 typedef enum {real, imag, complex} _FilterValue;
	 
	 lint _size;
	 bool _initalized;
	 
	 Operator* _olist[MAX_FILTERS];          /* Operator list */
	 _FilterAction _action[MAX_FILTERS];     /* action for corresponding operator */
	 _FilterValue _value[MAX_FILTERS];       /* action for corresponding operator */
         dcomplex _sum[MAX_FILTERS];             /* Sum for integretion */
         bool _integrate[MAX_FILTERS];           /* Turn on integration */
         string _labels[MAX_FILTERS];            /* user defined labels for expec table  */
	 
	 string _fname;
	 bool _writefile;                        /* write to file or stdout */
	 
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
