#ifndef QDLIBTRANSFORMMAT_H
#define QDLIBTRANSFORMMAT_H

#include "Transform.h"

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
      
         void SetMatrix(dMat *X, bool adjoint = true);
         
         /** Get the transformation matrix.*/
         dMat* GetMatrix() {return _X;}
      
   };

} /* namespace QDLIB */

#endif
