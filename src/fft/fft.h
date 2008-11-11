#ifndef FFT_H
#define FFT_H

#include "math/dcomplex.h"


using namespace std;



extern "C" {
#include "fftw3.h"
}


namespace QDLIB {

   /**
    * Raw interface to the FFTW library.
    * 
    * \todo Check if aligment of domplex array is fine (=> memalign)
    * \todo Check if the FFTW plan types are good & performant (FFTW_MEASURE is fine!)
    */
   class FFT {
      private:
	 fftw_plan _planf;
	 fftw_plan _planb;
	 bool _oneway;
      public:
	 FFT(size_t N, dcomplex *in, dcomplex *out, bool oneway = false);
	 FFT(size_t N1, size_t N2, dcomplex *in, dcomplex *out, bool oneway = false);
	 FFT(size_t N1, size_t N2, size_t N3, dcomplex *in, dcomplex *out, bool oneway = false);
	 FFT(int rank, const int *n, dcomplex *in, dcomplex *out, bool oneway = false);
	 FFT(size_t N, double *in, dcomplex *out, bool oneway = false);
	 
	 ~FFT();
	 void forward();	
	 void backward();	
      };




}/* namespace QDLIB */

#endif /* infdef FFT_H */
