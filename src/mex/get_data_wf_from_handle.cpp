#include "mex.h"

#include "mex/ObjectHandle.h"
#include "mex/Collector.h"
#include "mex/convert_to_mxArray.h"

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

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 1) {
		if (mxGetClassID(prhs[0]) == mxUINT64_CLASS) {
			try {
				WaveFunction *WF=NULL;
				WF = convert_to_mxArray::search_WF ( prhs[0]); 
				//std::cout << "WaveFunction: " << WF << std::endl;
				if (WF == NULL)
	   				 throw ( EParamProblem("WaveFunction loading failed") );
				convert_to_mxArray::WF_to_Struct_mxArray( &plhs[0], WF);
			} catch (Exception e) {
				mexErrMsgTxt(e.GetMessage().c_str());
			}
	  	}
		else mexErrMsgTxt("Bad input.");
  } else if (nlhs == 0 && nrhs == 2) {
	if (mxGetClassID(prhs[0]) == mxUINT64_CLASS && mxIsChar(prhs[1])) {
		if ( !strcmp (mxArrayToString(prhs[1]),"delete")) {
			convert_to_mxArray::deleteWF( prhs[0] );
		}
	}
  } else mexErrMsgTxt("Bad input.");
}