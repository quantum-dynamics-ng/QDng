#include "fft.h"

namespace QDLIB {
  
   /**
   * 1D complex-to-complex FFT.
   * 
   * Meaning of in and out will be reveresed for backward transformation.
   * 
   * \param in     input
   * \param out    output
   * \param oneway If true only the forward plan will be created
   */
   FFT::FFT(cVec &in, cVec &out, bool oneway)
   {
      _planf = fftw_plan_dft_1d(in.size(), (fftw_complex*) in.begin(), (fftw_complex*) out.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_1d(in.size(), (fftw_complex*) out.begin(), (fftw_complex*) in.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
      }
      
   }
   
   /**
   * ND complex-to-complex FFT.
   * 
   * Meaning of in and out will be reveresed for backward transformation.
   * 
   * \param grid   GridSystem description
   * \param in     input
   * \param out    output
   * \param oneway If true only the forward plan will be created
   */
   FFT::FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway)
   {
      
      switch (grid.Dim()){
	 case 1:  /* 1D */
	    _planf = fftw_plan_dft_1d(in.size(), (fftw_complex*) in.begin(), (fftw_complex*) out.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_1d(in.size(), (fftw_complex*) out.begin(), (fftw_complex*) in.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
	    }
	    break;
	 case 2: /* 2D */
	    _planf = fftw_plan_dft_2d(grid.DimSizes(0) , grid.DimSizes(1), (fftw_complex*) in.begin(),
				    (fftw_complex*) out.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_2d(grid.DimSizes(0) , grid.DimSizes(1), (fftw_complex*) out.begin(),
				       (fftw_complex*) in.begin(),  FFTW_BACKWARD, FFTW_ESTIMATE);
	    }
	    break;
	 case 3: /* 3D */
	    _planf = fftw_plan_dft_3d(grid.DimSizes(0) , grid.DimSizes(1), grid.DimSizes(2),
				    (fftw_complex*) in.begin(), (fftw_complex*) out.begin(), FFTW_FORWARD, FFTW_ESTIMATE);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_3d(grid.DimSizes(0) , grid.DimSizes(1), grid.DimSizes(2),
				       (fftw_complex*) out.begin(), (fftw_complex*) in.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
	    }
	    break;
	 default: /* arb. dims */
	    _planf = fftw_plan_dft(grid.Dim(), grid.DimSizes(), (fftw_complex*) in.begin(),
				 (fftw_complex*) out.begin(), FFTW_FORWARD, FFTW_ESTIMATE);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft(grid.Dim(), grid.DimSizes(), (fftw_complex*) out.begin(),
				    (fftw_complex*) in.begin(), FFTW_FORWARD, FFTW_ESTIMATE);
	    }
	 
      } /* switch  */
      
   }
   
   
   /**
   * 1D real-to-complex FFT.
   * \todo Check if backward FFT is the same as c2r.
   * 
   * \param in     input.
   * \param out    output.
   * \param oneway If true only the forward plan will be created.
   */
   FFT::FFT(dVec &in, cVec &out, bool oneway)
   {
      _planf = fftw_plan_dft_r2c_1d(in.size(), in.begin(), (fftw_complex*) out.begin(), FFTW_ESTIMATE);	
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_c2r_1d(in.size(), (fftw_complex*) out.begin(), in.begin(), FFTW_ESTIMATE);
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

