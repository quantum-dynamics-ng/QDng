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
    * \todo Improve the FFTW planing (FFTW_MEASURE is fine! wisdoms etc.)
    */
   class FFT {
      private:
	 fftw_plan _planf;
	 fftw_plan _planb;
	 int *_dims;
	 bool _oneway;
	 static bool _planed;
	 static int nthreads;
      public:
	 FFT(cVec &in, cVec &out, bool oneway = false);
	 FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway = false);
	 FFT(dVec &in, cVec &out, bool oneway = false);
	 	 
	 ~FFT();
	 void forward();	
	 void backward();	
      };




}/* namespace QDLIB */

#endif /* infdef FFT_H */
