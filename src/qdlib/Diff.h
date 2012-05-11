/*
 * Diff.h
 *
 *  Created on: 11.05.2012
 *      Author: markus
 */

#ifndef DIFF_H_
#define DIFF_H_

#include "qdlib/GridSystem.h"
#include "tools/ParamContainer.h"
#include "qdlib/HOFD.h"
#include "TransformFFT.h"

#define DIFF_DEF_ORDER 8  /* Default order for higher order finite differences */

namespace QDLIB
{

   /**
    * Differenciator with choosable methods.
    *
    * params:
    * \li HOFD (Higher order finite differences) order=2..8 for first and second derivatives
    * \li FFT  Fourier method, arbitrary derivatives.
    *
    */
   class Diff
   {
      private:
         GridSystem _grid;
         int _deriv;                /* Which derivative to use */
         int _dim;                  /* Prepare on dim, all if -1 */
         bool _mixed;               /* Prepare for mixed derivatives */
         enum {FFT, HOFD} _method;

         cHOFD* _hofd;              /* FD operator object */

         TransformFFT _FFT;         /* Transformation class */
         dVec _kspace1[MAX_DIMS];    /* Holds the k-values for each dim. */
         dVec _kspaceMix[MAX_DIMS][MAX_DIMS]; /* Holds the k-values for the mixed derivatives. */


         void _InitKspace();
      public:
         Diff();
         Diff(int derivative, bool mixed = false);
         virtual ~Diff();


         void InitParams(ParamContainer pm);

         void SetGrid(GridSystem &grid) { _grid = grid; }
         const GridSystem& GetGrid() { return _grid; }

         void Derivative(int deriv) { _deriv = deriv; }
         int Derivative() { return _deriv; }

         /**
          * Prepare for mixed derivatives.
          */
         void Mixed(bool mixed) { _mixed = mixed; }
         bool Mixed() { return _mixed; }

         /**
          * Prepare for specific dim.
          * If set to -1. All dims are prepared.
          */
         void Dim(int dim) { _dim = dim; }
         int Dim() { return _dim; }

         void DnDxn(WaveFunction* out, WaveFunction* in, int dim, double d = 1);
         void DnDxnAdd(WaveFunction* out, WaveFunction* in, int dim, double d = 1);
         void DxDy(WaveFunction* out, WaveFunction* in, int dim1, int dim2, double d = 1);
   };
} /* namespace QDLIB */
#endif /* DIFF_H_ */
