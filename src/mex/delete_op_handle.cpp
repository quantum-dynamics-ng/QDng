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
    * mexFunction to delete a Operator and it's handle or all Operators and it's handles
    * input (one argument): Objecthandle
    * no output
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 0 && nrhs == 1) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS ) {
		convert_op_mxArray::delete_op_handle(prhs[0]);
	} else mexErrMsgTxt("Bad input. One output and one input argumets needed (ObjectHandle)");
  } else if(nlhs == 0 && nrhs == 0) {
	op_ObjectHandle_interface::delete_all_OP();
  }else mexErrMsgTxt("Bad input. One output and no or one input argumets needed");
}