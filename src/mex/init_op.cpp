#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"

/**
    * mexFunction to load a Operator from struct
    * output (one argument) is the ObjectHandle
    * input: 1) two arguments -> structmxArray (ParamContainer) Wavefunction Handle
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	if (mxIsStruct(prhs[0]) && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
   	    
	   QDMEX::convert_op_mxArray::init_op(&plhs[0],prhs[0],prhs[1]);
	    
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (structmxArray)");
   }else mexErrMsgTxt("Bad input. One output and one input argumets needed");
}