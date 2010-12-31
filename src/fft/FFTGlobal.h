/*
 * fftglobal.h
 *
 *  Created on: 29.12.2010
 *      Author: markus
 */

#ifndef FFTGLOBAL_H_
#define FFTGLOBAL_H_

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "tools/GlobalParams.h"

namespace QDLIB
{
   /**
    * Handles global fftw issues as a singleton.
    */
   class FFTGlobal {
      private:
#ifdef _OPENMP
         int _nthreads;
#endif
         bool _fftwOptimize;
         int _fftwFlag;
      public:
         FFTGlobal();
         ~FFTGlobal();

         static FFTGlobal& Instance();

         /** Tell fftw Optimizer flag. */
         int OptimizeFlag() { return _fftwFlag; }

         void FlushWisdom();
#ifdef _OPENMP
         /** Returns the max usable fft threads. */
         int Threads () { return _nthreads; }
#endif
   };

} /* namespace QDLIB */

#endif /* FFTGLOBAL_H_ */
