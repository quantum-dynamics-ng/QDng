// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef FFT_H
#define FFT_H

#include "math/typedefs.h"
#include "qdlib/GridSystem.h"

using namespace std;



extern "C" {
#include "fftw3.h"
}


namespace QDLIB {

   /**
    * Interface to the FFTW library.
    * 
    * Note that you have use the Align method of the vector class.
    * fftw relies on aligned memory for acceleration.
    * 
    */
   class FFT {
      private:
         fftw_plan _planf[MAX_DIMS + 1]; /* Holds forward plans for full fft + n single dim ffts */
         fftw_plan _planb[MAX_DIMS + 1]; /* Holds forward plans for full fft + n single dim ffts */
         int *_dims; /* Dim sizes */
         int *_rdims; /* Dim sizes. reveresed, C-Order*/
         int _ndims; /* number of dims */
         bool _oneway;
         cVec *_in;
         enum {c2c, r2c, r2r} _type;
         dcomplex *_cin;
         double *_din;
         dcomplex *_cout;
         double *_dout;


         void _initplans();
         void _CreatePlanDim(int dim);
         int _lothers(int dim) const;
         int _uothers(int cdim) const;
      public:
         FFT(cVec &in, cVec &out, bool oneway = false);
         FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway = false);
         FFT(dVec &in, cVec &out, bool oneway = false);

         ~FFT();

         void ReplaceBuffers(cVec *in, cVec *out);
         void ReplaceBuffers(dVec *in, cVec *out);

         void forward(int dim = -1);
         void backward(int dim = -1);
      };




}/* namespace QDLIB */

#endif /* infdef FFT_H */
