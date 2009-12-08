#include "mex.h"
//#include "ObjectHandle.h"

#include <iostream>
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

void loadWF(mxArray **out1, const mxArray *in1 , const mxArray *in2) {
   try {
   	char *string_name1;
   	char *string_name2;
   	string_name1 = mxArrayToString(in1);
   	string_name2 = mxArrayToString(in2);
  	std::string name1 = string_name1;
   	std::string name2 = string_name2;
   	mxFree(string_name1);
   	mxFree(string_name2);
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	std::cout << "Suffix: " << file.GetSuffix() << std::endl;
   	file.Name(name1);
   	std::cout << "Filename: " << file.Name() << std::endl;
   	WaveFunction *WF=NULL;
   	ModuleLoader* mods = ModuleLoader::Instance();
   	WF = mods->LoadWF( name2 );
   	//file >> WF;
	*out1= mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
   } catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
  	*out1= mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
   }
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 2) {
	  if (mxIsChar(prhs[0]) && mxIsChar(prhs[1])) {
	    loadWF(&plhs[0],prhs[0],prhs[1]);
	  }
	}else mexErrMsgTxt("Bad input.");
}