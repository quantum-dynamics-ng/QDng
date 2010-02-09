#ifndef ODSPACE_H
#define ODSPACE_H

#include "Operator.h"
#include "Transform.h"

namespace QDLIB
{

   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    *
    * Provides a storage for the diagonal representation of the operator (real numbers).
    *
    */
   template<typename T>
   class ODSpaceT: public Operator
   {
      protected:
         /** k-Space representation is stored here */
         Vector<T> *_dspace;
      public:
         ODSpaceT() :
            _dspace(NULL)
         {
         }

         ~ODSpaceT()
         {
         }

         /**
          * Get Kspace representation.
          *
          *
          * \return Pointer to the diagonal space rep. of the operator
          */
         Vector<T>* Dspace()
         {
            if (_dspace == NULL)
               InitDspace();
            return _dspace;
         }

         /**
          * Initialize the diagonal representation.
          */
         virtual void InitDspace() = 0;

         /**
          * Returns a pointer to the transformation object (into diag. basis)
          */
         virtual Transform* Transformation() = 0;

         /**
          * Create a exponential from the diagonal \f$ e^{c * \hat O_D}\f$.
          *
          * \param exp Buffer for exponential storage
          * \param c   factor in exponential (something like e.g. \f$-i\Delta t\f$ )
          */
         virtual void InitExponential (cVec *exp, dcomplex c)
         {
            if (_dspace == NULL) InitDspace();
            ExpElements(exp, _dspace, c);
         }

         /** We don't do offsetting. */
         virtual Operator* Offset(const double d)
         {
            return this;
         }

         virtual Operator* Scale(const double d)
         {
            if (_dspace == NULL)
               InitDspace();

            MultElements(_dspace, d);
            scaling = d;
            return this;
         }
   };

   /* Provide types for complex and real numbers */
   typedef ODSpaceT<double> ODSpace;
   typedef ODSpaceT<dcomplex> ODSpaceCplx;

} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
