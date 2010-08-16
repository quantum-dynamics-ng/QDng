#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"


/**
    * mexFunction calculate the expectation value of an Operator
    * output (one argument) expectation value
    * input: 1) two arguments -> Operator Handle, Wavefunction Handle
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
   	    
	   QDMEX::convert_op_mxArray::op_expec(&plhs[0],prhs[0],prhs[1]);
	    
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (structmxArray)");
   }else mexErrMsgTxt("Bad input. One output and one input argumets needed");
}