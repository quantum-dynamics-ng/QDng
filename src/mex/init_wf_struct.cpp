#include "mex.h"

#include "mex/convert_wf_mxArray.h"
#include "mex/wf_ObjectHandle_interface.h"

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
    * mexFunction to load a Wavefunction from struct
    * output (one argument) is the ObjectHandle
    * input: 1) two arguments -> structmxArray, WF-module e.g. GridCartesian
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	if (mxIsStruct(prhs[0]) && mxIsChar(prhs[1])) {
   	    
	  convert_wf_mxArray::init_wf_from_struct(&plhs[0],prhs[0],prhs[1]);
	    
	} else mexErrMsgTxt("Bad input. One output and two input argumets needed (structmxArray, WF-module e.g. GridCartesian)");
   }else mexErrMsgTxt("Bad input. One output and 2 input argumets needed");
}