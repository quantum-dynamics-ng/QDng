#include "mex.h"

#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"

/**
    * mexFunction to get all Wavefunction handles
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 0) {
     QDMEX::wf_ObjectHandle_interface::all_WF_handle_mxArray(&plhs[0]);
  }  else mexErrMsgTxt("Bad input. One output and no input argumets needed");
}