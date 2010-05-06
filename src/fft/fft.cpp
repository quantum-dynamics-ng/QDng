#include "fft.h"
#include <stdio.h>

#ifdef _OPENMP
 #include <omp.h>
#endif

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
   FFT::FFT(cVec &in, cVec &out, bool oneway) : _dims(NULL)
   {
      cerr << "FFTW 1D init" << endl;
      _planf = fftw_plan_dft_1d(in.size(), (fftw_complex*) in.begin(0), (fftw_complex*) out.begin(0), FFTW_FORWARD, FFTW_ESTIMATE);
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_1d(in.size(), (fftw_complex*) out.begin(0), (fftw_complex*) in.begin(0), FFTW_FORWARD, FFTW_ESTIMATE);
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
   FFT::FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway) : _dims(NULL)
   {
      cVec inbuf;
      int fftwFlag;
      
      if (!_planed){
         inbuf = in;
	 FILE * pFile;
	 if ((pFile = fopen("wisdom", "r"))){
	    fftw_import_wisdom_from_file(pFile);
	    fclose(pFile);
	 }
	 fftwFlag = FFTW_PATIENT;
#ifdef _OPENMP
	 nthreads = omp_get_num_threads();
	 //cerr << "FFTW run init" << endl;
	 /* Initalisation */
	 if (fftw_init_threads() == 0)
	    cerr << "FFTW init thread error" << endl;
	 else
	    cerr << "FFTW init thread success " << nthreads << endl;
 	 fftw_plan_with_nthreads(nthreads);
#endif	 
      } else {
	 fftwFlag = FFTW_ESTIMATE;
      }
#ifdef _OPENMP
      nthreads = omp_get_num_threads();
       fftw_plan_with_nthreads(nthreads);
#endif
      switch (grid.Dim()){
	 case 1:  /* 1D */
	    _planf = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) in.begin(0),
				      (fftw_complex*) out.begin(0), FFTW_FORWARD, FFTW_ESTIMATE);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) out.begin(0),
					 (fftw_complex*) in.begin(0), FFTW_BACKWARD, FFTW_ESTIMATE);
	    }
	    break;
	 case 2: /* 2D */
	    _planf = fftw_plan_dft_2d(grid.DimSizes(1) , grid.DimSizes(0), (fftw_complex*) in.begin(0),
				      (fftw_complex*) out.begin(0), FFTW_FORWARD, fftwFlag);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_2d(grid.DimSizes(1) , grid.DimSizes(0), (fftw_complex*) out.begin(0),
					 (fftw_complex*) in.begin(0),  FFTW_BACKWARD, fftwFlag);
	    }
	    break;
	 case 3: /* 3D */
	    _planf = fftw_plan_dft_3d(grid.DimSizes(2) , grid.DimSizes(1), grid.DimSizes(0),
				      (fftw_complex*) in.begin(0), (fftw_complex*) out.begin(0), FFTW_FORWARD, FFTW_MEASURE);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_3d(grid.DimSizes(2) , grid.DimSizes(1), grid.DimSizes(0),
					 (fftw_complex*) out.begin(0), (fftw_complex*) in.begin(0), FFTW_BACKWARD, FFTW_MEASURE);
	    }
	    break;
	 default: /* arb. dims */
	    _dims = new int[grid.Dim()];      /* reverse order of dimension array */
	    for (int i=0; i < grid.Dim(); i++){
	       _dims[grid.Dim() - i - 1] = grid.DimSizes(i);
	    }
	    _planf = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) in.begin(0),
				   (fftw_complex*) out.begin(0), FFTW_FORWARD, FFTW_MEASURE);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) out.begin(0),
				      (fftw_complex*) in.begin(0), FFTW_BACKWARD, FFTW_MEASURE);
	    }
	 
      } /* switch  */
      
      if (!_planed) {
	 in = inbuf;
	 _planed = true;
	 FILE * pFile;
	 if ((pFile = fopen("wisdom", "w"))){
	    fftw_export_wisdom_to_file(pFile);
	    fclose(pFile);
	 }
	 
      }
      
   }
   
   
   /**
   * 1D real-to-complex FFT.
   * 
   * \param in     input.
   * \param out    output. Note that the outputsize must be n/2+1.
   * \param oneway If true only the forward plan will be created.
   */
   FFT::FFT(dVec &in, cVec &out, bool oneway) : _dims(NULL)
   {
      _planf = fftw_plan_dft_r2c_1d(in.size(), in.begin(0), (fftw_complex*) out.begin(0), FFTW_ESTIMATE);	
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_c2r_1d(in.size(), (fftw_complex*) out.begin(0), in.begin(0), FFTW_ESTIMATE);
      }
   }
      
   /** Destructor.
   * 
   * Destroys the FFTW Plans. You have to free the pointers of
   * to the in/out memory in a any other place.
   */
   FFT::~FFT()
   {
         if (_dims != NULL) delete _dims;
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

   bool FFT::_planed = false;
   int FFT::nthreads = 0;
   
   
} /* namespace QDLIB */

