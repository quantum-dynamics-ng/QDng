
#include "mex.h"

#include "mex/libs/modify_wf.h"
#include "mex/libs/wf_ObjectHandle_interface.h"
#include "mex/libs/convert_wf_mxArray.h"

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
    * input: 1)three or more input-arguments -> modifyer (+,-,*,c*,p*), Wavefunction 1, Wavefunction 2, etc 
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs > 2) {
	if (mxIsChar(prhs[0]) && (!strcmp (mxArrayToString(prhs[0]),"+") || !strcmp (mxArrayToString(prhs[0]),"-") 
	  || !strcmp (mxArrayToString(prhs[0]),"*") || !strcmp (mxArrayToString(prhs[0]),"test") 
	  || !strcmp (mxArrayToString(prhs[0]),"c*") || !strcmp (mxArrayToString(prhs[0]),"p*") )) {
   	    if (!strcmp (mxArrayToString(prhs[0]),"+") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
		for (int i = 2;i<nrhs;i++) {
		  if (i == 2) { 
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      modify_wf::add_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  } else {
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      mxArray *temp = plhs[0];
		      modify_wf::add_wf(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  }  
		}
	    } else if (!strcmp (mxArrayToString(prhs[0]),"*") && (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS || mxGetClassID(prhs[2]) == mxDOUBLE_CLASS) ) {
	      for (int i = 2;i<nrhs;i++) {
		  if (i == 2) {
		    if (mxGetClassID(prhs[i-1]) == mxUINT64_CLASS && mxGetClassID(prhs[i]) == mxDOUBLE_CLASS ) {
		      modify_wf::mult_wf_double(&plhs[0],(mxArray*) prhs[i-1] ,(mxArray*) prhs[i]);
		    } else if (mxGetClassID(prhs[i]) == mxUINT64_CLASS && mxGetClassID(prhs[i-1]) == mxDOUBLE_CLASS) {
		      modify_wf::mult_wf_double(&plhs[0],(mxArray*) prhs[i] ,(mxArray*) prhs[i-1]);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  } else {
		    if (mxGetClassID(prhs[i]) == mxDOUBLE_CLASS) {
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_double(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  }  
		}
	    } else if (!strcmp (mxArrayToString(prhs[0]),"-") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      for (int i = 2;i<nrhs;i++) {
		  if (i == 2) {  
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      modify_wf::sub_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  } else {
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      mxArray *temp = plhs[0];
		      modify_wf::sub_wf(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  }  
		}
	    } else if (!strcmp (mxArrayToString(prhs[0]),"test") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      modify_wf::test_wf(&plhs[0],(mxArray*) prhs[1]);
	    } else if (!strcmp (mxArrayToString(prhs[0]),"c*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      for (int i = 2;i<nrhs;i++) {
		  if (i == 2) {  
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      modify_wf::mult_wf_complex(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  } else {
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_complex(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  }  
		}
	    } else if (!strcmp (mxArrayToString(prhs[0]),"p*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      for (int i = 2;i<nrhs;i++) {
		  if (i == 2) {  
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      modify_wf::mult_wf_pointwise(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  } else {
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_pointwise(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		    } else mexErrMsgTxt("Bad input. Modifikation not defined");
		  }  
		}
	    } else if (nrhs == 3 && mxIsChar(prhs[0]) && !strcmp (mxArrayToString(prhs[0]),"*") &&  mxGetClassID(prhs[1]) == mxUINT64_CLASS && mxGetClassID(prhs[2]) == mxUINT64_CLASS) {
	      modify_wf::scalar_prod_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[2]);
	    } else mexErrMsgTxt("Bad input. Modifikation not defined");
	    
	} else mexErrMsgTxt("Bad input. First inputargument needs to be the operator e.g. +,-,*");
   } else mexErrMsgTxt("Bad input. One output and 3 or more input argumets needed");
}