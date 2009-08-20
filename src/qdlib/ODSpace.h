#ifndef ODSPACE_H
#define ODSPACE_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    * 
    * Provides a storage for the diagonal representation of the operator (real numbers).
    * 
    * \todo add support for complex k-space operators.
    */
   template<typename T>
   class ODSpaceT: public Operator
   {
      protected:
         /** k-Space representation is stored here */
         Vector<T> *_dspace;
      public:
	 ODSpaceT() : _dspace(NULL) {}
         
	 ~ODSpaceT()
         {
            if (_dspace != NULL) delete _dspace;
         }
         
         /**
          * Get Kspace representation.
          * 
          * 
          * \return Pointer to the diagonal space rep. of the operator
          */
	 Vector<T>* Dspace()
         {
            if (_dspace == NULL) InitDspace();
            return _dspace;
         }
         
         /**
          * Initialize the diagonal respresentation.
          */
         virtual void InitDspace() = 0;
	 
	 /** We don't do offsetting. */
	 virtual Operator* Offset(const double d){return this;}

	 virtual Operator* Scale(const double d)
	 {
	    if (_dspace == NULL)
	       throw ( EParamProblem("k-space not initialized", Name()) );
	    MultElements(_dspace, d);
	    scaling=d;
	    return this;
	 }
	 
   };
   
   /* Provide types for complex and real numbers */
   typedef ODSpaceT<double> ODSpace;
   typedef ODSpaceT<dcomplex> ODSpaceCplx;
   
} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
