#ifndef ODiag_H
#define ODiag_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    * 
    * Provides a storage for the diagonal representation of the operator (real numbers).
    * 
    */
   class ODiag: public Operator
   {
      protected:
         /** diagonal form is stored here */
         dVec *_dspace;
         /** We have to take of the dspace pointer => true if *_dspace left the class*/
         bool _dhandover;
      public:
         ODiag() : _dspace(NULL),  _dhandover(false) {}
         
         ~ODiag()
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
         dVec* Dspace()
         {
            if (_kspace == NULL) InitDspace();
            _dhandover = true;
            return _dspace;
         }
         
         /**
          * Initialize the diagonal respresentation.
          */
         virtual void InitDspace() = 0;
   };
   
} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
