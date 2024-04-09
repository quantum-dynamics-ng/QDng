// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBTRANSFORMFFT_H
#define QDLIBTRANSFORMFFT_H

#include "qdlib/Transform.h"

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
         double _norm;
      public:
         TransformFFT();
         ~TransformFFT();
         
         virtual void Forward(WaveFunction *Psi);
         virtual void Backward(WaveFunction *Psi);
         
         virtual double Normalization(){ return _norm; }

         void Forward(WaveFunction *Psi, int dim);
         void Backward(WaveFunction *Psi, int dim);
   };

} /* namespace QDLIB */

#endif
