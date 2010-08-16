#include "mex.h"

#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"


/**
    * mexFunction to load a Wavefunction from file
    * output (one argument) is the ObjectHandle
    * input: 1) one arguments -> Filename or cell array of filenames
    *	     2) two arguments -> array of filenames,int  #WF
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 1) {
	if (mxIsChar(prhs[0])) {
	  
	   QDMEX::convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0]);
	    
	} else if (mxIsCell(prhs[0])) {
	  
	   QDMEX::convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0]);
	  
	}else mexErrMsgTxt("Bad input. One output and one input argumets needed (filename)");
  } else if (nlhs == 1 && nrhs == 2){
	if (mxIsChar(prhs[0]) &&  mxIsDouble(prhs[1])) {
		
	   QDMEX::convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0],prhs[1]);
	  
	} else mexErrMsgTxt("Bad input. One output and two input argumets needed (filename #WF)");
  }  else mexErrMsgTxt("Bad input. One output and one or two input argumets needed");
}