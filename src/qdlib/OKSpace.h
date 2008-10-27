#ifndef OKSPACE_H
#define OKSPACE_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    * 
    * Provides a storage for the diagonal representation of the operator (real numbers).
    * 
    */
   class OKSpace: public Operator
   {
      protected:
         /** diagonal form is stored here */
         dVec *_kspace;
         /** We have to take of the dspace pointer => true if *_dspace left the class*/
         bool _khandover;
      public:
         OKSpace() : _kspace(NULL),  _khandover(false) {}
         
         ~OKSpace()
         {
            if ( !_khandover && _kspace != NULL) delete _kspace;
         }
         
         /**
          * Get Kspace representation.
          * 
          * If you use this function you have to take care of the pointer by yourself.
          * At this moment the this class won't delete the pointer anymore.
          * 
          * \return Pointer to the diagonal space rep. of the operator
          */
         dVec* Kspace()
         {
            if (_kspace == NULL) InitKspace();
            _khandover = true;
            return _kspace;
         }
         
         /**
          * Initialize the diagonal respresentation.
          */
         virtual void InitKspace() = 0;
   };
   
} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
