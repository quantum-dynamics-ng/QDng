#ifndef FFT_H
#define FFT_H

#include "math/typedefs.h"
#include "qdlib/GridSystem.h"

using namespace std;



extern "C" {
#include "fftw3.h"
}

#define MAX_FFT_PLANS 8

namespace QDLIB {

   /**
    * Interface to the FFTW library.
    * 
    * Multidimensional plans are registered in a global planbuffer
    * which can take up MAX_FFT_PLANS fftw plans.
    * 
    * \todo Check if aligment of domplex array is fine (=> memalign)
    * \todo Improve the FFTW planing (FFTW_MEASURE is fine! wisdoms etc.)
    */
   class FFT {
      private:
	 typedef enum {c2c, r2c} _fft_t;
	 typedef struct {
	    GridSystem g;
	    fftw_plan planf;
	    fftw_plan planb;
	    bool oneway;
	    _fft_t type;
	 } planbuffer_t;
	 
	 bool _registered;
	 
	 fftw_plan _planf;
	 fftw_plan _planb;
	 int *_dims;
	 
	 bool _oneway;      /* Create only a forward plan */
	 _fft_t _type;      /* Type oft DFT */
	 
	 double *_rin;       /* Pointer to input data */
	 dcomplex *_cdata;   /* pointer to input data and r2c out*/
	 
	 static planbuffer_t planbuffer[MAX_FFT_PLANS];
	 static int nplans;
	       
	 void _Create_c2c(GridSystem &grid, cVec &data, bool oneway);
	 
      public:
	 FFT(cVec &data, bool oneway = false);
	 FFT(GridSystem &grid, cVec &data, bool oneway = false);
	 FFT(dVec &in, cVec &out, bool oneway = false);
	 	 	 
	 static void DestroyPlans();
	 
	 ~FFT();
	 void forward();	
	 void backward();	
      };

}/* namespace QDLIB */

#endif /* infdef FFT_H */
