#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"


using namespace QDLIB;

/**
    * mexFunction to delete a Operator and it's handle or all Operators and it's handles
    * input (one argument): Objecthandle
    * no output
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 0 && nrhs == 1) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS ) {
		QDMEX::convert_op_mxArray::delete_op_handle(prhs[0]);
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (ObjectHandle)");
  } else if(nlhs == 0 && nrhs == 0) {
     QDMEX::op_ObjectHandle_interface::delete_all_OP();
  }else mexErrMsgTxt("Bad input. One output and no or one input argumets needed");
}
