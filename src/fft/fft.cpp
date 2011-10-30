#include "fft.h"
#include <stdio.h>

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "fft.h"
#include "FFTGlobal.h"

namespace QDLIB {

   /** Determine points in lower other dimensions.
    *  (for coloumn major, QD-order)
    **/
   int FFT::_lothers(int dim) const
   {
      int lothers = 1;
      if (dim>0)
      {
         for(int i=0; i < dim; i++)
            lothers *= _dims[i];
      }
      return lothers;
   }

   /** Determine points in lower other dimensions.
    *  (for row major, C-order)
    **/
   int FFT::_uothers(int cdim) const
   {
      return _lothers(_ndims - cdim -1);
   }


   void FFT::_initplans()
   {
      for (int i=0; i < MAX_DIMS+1; i++) {
         _planf[i] = NULL;
         _planb[i] = NULL;
      }
   }

   /**
   * 1D complex-to-complex FFT.
   * 
   * Meaning of in and out will be reversed for backward transformation.
   * 
   * \param in     input
   * \param out    output
   * \param oneway If true only the forward plan will be created
   */
   FFT::FFT(cVec &in, cVec &out, bool oneway) :
            _dims(NULL), _rdims(NULL), _ndims(1), _oneway(oneway), _in(&in),
            _type(c2c), _cin(in.begin(0)), _din(NULL), _cout(out.begin(0)), _dout(NULL)
   {
      cVec inbuf = in;
      _initplans();
      
      _planf[0] = fftw_plan_dft_1d(in.size(), (fftw_complex*) _cin, (fftw_complex*) _cout, FFTW_FORWARD, FFTW_MEASURE);
      if (!(_oneway = oneway)){
         _planb[0] = fftw_plan_dft_1d(in.size(), (fftw_complex*) _cout, (fftw_complex*) _cin, FFTW_BACKWARD, FFTW_MEASURE);
      }
      in = inbuf;
      
   }
   
   /**
   * ND complex-to-complex FFT.
   * 
   * Meaning of in and out will be reversed for backward transformation.
   * 
   * \param grid   GridSystem description
   * \param in     input
   * \param out    output (content maybe destroyed)
   * \param oneway If true only the forward plan will be created
   */
   FFT::FFT(GridSystem &grid, cVec &in, cVec &out, bool oneway) :
            _ndims(grid.Dim()), _oneway(oneway), _in(&in),
            _type(c2c), _cin(in.begin(0)), _din(NULL), _cout(out.begin(0)), _dout(NULL)
   {

      int optflag = FFTGlobal::Instance().OptimizeFlag();
      _initplans();

      _dims = new int[grid.Dim()];
      _rdims = new int[grid.Dim()]; /* reverse order of dimension array */
      for (int i = 0; i < grid.Dim(); i++) {
         _rdims[grid.Dim() - i - 1] = grid.DimSize(i);
         _dims[i] = grid.DimSize(i);
      }

      /* Create fftw plans */
      _planf[0] = fftw_plan_dft(grid.Dim(), _rdims, (fftw_complex*) _cin, (fftw_complex*) _cout,
               FFTW_FORWARD, optflag | FFTW_PRESERVE_INPUT | FFTW_WISDOM_ONLY);

      if (_planf[0] == NULL){ /* Create fresh, optimized plan */
         cVec inbuf;
         inbuf = in; /* Save for planning */
         _planf[0] = fftw_plan_dft(grid.Dim(), _rdims, (fftw_complex*) _cin, (fftw_complex*) _cout,
                  FFTW_FORWARD, optflag);

         in = inbuf;
      }

      /* backward plan */
      if (!(_oneway = oneway)) {
         _planb[0] = fftw_plan_dft(grid.Dim(), _rdims, (fftw_complex*) _cout, (fftw_complex*) _cin,
                  FFTW_BACKWARD, optflag | FFTW_PRESERVE_INPUT | FFTW_WISDOM_ONLY);
         if (_planb[0] == NULL){
            cVec inbuf;
            inbuf = in; /* Save for planning */
            _planb[0] = fftw_plan_dft(grid.Dim(), _rdims, (fftw_complex*) _cout, (fftw_complex*) _cin,
                     FFTW_BACKWARD, optflag);
            in = inbuf;
         }
      }
   }
   
   
   /**
   * 1D real-to-complex FFT.
   * 
   * \param in     input.
   * \param out    output. Note that the output size must be n/2+1.
   * \param oneway If true only the forward plan will be created.
   */
   FFT::FFT(dVec &in, cVec &out, bool oneway) :
            _dims(NULL), _rdims(NULL), _ndims(1), _oneway(oneway), _in(NULL),
            _type(r2c), _cin(NULL), _din(in.begin(0)), _cout(out.begin(0)), _dout(NULL)
   {
      _initplans();
      _planf[0] = fftw_plan_dft_r2c_1d(in.size(), in.begin(0), (fftw_complex*) out.begin(0), FFTW_ESTIMATE);
      if (!(_oneway = oneway)){
         _planb[0] = fftw_plan_dft_c2r_1d(in.size(), (fftw_complex*) out.begin(0), in.begin(0), FFTW_ESTIMATE);
      }
   }
      
