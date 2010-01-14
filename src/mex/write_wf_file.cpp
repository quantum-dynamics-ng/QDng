
#include "mex.h"

#include "mex/libs/convert_wf_mxArray.h"
#include "mex/libs/wf_ObjectHandle_interface.h"

#include <iostream>
#include <string>
#include <math.h>

#include "tools/Exception.h"
#include "tools/FileSingleDefs.h"
#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"

using namespace QDLIB;

/**
    * mexFunction to write a Wavefunction to file
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
      if (nlhs == 1 && nrhs == 2) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxIsChar(prhs[1]) ) {
	  
	  convert_wf_mxArray::write_wf_to_file(&plhs[0],prhs[0],prhs[1]);
	  
	}  else mexErrMsgTxt("Bad input. Two input arguments needed (Objecthandle, Filename)");
      } else if ( nlhs == 1 && nrhs == 3 ) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxIsChar(prhs[1]) && mxIsDouble(prhs[2]) ) {
	  
	  convert_wf_mxArray::write_wf_to_file(&plhs[0],prhs[0],prhs[1],prhs[2]);
	  
	}  else mexErrMsgTxt("Bad input. Three input arguments needed (Objecthandle, Filename, #sequenze)"); 
      }else mexErrMsgTxt("Bad input. One output argument and two input arguments needed (Objecthandle, Filename)");
}