#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"

/**
    * mexFunction to apply a Operator on a Wavefunction
    * output 1) no argument -> Operator is applied to the input Wavefunction
    * output 2) one argument -> the Wavefunction ObjectHandle of the Wavefunction after Operator is applied
    *				the input Wavefunction is untouched
    * input: two arguments -> Operator Handle, Wavefunction Handle
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 0 && nrhs == 2) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
   	    
	  QDMEX::convert_op_mxArray::apply_op(prhs[0],prhs[1]);
	    
	} else mexErrMsgTxt("Bad input. No output and two input argumets needed (Operator Handle, Wavefunction Handle)");
   } else if (nlhs == 1 && nrhs == 2) {
     if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
   	    
	QDMEX::convert_op_mxArray::apply_op(&plhs[0],prhs[0],prhs[1]);
	    
      } else mexErrMsgTxt("Bad input. No output and two input argumets needed (Operator Handle, Wavefunction Handle)");
     
   } else mexErrMsgTxt("Bad input. No or one output and two input argumets needed");
}