   /** Destructor.
   * 
   * Destroys the FFTW Plans. You have to free the pointers of
   * to the in/out memory in a any other place.
   */
   FFT::~FFT()
   {
      if (_rdims != NULL) delete[] _rdims;
      if (_dims != NULL) delete [] _dims;
      for (int i=0; i < MAX_DIMS+1; i++){
         if (_planf[i] != NULL) fftw_destroy_plan(_planf[i]);
         if (_planb[i] != NULL) fftw_destroy_plan(_planb[i]);
      }
   }
   
   /**
    * Create Plan for a single dimension.
    */
   void FFT::_CreatePlanDim(int dim)
   {
      int optflag = FFTGlobal::Instance().OptimizeFlag();

      int lothers = _lothers(dim);  /* points in lower dims */
      int cdim = _ndims - dim - 1; /* reversed dim index (C-order) */

      /* describe 1D-stripes */
      fftw_iodim datadim;
      datadim.n = _rdims[cdim];
      datadim.is = datadim.os = lothers;

      /* describe loop over other dimension */
      fftw_iodim* loopdim = new fftw_iodim[_ndims-1];
      int k = 0;
      for (int i=0; i < _ndims; i++){
         if (i != cdim){
            loopdim[k].n = _rdims[i];
            loopdim[k].is = _uothers(i);
            loopdim[k].os = _uothers(i);
            k++;
         }
      }

      /* Create plans for the specific dimension */
      _planf[dim+1] = fftw_plan_guru_dft(1, &datadim, _ndims - 1, loopdim,
                                       (fftw_complex*) _cin,
                                       (fftw_complex*) _cout,
                                       FFTW_FORWARD, FFTW_PATIENT | FFTW_PRESERVE_INPUT | FFTW_WISDOM_ONLY);



      if (_planf[dim+1] == NULL){ /* Create fresh, optimized plan */
         cVec inbuf;
         inbuf = *_in; /* Save for planning */
         _planf[dim+1] = fftw_plan_guru_dft(1, &datadim, _ndims - 1, loopdim,
                                          (fftw_complex*) _cin,
                                          (fftw_complex*) _cout,
                                          FFTW_FORWARD, optflag);
         *_in = inbuf;
      }


      /* backward plan */
      if (!_oneway){

         _planb[dim+1] = fftw_plan_guru_dft(1, &datadim, _ndims - 1, loopdim,
                                          (fftw_complex*) _cout,
                                          (fftw_complex*) _cin,
                                          FFTW_BACKWARD, FFTW_PATIENT | FFTW_PRESERVE_INPUT | FFTW_WISDOM_ONLY);

         if (_planb[dim+1] == NULL){ /* Create fresh, optimized plan */
            cVec inbuf;
            inbuf = *_in; /* Save for planning */
            _planb[dim+1] = fftw_plan_guru_dft(1, &datadim, _ndims - 1, loopdim,
                                             (fftw_complex*) _cout,
                                             (fftw_complex*) _cin,
                                             FFTW_BACKWARD, optflag);
           *_in = inbuf;
         }

      }

      delete[] loopdim;
   }


   /**
    * Replace the buffers.
    *
    * Properties must be exactly the same!
    */
   void FFT::ReplaceBuffers(cVec *in, cVec *out)
   {
      _cin = in->begin(0);
      _cout = out->begin(0);
   }

   void FFT::ReplaceBuffers(dVec *in, cVec *out)
   {
      _din = in->begin(0);
      _cout = out->begin(0);
   }

   /**
    * Execute forward fft.
    *
    * If dim is specified the transform is carried out along the specified dimension
    */
   void FFT::forward(int dim)
   {
      if (_planf[dim+1] == NULL && dim > -1) _CreatePlanDim(dim);
      switch (_type){
         case c2c:
            fftw_execute_dft(_planf[dim+1], (fftw_complex*) _cin, (fftw_complex*) _cout);
           break;
         case r2c:
            fftw_execute_dft_r2c(_planf[dim+1], _din, (fftw_complex*) _cout);
           break;
         case r2r:
            /* Not implemented */
            break;
      }

   }

   /**
    * Execute backward FFT.
    *
    * If dim is specified the transform is carried out along the specified dimension.
    * Note that this call is only valid if oneway isn't specified.
    */
   void FFT::backward(int dim)
   {
      if (_planb[dim+1] == NULL && dim > -1) _CreatePlanDim(dim);
      switch (_type){
         case c2c:
            fftw_execute_dft(_planb[dim+1], (fftw_complex*) _cout, (fftw_complex*) _cin);
           break;
         case r2c:
            fftw_execute_dft_c2r(_planb[dim+1], (fftw_complex*) _cout, _din);
           break;
         case r2r:
            /* Not implemented */
            break;
      }
   }
   
} /* namespace QDLIB */

