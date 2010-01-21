#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"

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
    * mexFunction to apply a Operator on a Wavefunction
    * output (one argument) is the ObjectHandle
    * input: 1) two arguments -> Operator Handle, Wavefunction Handle
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	if ( mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
   	    
	  convert_op_mxArray::apply_op(&plhs[0],prhs[0],prhs[1]);
	    
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (structmxArray)");
   }else mexErrMsgTxt("Bad input. One output and one input argumets needed");
}