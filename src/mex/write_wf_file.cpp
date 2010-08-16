#include "mex.h"

#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"



/**
    * mexFunction to write a Wavefunction to file
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
      if (nlhs == 1 && nrhs == 2) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxIsChar(prhs[1]) ) {
	  
	   QDMEX::convert_wf_mxArray::write_wf_to_file(&plhs[0],prhs[0],prhs[1]);
	  
	}  else mexErrMsgTxt("Bad input. Two input arguments needed (Objecthandle, Filename)");
      } else if ( nlhs == 1 && nrhs == 3 ) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxIsChar(prhs[1]) && mxIsDouble(prhs[2]) ) {
	  
	   QDMEX::convert_wf_mxArray::write_wf_to_file(&plhs[0],prhs[0],prhs[1],prhs[2]);
	  
	}  else mexErrMsgTxt("Bad input. Three input arguments needed (Objecthandle, Filename, #sequenze)"); 
      }else mexErrMsgTxt("Bad input. One output argument and two input arguments needed (Objecthandle, Filename)");
}