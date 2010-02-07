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
         dMat **_X;
      public:
         TransformMSD();
         virtual ~TransformMSD();

         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);

         void SetMatrixArray(dMat **X, bool adjoint = true);

         /** Get the transformation matrix. */
         dMat** GetMatrix() {return _X;}

   };

}

#endif /* TRANSFORMMSD_H_ */
