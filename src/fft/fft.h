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
    * fftw relies on aligned memory for accelleration.
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
         cVec *_out;

         void _initplans();
         void _CreatePlanDim(int dim);
         int _lothers(int dim) const;
         int _uothers(int cdim) const;
      public:
         FFT(cVec &in, cVec &out, bool oneway = false);
         FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway = false);
         FFT(dVec &in, cVec &out, bool oneway = false);

         ~FFT();
         void forward();
         void forward(int dim);
         void backward();
         void backward(int dim);
      };




}/* namespace QDLIB */

#endif /* infdef FFT_H */
