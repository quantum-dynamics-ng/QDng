/*
 * TransformMSD.h
 *
 *  Created on: 07.02.2010
 *      Author: markus
 */

#ifndef TRANSFORMMSD_H_
#define TRANSFORMMSD_H_

#include "Transform.h"

namespace QDLIB
{

   class TransformMSD: public QDLIB::Transform
   {
      private:
         dMat **_X;        /* Array of Matrices, one per grid point*/
         bool _adjoint;    /* Can we use the adjoint Matrix? */
         Transform **_T;     /* External Transformation, array over states */
      public:
         TransformMSD();
         virtual ~TransformMSD();

         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);

         /**
          * Set an external tranformer.
          * This is needed if the basis transformation isn't done by us. (e.g. Tkin)
          */
         void ExternalTransform(Transform **T) { _T = T; }
         
         Transform** ExternalTransform() { return _T; }
         
         /** 
          * Set Matrix Array.
          * \param adjoint Indicates, that matrix inversion is not needed.
          */
         void SetMatrixArray(dMat **X, bool adjoint = true) { _X = X; _adjoint = adjoint; }

         /** Get the transformation matrix. */
         dMat** GetMatrixArray() {return _X;}

   };

}

#endif /* TRANSFORMMSD_H_ */

