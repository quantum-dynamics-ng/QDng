// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef ODSPACE_H
#define ODSPACE_H

#include "qdlib/Operator.h"
#include "qdlib/Transform.h"

namespace QDLIB
{

   /**
    * Abstract class for Operator which can be transformed into a diagonal representation.
    *
    * Provides a storage for the diagonal representation of the operator (real numbers).
    *
    */
   template<typename T>
   class ODSpaceT: public virtual Operator
   {
      protected:
         /** k-Space representation is stored here */
         Vector<T> *_dspace;
      public:
         ODSpaceT() :
            Operator(), _dspace(NULL)
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

   };

   /* Provide types for complex and real numbers */
   typedef ODSpaceT<double> ODSpace;
   typedef ODSpaceT<dcomplex> ODSpaceCplx;

} /* namespace QDLIB */

#endif /* #ifndef ODiag_H */
