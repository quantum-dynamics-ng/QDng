#include "fft.h"

namespace QDLIB {
  
   /**
    * Create a complex to complex FFTW plan
    */
   void FFT::_Create_c2c(GridSystem &grid, cVec &data, bool oneway)
   {
      cVec inbuf;
      bool found = false;
      
      inbuf = data; /* Copy for FFT-Measure */
      _type = c2c;
      
      _cdata = data.begin(0);
      
      _oneway = oneway;
      
      /* Check if a plan exists */
      for (int i=0; i < nplans; i++)
      {
	 if (grid == planbuffer[i].g && planbuffer[i].oneway == oneway && planbuffer[i].type == _type){
	    _planf = planbuffer[i].planf;
	    _planb = planbuffer[i].planb;
	    _registered = true;
	    return;
	 }
      }
      	 
      switch (grid.Dim()){
	 case 1:  /* 1D */
	    _planf = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) _cdata,
				      (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_ESTIMATE);
	    if (!(_oneway = oneway)){
	       _planb = fftw_plan_dft_1d(grid.DimSizes(0), (fftw_complex*) _cdata,
					 (fftw_complex*) _cdata, FFTW_BACKWARD, FFTW_ESTIMATE);
	    }
	    break;
	    case 2: /* 2D */
	       _planf = fftw_plan_dft_2d(grid.DimSizes(1) , grid.DimSizes(0), (fftw_complex*) _cdata,
					 (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_MEASURE);
	       if (!(_oneway = oneway)){
		  _planb = fftw_plan_dft_2d(grid.DimSizes(1) , grid.DimSizes(0), (fftw_complex*) _cdata,
					    (fftw_complex*) _cdata,  FFTW_BACKWARD, FFTW_MEASURE);
	       }
	       break;
	       case 3: /* 3D */
		  _planf = fftw_plan_dft_3d(grid.DimSizes(2) , grid.DimSizes(1), grid.DimSizes(0),
					    (fftw_complex*) _cdata, (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_MEASURE);	
		  if (!(_oneway = oneway)){
		     _planb = fftw_plan_dft_3d(grid.DimSizes(2) , grid.DimSizes(1), grid.DimSizes(0),
					       (fftw_complex*) _cdata, (fftw_complex*) _cdata, FFTW_BACKWARD, FFTW_MEASURE);
		  }
		  break;
		  default: /* arb. dims */
		     _dims = new int[grid.Dim()];      /* reverse order of dimension array */
		     for (int i=0; i < grid.Dim(); i++){
			_dims[grid.Dim() - i - 1] = grid.DimSizes(i);
		     }
		     _planf = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) _cdata,
					    (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_MEASURE);	
		     if (!(_oneway = oneway)){
			_planb = fftw_plan_dft(grid.Dim(), _dims, (fftw_complex*) _cdata,
					       (fftw_complex*) _cdata, FFTW_BACKWARD, FFTW_MEASURE);
		     }
	 
      } /* switch  */
      
      /* Register new plan - nothing has been found */
      if (nplans < MAX_FFT_PLANS){
	 planbuffer[nplans].g = grid;
	 planbuffer[nplans].oneway = oneway;
	 planbuffer[nplans].planf = _planf;
	 planbuffer[nplans].planb = _planb;
	 planbuffer[nplans].type = _type;
	 nplans++;
	 _registered = true;
      }

   }
   
   void _Create_r2c()
   {
   }
   
   /**
   * 1D complex-to-complex FFT.
   * 
   * Meaning of in and out will be reveresed for backward transformation.
   * 
   * \param data     input/output
   * \param oneway If true only the forward plan will be created
   */
   FFT::FFT(cVec &data, bool oneway) : _dims(NULL), _registered(false)
   {
      _type = c2c;
      _cdata = data.begin(0);
      _oneway = oneway;
      
      _planf = fftw_plan_dft_1d(data.size(), (fftw_complex*) _cdata, (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_ESTIMATE);
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_1d(data.size(), (fftw_complex*) _cdata, (fftw_complex*) _cdata, FFTW_FORWARD, FFTW_ESTIMATE);
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
   FFT::FFT(GridSystem &grid, cVec &data, bool oneway) : _dims(NULL), _registered(false)
   {
      _Create_c2c(grid, data, oneway);
   }
   
   
   /**
   * 1D real-to-complex FFT.
   * \todo Check if backward FFT is the same as c2r.
   * 
   * \param in     input.
   * \param out    output.
   * \param oneway If true only the forward plan will be created.
   */
   FFT::FFT(dVec &in, cVec &out, bool oneway) : _dims(NULL), _registered(false)
   {
      _type = r2c;
      _rin= in.begin(0);
      _cdata = out.begin(0);
      _oneway = oneway;
      
      _planf = fftw_plan_dft_r2c_1d(in.size(), _rin, (fftw_complex*) _cdata, FFTW_ESTIMATE);	
      if (!(_oneway = oneway)){
	 _planb = fftw_plan_dft_c2r_1d(in.size(), (fftw_complex*) _cdata, _rin, FFTW_ESTIMATE);
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
      if (!_registered) {
         
	 fftw_destroy_plan(_planf);
	 if(!_oneway) fftw_destroy_plan(_planb); 
      }
   }
   
   /**
   * Execute forward FFT.
   */
   void FFT::forward()
   {
      switch(_type){
	 c2c:
	       fftw_execute_dft(_planf, (fftw_complex*) _cdata, (fftw_complex*) _cdata);
            break;
	 r2c:
	       fftw_execute_dft_r2c(_planf, _rin, (fftw_complex*) _cdata);
	    break;
      }
   }
   
   
   /**
   * Execute backward FFT.
   */
   void FFT::backward()
   {
      if (!_oneway){
	 switch(_type){
	    c2c:
		  fftw_execute_dft(_planb, (fftw_complex*) _cdata, (fftw_complex*) _cdata);
	    break;
	    r2c:
		  fftw_execute_dft_c2r(_planb, (fftw_complex*) _cdata, _rin);
	    break;
	 }
      }
   }

   /* Initalize static members */
   int FFT::nplans = 0;
   FFT::planbuffer_t FFT::planbuffer[MAX_FFT_PLANS];
   
} /* namespace QDLIB */

