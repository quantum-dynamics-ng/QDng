#ifndef QDLIBTRANSFORMFFT_H
#define QDLIBTRANSFORMFFT_H

#include "Transform.h"

namespace QDLIB
{

   /**
   * FFT transform for wave functions
   * 
   * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class TransformFFT : public Transform
   {
      private:
      public:
         TransformFFT();
         ~TransformFFT();
         
         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);
         
         
   };

} /* namespace QDLIB */

#endif