#include "mex.h"
//#include "ObjectHandle.h"

#include <iostream>
#include <math.h>


void loadWF(mxArray **out1, const mxArray *in1 , const mxArray *in2) {
  //std::cout << "Hallo4" << std::endl;
  char *string_name1;
  char *string_name2;
  string_name1 = mxArrayToString(in1);
  string_name2 = mxArrayToString(in2);
  std::string name1 = string_name1;
  std::string name2 = string_name2;
  std::cout << name1 << std::endl;
  std::cout << name2 << std::endl;

  *out1= mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  //std::cout << "Hallo1" << std::endl;
  if (nlhs == 1 && nrhs == 2) {
	  //std::cout << "Hallo2" << std::endl;
	  if (mxIsChar(prhs[0]) && mxIsChar(prhs[1])) {
	    //std::cout << "Hallo3" << std::endl;
	    loadWF(&plhs[0],prhs[0],prhs[1]);
	  }
	}else mexErrMsgTxt("Bad input.");
}