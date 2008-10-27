#ifndef GRIDOPERATOR_H
#define GRIDOPERATOR_H

#include "Operator.h"



namespace QDLIB {

   /**
    * Class which represents simple numerical Operators.
    * 
    * Base class for potentials, dipole operators etc.
    * The template parameter is meant to be cVec or dVec.
    */
   template <class C>
   class GridOperator: public Operator, public C
   {
      
   };

} /* namespace QDLIB */

#endif /* GRIDOPERATOR_H */
