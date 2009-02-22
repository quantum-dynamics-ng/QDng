#ifndef QDLIBFILTERSLIST_H
#define QDLIBFILTERSLIST_H

#include "qdlib/Operator.h"
#include "sys/XmlNode.h"

#define MAX_FILTERS 32  /* Maximum number of filters */

namespace QDLIB {

   /**
    *  This class holds a list of operators.
    * 
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class FiltersList{
      private:
	 typedef enum {expec, apply, expeconly} _FilterAction;
	 
	 lint _size;
	 bool _initalized;
	 
	 Operator* _olist[MAX_FILTERS];
	 _FilterAction _action[MAX_FILTERS];
	 
	 string _fname;
	 bool _writefile;
	 
      public:
	 FiltersList();
	 ~FiltersList();
	 
	 void Init(XmlNode* section);
	 
	 void Apply( WaveFunction *Psi );
   };
   
   }

#endif
