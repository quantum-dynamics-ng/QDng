/*
 * QDParams.cpp
 *
 *  Created on: 20.07.2011
 *      Author: mko
 */

#include "mex.h"
#include "tools/GlobalParams.h"


/**
 * Set or get QDng global parameters.
 *
 * These are mainly used for fftw behavoir or file compression control.
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
   QDLIB::ParamContainer& pm = QDLIB::GlobalParams::Instance();

   if (nrhs == 0){ /* Show all parameters */
      cout << pm << endl;
   } else if (nrhs == 1){ /* Get parameter */
      if ( mxGetClassID(prhs[0]) != mxCHAR_CLASS )
         mexErrMsgTxt("Input argument must be string");

      char* key = mxArrayToString(prhs[0]);

      if ( !pm.isPresent(key) )
         mexErrMsgTxt("Value not found");

      /* Return value*/
      std::string value;
      pm.GetValue(key, value);

      plhs[0] = mxCreateString(value.c_str());

      mxFree(key);
   } else if (nrhs == 2){ /* Set parameter */

      if ( mxGetClassID(prhs[0]) != mxCHAR_CLASS )
         mexErrMsgTxt("Input argument 1 must be string");

      /* Get the key */
      char* key = mxArrayToString(prhs[0]);
      char* sval;

      /* Get the value */
      switch (mxGetClassID(prhs[1])){
         case mxCHAR_CLASS:
            sval = mxArrayToString(prhs[1]);
            pm.SetValue(key, sval);
            mxFree(sval);
            break;
         case mxDOUBLE_CLASS:
            pm.SetValue(key, *(mxGetPr(prhs[1])) );
            break;
         default:
            mexErrMsgTxt("Value has incompatble type");
      }

      mxFree(key);
   } else  /* Any thing else is an error */
         mexErrMsgTxt("Wrong number of input arguments. Must be 1 or 2 ");
}
