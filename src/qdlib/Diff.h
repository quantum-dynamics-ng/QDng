// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

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
    * \li HOFD (Higher order finite differences) order=2..50 for first and second derivatives
    * \li FFT  Fourier method, arbitrary derivatives.
    *
    */
   class Diff
   {
      public:
         typedef enum {FFT, HOFD} method_t;
      private:
         GridSystem _grid;
         int _deriv;                /* Which derivative to use */
         int _dim;                  /* Prepare on dim, all if -1 */
         bool _mixed;               /* Prepare for mixed derivatives */
         bool _single;
         bool _coll;
         method_t _method;
         dVec _pfac;

         cHOFD* _hofd;              /* FD operator object */

         TransformFFT _FFT;         /* Transformation class */
         dVec _kspace1[MAX_DIMS];    /* Holds the k-values for each dim. */
         dVec _kspaceMix[MAX_DIMS][MAX_DIMS]; /* Holds the k-values for the mixed derivatives. */

         void MultiplyKspace(cVec* data, int dim1, int dim2);
         void MultiplyKspace(cVec* data, int dim);
         void MultiplyKspace(cVec* data);

      protected:
         void _InitKspace();
         dVec _kspace;    /* Holds the k-values for all dims */
      public:
         Diff();
         Diff(int derivative, bool mixed = false);
         virtual ~Diff();

         Transform* GetTransform();

         void InitParams(ParamContainer pm);

         void SetGrid(GridSystem &grid) { _grid = grid; }
         const GridSystem& GetGrid() { return _grid; }

         /**
          * Set derivative.
          */
         void Derivative(int deriv) { _deriv = deriv; }
         int Derivative() { return _deriv; }

         /**
          * Prepare for mixed derivatives.
          */
         void Mixed(bool mixed) { _mixed = mixed; }
         bool Mixed() { return _mixed; }

         /**
          * Prepare single dim derivatives.
          */
         void Single(bool single) { _single = single; }
         bool Single() { return _single; }

         /**
          * Prepare All dim derivatives.
          */
         void Collective(bool coll) { _coll = coll; }
         bool Collective() { return _coll; }

         /**
          * Set Dimension specific prefactors.
          */
         void Factors(dVec &factors)
         {
            _pfac = factors;
         }

         /**
          * Choose the method for diff.
          */
         void Method(method_t method) { _method = method; }
         method_t Method() { return _method; }

         /**
          * Prepare for specific dim.
          * If set to -1. All dims are prepared.
          */
         void Dim(int dim) { _dim = dim; }
         int Dim() { return _dim; }

         void DnDxn(WaveFunction* out, WaveFunction* in, double d = 1);
         void DnDxn(WaveFunction* out, WaveFunction* in, int dim, double d = 1);
         void DnDxnAdd(WaveFunction* out, WaveFunction* in, int dim, double d = 1);
         void DxDy(WaveFunction* out, WaveFunction* in, int dim1, int dim2, double d = 1);
   };

} /* namespace QDLIB */
#endif /* DIFF_H_ */
