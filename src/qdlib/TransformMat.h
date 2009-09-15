#ifndef QDLIBTRANSFORMMAT_H
#define QDLIBTRANSFORMMAT_H

#include <Transform.h>

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
      public:
         TransformMat();
         ~TransformMat();
         /* Interface implementation */
         
         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);
      
         /** 
          * Set the transformation matrix.
          * 
          * \param adjoint Inverse matrix equals the adjoint matrix.
          * This is the case with hermitian matrix and much faster, than a full matrix inversion
          */
         void SetMatrix(dMat *, bool adjoint = true);
         
         /** Get the transformation matrix.*/
         dMat* GetMatrix() {return _X;}
      
   };

} /* namespace QDLIB */

#endif
