#include "mex.h"


#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"



using namespace QDLIB;
/**
    * mexFunction to delete a wavefunction and it's handle or all wavefunction and it's handles
    * input (one argument): Objecthandle
    * no output
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 0 && nrhs == 1) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS ) {
	   QDMEX::convert_wf_mxArray::delete_wf_handle(prhs[0]);
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (ObjectHandle)");
  } else if(nlhs == 0 && nrhs == 0) {
     QDMEX::wf_ObjectHandle_interface::delete_all_WF();
  }else mexErrMsgTxt("Bad input. One output and no or one input argumets needed");
}