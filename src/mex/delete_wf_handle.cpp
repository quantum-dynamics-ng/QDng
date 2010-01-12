#include "mex.h"


#include "mex/convert_wf_mxArray.h"
#include "mex/wf_ObjectHandle_interface.h"

#include <iostream>
#include <string>
#include <math.h>

#include "qdlib/WaveFunction.h"
#include "qdlib/WFGridCartesian.h"

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
    * mexFunction to get a structured mxArray of the wavefunction
    * input (one argument): Objecthandle
    * output (one argument): Struct containing the Wavefunction
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 0 && nrhs == 1) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS ) {
		convert_wf_mxArray::delete_wf_handle(prhs[0]);
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (ObjectHandle)");
  } else mexErrMsgTxt("Bad input. One output and one input argumets needed");
}