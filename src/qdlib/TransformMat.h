// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBTRANSFORMMAT_H
#define QDLIBTRANSFORMMAT_H

#include "qdlib/Transform.h"

namespace QDLIB {

   /**
    * Basis Transformations via Matrix.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class TransformMat : public Transform
   {
      private:
         dMat *_X;
         bool _adjoint;
      public:
         TransformMat();
         ~TransformMat();
         /* Interface implementation */
         
         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);
         virtual double Normalization(){ return 1; }
         
         void SetMatrix(dMat *X, bool adjoint = true);
         
         /** Get the transformation matrix. */
         dMat* GetMatrix() {return _X;}
      
   };

} /* namespace QDLIB */

#endif
