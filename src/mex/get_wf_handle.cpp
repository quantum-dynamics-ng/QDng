#include "mex.h"


#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"


/**
    * mexFunction to get a structured mxArray of the wavefunction
    * input (one argument): Objecthandle
    * output (one argument): Struct containing the Wavefunction
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 1) {
		if (mxGetClassID(prhs[0]) == mxUINT64_CLASS) {
		   QDMEX::convert_wf_mxArray::get_wf_from_handle(&plhs[0],prhs[0]);
	  	}
		else mexErrMsgTxt("Bad input. ObjectHandle needed as input argument");
  }  else mexErrMsgTxt("Bad input. One output and one or two input argumets needed");
}