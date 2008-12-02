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
    * \todo Check if aligment of domplex array is fine (=> memalign)
    * \todo Improve the FFTW planing (FFTW_MEASURE is fine! wisdoms etc.)
    */
   class FFT {
      private:
	 fftw_plan _planf;
	 fftw_plan _planb;
	 int *_dims;
	 bool _oneway;
	 static bool _planed;
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
