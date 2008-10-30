#ifndef OPERATOR_H
#define OPERATOR_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * Exponential operator for KSpace operator classes and Gridsystems.
    */
   class OGridExp : public Operator
   {
      public:
         void ExpFactor(double d);
         void ExpFactor(dcomplex d);
	 
	 
   };
   
   
   
} /* namespace QDLIB */

#endif /* #ifndef OPERATOR_H */