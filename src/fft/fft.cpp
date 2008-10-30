#include "fft.h"

namespace QDLIB {
  
/**
 * 1D complex-to-complex FFT.
 * 
 * \param N     Number of points 
 * \param in     Pointer to input
 * \param out    Pointer to output
 * \param oneway If true only the forward plan will be created
 */
FFT::FFT(size_t N, dcomplex *in, dcomplex *out, bool oneway)
{
   _planf = fftw_plan_dft_1d(N, (fftw_complex*) in, (fftw_complex*) out, FFTW_FORWARD, FFTW_ESTIMATE);
   if (!(_oneway = oneway)){
      _planb = fftw_plan_dft_1d(N, (fftw_complex*)  out, (fftw_complex*)  in, FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}


/**
 * 2D complex-to-complex FFT.
 * 
 * \param N1     Number of points x
 * \param N2     Number of points y
 * \param in     Pointer to input
 * \param out    Pointer to output
 * \param oneway If true only the forward plan will be created
 */
FFT::FFT(size_t N1, size_t N2, dcomplex *in, dcomplex *out, bool oneway)
{
   _planf = fftw_plan_dft_2d(N1, N2, (fftw_complex*) in, (fftw_complex*) out, FFTW_FORWARD, FFTW_ESTIMATE);
   if (!(_oneway = oneway)){
      _planb = fftw_plan_dft_2d(N1, N2, (fftw_complex*) out, (fftw_complex*) in,  FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}


/**
 * 3D complex-to-complex FFT.
 * 
 * \param N1      Number of points in x
 * \param N2      Number of points in y
 * \param N3      Number of points in z
 * \param in     Pointer to input
 * \param out    Pointer to output
 * \param oneway If true only the forward plan will be created
 */
FFT::FFT(size_t N1, size_t N2, size_t N3, dcomplex *in, dcomplex *out, bool oneway)
{
   _planf = fftw_plan_dft_3d(N1, N2, N3, (fftw_complex*) in, (fftw_complex*) out, FFTW_FORWARD, FFTW_ESTIMATE);	
   if (!(_oneway = oneway)){
      _planb = fftw_plan_dft_3d(N1, N2, N3, (fftw_complex*) out, (fftw_complex*) in, FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}


/**
 *  Complex-to-complex FFT of arbitrary dimensions.
 * 
 * \param rank   Number of dimensions
 * \param n      array with sizes of each dimension
 * \param in     Pointer to input
 * \param out    Pointer to output
 * \param oneway If true only the forward plan will be created
 */
FFT::FFT(int rank, const int *n, dcomplex *in, dcomplex *out, bool oneway)
{
   _planf = fftw_plan_dft(rank, n, (fftw_complex*) in, (fftw_complex*) out, FFTW_FORWARD, FFTW_ESTIMATE);	
    if (!(_oneway = oneway)){
      _planb = fftw_plan_dft(rank, n, (fftw_complex*) out, (fftw_complex*) in, FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}


/**
 * 1D real-to-complex FFT.
 * \todo Check if backward FFT is the same as c2r.
 * 
 * \param N      Number of points.
 * \param in     Pointer to input.
 * \param out    Pointer to output.
 * \param oneway If true only the forward plan will be created.
 */
FFT::FFT(size_t N, double *in, dcomplex *out, bool oneway)
{

	_planf = fftw_plan_dft_r2c_1d(N, in, (fftw_complex*)out, FFTW_ESTIMATE);	
	if (!(_oneway = oneway)){
		_planb = fftw_plan_dft_c2r_1d(N, (fftw_complex*) out, in, FFTW_ESTIMATE);
	}
}

    
/** Destructor.
 * 
 * Destroys the FFTW Plans. You have to free the pointers of
 * to the in/out memory in a any other place.
 */
FFT::~FFT()
{
	fftw_destroy_plan(_planf);
	if(!_oneway) fftw_destroy_plan(_planb); 
}

/**
 * Execute forward FFT.
 */
void FFT::forward()
{
	fftw_execute(_planf);
}


/**
 * Execute backward FFT.
 */
void FFT::backward()
{
	fftw_execute(_planb);
}


} /* namespace QDLIB */

