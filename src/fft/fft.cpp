#include "fft.h"
#include <stdio.h>

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "tools/GlobalParams.h"
#include "tools/Logger.h"

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
//       cerr << "FFTW 1D init" << endl;
      _planf = fftw_plan_dft_1d(in.size(), (fftw_complex*) in.begin(0), (fftw_complex*) out.begin(0), FFTW_FORWARD, FFTW_MEASURE);
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_1d(in.size(), (fftw_complex*) out.begin(0), (fftw_complex*) in.begin(0), FFTW_FORWARD, FFTW_MEASURE);
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
      
#ifdef _OPENMP
      _nthreads = omp_get_max_threads();
      /* Initalisation */
      
      if (fftw_init_threads() == 0)
          cerr << "FFTW init thread error" << endl;
      
      fftw_plan_with_nthreads(_nthreads);
#endif
      
      /* Read plan */
      if (!_planed){
	 Logger& log = Logger::InstanceRef();
	 ParamContainer& gp = GlobalParams::Instance();
	 string wisdom("wisdom"); /* This is the default: wisdom in the current directory */
	 bool fftwOptimize;
	 
	 if ( gp.isPresent("wisdom") )
	    gp.GetValue("wisdom", wisdom);
	 
	 gp.GetValue("fftwOptimize", fftwOptimize, true);
	 
	 /* Read wisdom */
         inbuf = in;
	 FILE * pFile;
	 if ((pFile = fopen(wisdom.c_str() , "r"))){
	    if (fftw_import_wisdom_from_file(pFile) == 1)
	       log.cout() << "Reading fftw3 wisdom from file: " << wisdom << endl;
	    else
	       log.cout() << "Reading fftw3 wisdom from file: " << wisdom << " failed!" << endl;
	    
	    fclose(pFile); 
	    fftwFlag = FFTW_MEASURE;
	 } else { 
	    if ( fftwOptimize ) { /* Optimal fftw plan */
	      fftwFlag = FFTW_PATIENT;
	      log.cout() << "No wisdom found. Creating an optimal plan. This can take a while..." << endl;
	    } else {
	       fftwFlag = FFTW_ESTIMATE;
	    }
	 }
	 log.flush();
      } else {
	 fftwFlag = FFTW_ESTIMATE;
      }
      
      
      switch (grid.Dim()){
	 case 1:  /* 1D */
	    _planf = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) in.begin(0),
				      (fftw_complex*) out.begin(0), FFTW_FORWARD, fftwFlag);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) out.begin(0),
					 (fftw_complex*) in.begin(0), FFTW_BACKWARD, fftwFlag);
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
				      (fftw_complex*) in.begin(0), (fftw_complex*) out.begin(0), FFTW_FORWARD, fftwFlag);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_3d(grid.DimSizes(2) , grid.DimSizes(1), grid.DimSizes(0),
					 (fftw_complex*) out.begin(0), (fftw_complex*) in.begin(0), FFTW_BACKWARD, fftwFlag);
	    }
	    break;
	 default: /* arb. dims */
	    _dims = new int[grid.Dim()];      /* reverse order of dimension array */
	    for (int i=0; i < grid.Dim(); i++){
	       _dims[grid.Dim() - i - 1] = grid.DimSizes(i);
	    }
	    _planf = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) in.begin(0),
				   (fftw_complex*) out.begin(0), FFTW_FORWARD, fftwFlag);	
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) out.begin(0),
				      (fftw_complex*) in.begin(0), FFTW_BACKWARD, fftwFlag);
	    }
	 
      } /* switch  */
      
      /* Save plan */
      if (!_planed) {
	 Logger& log = Logger::InstanceRef();
	 ParamContainer& gp = GlobalParams::Instance();
	 string wisdom("wisdom"); /* This is the default: wisdom in the current directory */
	 
	 if ( gp.isPresent("wisdomsave") )
	    gp.GetValue("wisdomsave", wisdom);
	 
	 in = inbuf;
	 _planed = true;
	 FILE * pFile;
	 if ((pFile = fopen(wisdom.c_str(), "w"))){
	    log.cout() << "Write fftw3 wisdom to file: " << wisdom << endl;
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
   int FFT::_nthreads = 1;
   
   
} /* namespace QDLIB */

