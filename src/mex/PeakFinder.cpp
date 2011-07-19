/*
 * PeakFinder.cpp
 *
 *  Created on: 15.07.2011
 *      Author: mko
 */

#include "mex.h"
#include "matrix.h"

#include "tools/PeakFinder.h"


/**
 * Utilize the peak finder class for MATLAB
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
  /* Command line input check */
  if (nlhs > 3)
     mexErrMsgTxt("Invalid number of output arguments");

  if (nrhs < 1 && nrhs > 3)
     mexErrMsgTxt("Invalid number of input arguments");


  /* Check for right data type */
  mxArray* mxdata = mxDuplicateArray(prhs[0]);
  if ( !mxIsDouble(mxdata) || mxIsComplex(mxdata) )
     mexErrMsgTxt("Argument 1 must be an array of real data");

  /* Create dVec from mxArray input*/
  size_t dsize = mxGetNumberOfElements(mxdata);
  QDLIB::dVec data(dsize, mxGetPr(mxdata));
  mxDestroyArray(mxdata); /* remove temporary mxArray */

  QDLIB::PeakFinder pf;

  /* Set peak finding tolerance */
  if (nrhs > 1) {
     mxArray* mxtol = mxDuplicateArray(prhs[1]);
     if ( !mxIsDouble(mxtol) || mxIsComplex(mxtol) || mxGetNumberOfElements(mxtol) != 1)
          mexErrMsgTxt("Argument 2 must be an real scalar value for the tolerance");

     pf.Tolerance(*(mxGetPr(mxtol)));

     mxDestroyArray(mxtol);
  }

  /* Run Peakfinder with sliding windows*/
  if (nrhs > 2) {
     mxArray* mxwin = mxDuplicateArray(prhs[2]);
     if ( !mxIsDouble(mxwin) || mxIsComplex(mxwin) || mxGetNumberOfElements(mxwin) != 1)
          mexErrMsgTxt("Argument 2 must be an real scalar value for the tolerance");

     unsigned int winsize = int(*(mxGetPr(mxwin)));
     pf.Find(data, winsize);
  } else /* Run Peakfinder */
     pf.Find(data);

  /* Pack results into mxArray*/
  if (nlhs > 0){
     plhs[0] = mxCreateNumericMatrix(pf.NumPeaks(), 1, mxUINT32_CLASS, mxREAL);
     unsigned int *peakdata = (unsigned int*) mxGetData(plhs[0]);

     for (int i=0; i < pf.NumPeaks(); i++)
        peakdata[i] = pf[i] + 1;

     /* Return mean value */
     if (nlhs > 1)
        plhs[1] = mxCreateDoubleScalar(pf.Mean());

     /* Return sigma value */
     if (nlhs > 2)
        plhs[2] = mxCreateDoubleScalar(pf.Sigma());

  } else { /* Show result on console */
     mexPrintf("Found %d peaks at indices:\n", pf.NumPeaks());
     for (int i=0; i < pf.NumPeaks(); i++)
        mexPrintf("%d\n", pf[i] + 1);
  }
}
