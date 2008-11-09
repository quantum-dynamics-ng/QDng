#ifndef OKSPACE_H
#define OKSPACE_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    * 
    * Provides a storage for the diagonal representation of the operator (real numbers).
    * 
    * \todo add support for complex k-space operators.
    */
   class OKSpace: public Operator
   {
      protected:
         /** k-Space representation is stored here */
         dVec *_kspace;
      public:
         OKSpace() : _kspace(NULL) {}
         
         ~OKSpace()
         {
            if (_kspace != NULL) delete _kspace;
         }
         
         /**
          * Get Kspace representation.
          * 
          * 
          * \return Pointer to the diagonal space rep. of the operator
          */
         dVec* Kspace()
         {
            if (_kspace == NULL) InitKspace();
            return _kspace;
         }
         
         /**
          * Initialize the diagonal respresentation.
          */
         virtual void InitKspace() = 0;
   };
   
} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
