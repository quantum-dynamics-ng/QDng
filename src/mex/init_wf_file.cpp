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
  if (nlhs == 1 && nrhs == 2) {
	if (mxIsChar(prhs[0]) && mxIsChar(prhs[1])) {
	    char *string_File_name = mxArrayToString(prhs[0]);
   	    char *string_WF_type = mxArrayToString(prhs[1]);
	    WaveFunction* WF = convert_to_mxArray::loadWF(string_File_name,string_WF_type);
	    mxFree(string_File_name);
   	    mxFree(string_WF_type);
	    convert_to_mxArray::WF_to_handle_mxArray(&plhs[0],WF);
	} else mexErrMsgTxt("Bad input.");
  } else if (nlhs == 1 && nrhs == 5){
	if (mxIsChar(prhs[0]) && mxIsChar(prhs[1]) && mxIsChar(prhs[2]) && mxIsDouble(prhs[3]) && mxIsDouble(prhs[4]) && !strcmp (mxArrayToString(prhs[1]),"Multistate")) {
		char *string_Base_File_name = mxArrayToString(prhs[0]);
   	    	char *string_WF_type = mxArrayToString(prhs[2]);
		int States = (int) *mxGetPr(prhs[3]);
		int Number = (int) *mxGetPr(prhs[4]);
		std::cout << "States: " << States << std::endl;
		std::cout << "Number: " << Number << std::endl;
		WaveFunction *wfsub;
	 	WFMultistate *multi = new WFMultistate();
		for (int i = 0; i<States; i++) {
			std::ostringstream ss;
			ss << string_Base_File_name << "-" << i << "_" << Number ;
			char *string_File_name = new char [ss.str().size()+1];
			strcpy (string_File_name,ss.str().c_str());
			wfsub = convert_to_mxArray::loadWF( string_File_name , string_WF_type );
			multi->Add( wfsub, i);	
			mxFree(string_File_name);	
		}
		convert_to_mxArray::WF_to_handle_mxArray(&plhs[0],multi);
		mxFree(string_Base_File_name);
   	        mxFree(string_WF_type);
	}else mexErrMsgTxt("Bad input.");
  } else if (nlhs == 1 && nrhs == 3){
	if ( mxIsChar(prhs[0]) && mxIsChar(prhs[1]) && mxIsChar(prhs[2]) && !strcmp (mxArrayToString(prhs[1]),"Multistate")) {
		int States = mxGetM(prhs[0]);
		int str_length = mxGetN(prhs[0]);
		if (States > 1) {
			char *string_File_name_all = mxArrayToString(prhs[0]);
			char *files[States];
			for (int i =0 ; i<States; i++) {
				files[i] = new char [str_length+1];
				for(int j =0; j<str_length;j++) {
					files[i][j]=string_File_name_all[i+(States*j)];
				}
				files[i][str_length] = string_File_name_all[States*str_length];
			}
			char *string_WF_type = mxArrayToString(prhs[2]);
			WaveFunction *wfsub;
	 		WFMultistate *multi = new WFMultistate();
			for (int i = 0; i < States; i++) {	
				wfsub = convert_to_mxArray::loadWF( files[i], string_WF_type );
				multi->Add( wfsub, i);
				mxFree(files[i]);
			}
			convert_to_mxArray::WF_to_handle_mxArray(&plhs[0],multi);
			mxFree(string_File_name_all);
   	       		mxFree(string_WF_type);
		} else mexErrMsgTxt("Bad input.");
	} else mexErrMsgTxt("Bad input.");
  } else mexErrMsgTxt("Bad input.");
}