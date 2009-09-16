#include "TransformMat.h"
#include "tools/Exception.h"
namespace QDLIB {

   TransformMat::TransformMat()
   : Transform(), _X(NULL), _adjoint(true)
   {
   }
   
   
   TransformMat::~TransformMat()
   {
   }

   void TransformMat::Forward(WaveFunction * Psi)
   {
      MatVecMult((cVec*) Psi, _X, (cVec*) Psi);
   }
   
   
   void TransformMat::Backward(WaveFunction * Psi)
   {
   }

   /** 
    * Set the transformation matrix.
    * 
    * \param adjoint Inverse matrix equals the adjoint matrix.
    * This is the case with hermitian matrix and much faster, than a full matrix inversion
    */
   void TransformMat::SetMatrix(dMat *X, bool adjoint)
   {
      _adjoint = adjoint;
      _X = X;
      
      /* Do full inversion */
      if (! _adjoint){
         throw ( EIncompatible("Matrix inversion not implementet yet") );
      }
   }


}