
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
    * mexFunction to load a Wavefunction from file
    * output (one argument) is the ObjectHandle
    * input: 1) two arguments -> Filename, WF-module e.g. GridCartesian
    *	     2) three arguments -> array of filenames, Multistate, WF-module e.g. GridCartesian
    *	     3) five arguments -> filename, Multistate, WF-module e.g. GridCartesian, #States, #WF
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	if (mxIsChar(prhs[0]) && mxIsChar(prhs[1])) {
	    convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0],prhs[1]);
	} else mexErrMsgTxt("Bad input. One output and two input argumets needed (filename, WF-module e.g. GridCartesian)");
  } else if (nlhs == 1 && nrhs == 5){
	if (mxIsChar(prhs[0]) && mxIsChar(prhs[1]) && mxIsChar(prhs[2]) && mxIsDouble(prhs[3]) && mxIsDouble(prhs[4]) && !strcmp (mxArrayToString(prhs[1]),"Multistate")) {
		
	  convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0],prhs[2],prhs[3],prhs[4]);
	  
	}else mexErrMsgTxt("Bad input. One output and 5 input argumets needed (filename, Multistate, WF-module e.g. GridCartesian, #States, #WF)");
  } else if (nlhs == 1 && nrhs == 3){
	if ( mxIsChar(prhs[0]) && mxIsChar(prhs[1]) && mxIsChar(prhs[2]) && !strcmp (mxArrayToString(prhs[1]),"Multistate")) {
		
	  convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0],prhs[1],prhs[2]);
	  
	}else if (mxIsCell(prhs[0]) && mxIsChar(prhs[1]) && mxIsChar(prhs[2]) && !strcmp (mxArrayToString(prhs[1]),"Multistate")){

	  convert_wf_mxArray::init_wf_from_file(&plhs[0],prhs[0],prhs[1],prhs[2]);
	  
	} else mexErrMsgTxt("Bad input. Three  input argumets needed: array of filenames, Multistate, WF-module e.g. GridCartesian ");
  } else mexErrMsgTxt("Bad input. One output and 2 or 3 or 5 input argumets needed");
}