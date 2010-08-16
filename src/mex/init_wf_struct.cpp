#include "mex.h"

#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"


/**
    * mexFunction to load a Wavefunction from struct
    * output (one argument) is the ObjectHandle
    * input: 1) one arguments -> structmxArray
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 1) {
	if (mxIsStruct(prhs[0])) {
   	    
	   QDMEX::convert_wf_mxArray::init_wf_from_struct(&plhs[0],prhs[0]);
	    
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (structmxArray)");
   }else mexErrMsgTxt("Bad input. One output and one input argumets needed");
}