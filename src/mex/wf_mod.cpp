#include "mex.h"

#include "mex/libs/modify_wf.h"
#include "mex/libs/wf_ObjectHandle_interface.h"
#include "mex/libs/convert_wf_mxArray.h"


using namespace QDMEX;
/**
    * mexFunction to load a Wavefunction from struct
    * output (one argument) is the ObjectHandle
    * input: 1)three or more input-arguments -> modifyer (+,-,*,c*,p*), Wavefunction 1, Wavefunction 2, etc 
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  /* if new Wavefunction is ordered ( one output argument)*/
  if (nlhs == 1 && nrhs > 2) {
	
	/* If modifier is registerd */
	if (mxIsChar(prhs[0]) && (!strcmp (mxArrayToString(prhs[0]),"+") || !strcmp (mxArrayToString(prhs[0]),"-") 
	  || !strcmp (mxArrayToString(prhs[0]),"*") || !strcmp (mxArrayToString(prhs[0]),"test") || !strcmp (mxArrayToString(prhs[0]),"=") 
	  || !strcmp (mxArrayToString(prhs[0]),"c*") || !strcmp (mxArrayToString(prhs[0]),"p*") )) {
	  
	    /* Addidtion*/
   	    if (!strcmp (mxArrayToString(prhs[0]),"+") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	        
		/*over all input arguments*/ 
		for (int i = 2;i<nrhs;i++) {
		  
		  /*for the first modification*/ 
		  if (i == 2) { 
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		       modify_wf::add_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle");
		  
		  /*for the following modifications*/   
		  } else {
		    
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      mxArray *temp = plhs[0];
		      modify_wf::add_wf(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle");
		  }  
		}/*end for loop*/
		
	    /* Multiplication with double */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"*") && (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS || mxGetClassID(prhs[2]) == mxDOUBLE_CLASS) ) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/ 
		  if (i == 2) {
		    
		    /*first is double second the Handle*/
		    if (mxGetClassID(prhs[i-1]) == mxUINT64_CLASS && mxGetClassID(prhs[i]) == mxDOUBLE_CLASS ) {
		      
		       modify_wf::mult_wf_double(&plhs[0],(mxArray*) prhs[i-1] ,(mxArray*) prhs[i]);
		    
		    /*first is handle second the double*/ 
		    } else if (mxGetClassID(prhs[i]) == mxUINT64_CLASS && mxGetClassID(prhs[i-1]) == mxDOUBLE_CLASS) {
		      
		       modify_wf::mult_wf_double(&plhs[0],(mxArray*) prhs[i] ,(mxArray*) prhs[i-1]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle or no double");
		  
		  /*for the following modifications*/
		  } else {
		    
		    /*if is double */
		    if (mxGetClassID(prhs[i]) == mxDOUBLE_CLASS) { 
		      
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_double(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		    } else mexErrMsgTxt("Bad input. No double");
		  }  
		} /*end for loop*/
		
	    /* assigne with double */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"=") && (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS || mxGetClassID(prhs[2]) == mxDOUBLE_CLASS) ) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/ 
		  if (i == 2) {
		    
		    /*first is double second the Handle*/
		    if (mxGetClassID(prhs[i-1]) == mxUINT64_CLASS && mxGetClassID(prhs[i]) == mxDOUBLE_CLASS ) {
		      
		       modify_wf::wf_assigne(&plhs[0],(mxArray*) prhs[i-1] ,(mxArray*) prhs[i]);
		    
		    /*first is handle second the double*/ 
		    } else if (mxGetClassID(prhs[i]) == mxUINT64_CLASS && mxGetClassID(prhs[i-1]) == mxDOUBLE_CLASS) {
		      
		       modify_wf::wf_assigne(&plhs[0],(mxArray*) prhs[i] ,(mxArray*) prhs[i-1]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle or no double");
		  
		  /*for the following modifications*/
		  } else {
		    
		    /*if is double */
		    if (mxGetClassID(prhs[i]) == mxDOUBLE_CLASS) { 
		      
		      mxArray *temp = plhs[0];
		      modify_wf::wf_assigne(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		    } else mexErrMsgTxt("Bad input. No double");
		  }  
		} /*end for loop*/
		
	    /* substact */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"-") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	       /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {  
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::sub_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  
		  /*for the following modifications*/  
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      mxArray *temp = plhs[0];
		      modify_wf::sub_wf(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
	   
	    /* Conjugate pointwise Multiplication */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"c*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_complex(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		    
		  /*for the following modifications*/ 
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_complex(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
		
	    /* Pointwise Multiplication */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"p*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {  
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_pointwise(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  
		  /*for the following modifications*/  
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      mxArray *temp = plhs[0];
		      modify_wf::mult_wf_pointwise(&plhs[0],(mxArray*) plhs[0] ,(mxArray*) prhs[i]);
		      convert_wf_mxArray::delete_wf_handle((const mxArray *) temp);
		      mxDestroyArray(temp);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
		
	    /* Scalar Product */
	    } else if (nrhs == 3 && mxIsChar(prhs[0]) && !strcmp (mxArrayToString(prhs[0]),"*") &&  mxGetClassID(prhs[1]) == mxUINT64_CLASS && mxGetClassID(prhs[2]) == mxUINT64_CLASS) {
	      
	      modify_wf::scalar_prod_wf(&plhs[0],(mxArray*) prhs[1] ,(mxArray*) prhs[2]);
	      
	    } else mexErrMsgTxt("Bad input. No Wavefunction handles");
	    
	} else mexErrMsgTxt("Bad input. First inputargument needs to be the operator e.g. +,-,*");
  } else /* if new Wavefunction is ordered ( one output argument)*/
  if (nlhs == 0 && nrhs > 2) {
	
	/* If modifier is registerd */
	if (mxIsChar(prhs[0]) && (!strcmp (mxArrayToString(prhs[0]),"+") || !strcmp (mxArrayToString(prhs[0]),"-") 
	  || !strcmp (mxArrayToString(prhs[0]),"*") || !strcmp (mxArrayToString(prhs[0]),"test") 
	  || !strcmp (mxArrayToString(prhs[0]),"c*") || !strcmp (mxArrayToString(prhs[0]),"p*") )) {
	  
	    /* Addidtion*/
   	    if (!strcmp (mxArrayToString(prhs[0]),"+") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	        
		/*over all input arguments*/ 
		for (int i = 2;i<nrhs;i++) {
		  
		  /*for the first modification*/ 
		  if (i == 2) { 
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::add_wf((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle");
		  
		  /*for the following modifications*/   
		  } else {
		    
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::add_wf((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction Handle");
		  }  
		}/*end for loop*/
		
	    /* Multiplication with double */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"*") && (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS || mxGetClassID(prhs[2]) == mxDOUBLE_CLASS) ) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/ 
		  if (i == 2) {
		    
		    /*first is double second the Handle*/
		    if (mxGetClassID(prhs[1]) == mxUINT64_CLASS && mxGetClassID(prhs[2]) == mxDOUBLE_CLASS ) {
		      
		      modify_wf::mult_wf_double((mxArray*) prhs[1] ,(mxArray*) prhs[2]);
		    
		    /*first is handle second the double*/ 
		    }  else mexErrMsgTxt("Bad input. No Wavefunction Handle or no double");
		  
		  /*for the following modifications*/
		  } else {
		    
		    /*if is double */
		    if (mxGetClassID(prhs[i]) == mxDOUBLE_CLASS) { 
		      
		      modify_wf::mult_wf_double((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No double");
		  }  
		} /*end for loop*/
		
	    /* assigne double */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"=") && (mxGetClassID(prhs[1]) == mxDOUBLE_CLASS || mxGetClassID(prhs[2]) == mxDOUBLE_CLASS) ) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/ 
		  if (i == 2) {
		    
		    /*first is double second the Handle*/
		    if (mxGetClassID(prhs[1]) == mxUINT64_CLASS && mxGetClassID(prhs[2]) == mxDOUBLE_CLASS ) {
		      
		      modify_wf::wf_assigne((mxArray*) prhs[1] ,(mxArray*) prhs[2]);
		    
		    /*first is handle second the double*/ 
		    }  else mexErrMsgTxt("Bad input. No Wavefunction Handle or no double");
		  
		  /*for the following modifications*/
		  } else {
		    
		    /*if is double */
		    if (mxGetClassID(prhs[i]) == mxDOUBLE_CLASS) { 
		      
		      modify_wf::wf_assigne((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No double");
		  }  
		} /*end for loop*/
		
	    /* substact */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"-") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	       /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {  
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::sub_wf((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  
		  /*for the following modifications*/  
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::sub_wf((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
	   
	    /* Conjugate pointwise Multiplication */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"c*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_complex((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		    
		  /*for the following modifications*/ 
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_complex((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
		
	    /* Pointwise Multiplication */
	    } else if (!strcmp (mxArrayToString(prhs[0]),"p*") && mxGetClassID(prhs[1]) == mxUINT64_CLASS) {
	      
	      /*over all input arguments*/
	      for (int i = 2;i<nrhs;i++) {
		
		  /*for the first modification*/
		  if (i == 2) {  
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_pointwise((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  
		  /*for the following modifications*/  
		  } else {
		    
		    /*if handle*/
		    if (mxGetClassID(prhs[i]) == mxUINT64_CLASS ) {
		      
		      modify_wf::mult_wf_pointwise((mxArray*) prhs[1] ,(mxArray*) prhs[i]);
		      
		    } else mexErrMsgTxt("Bad input. No Wavefunction handle");
		  }  
		}/*end for loop*/
	      
	    } else mexErrMsgTxt("Bad input. No Wavefunction handles");
	    
	} else mexErrMsgTxt("Bad input. First inputargument needs to be the operator e.g. +,-,*");
	
  } else if (!strcmp (mxArrayToString(prhs[0]),"norm") && mxGetClassID(prhs[1]) == mxUINT64_CLASS && nrhs == 2 && nlhs == 1) {
	     
		      modify_wf::norm_wf(&plhs[0],(mxArray*) prhs[1]);  
		      
  } else if (!strcmp (mxArrayToString(prhs[0]),"norm") && mxGetClassID(prhs[1]) == mxUINT64_CLASS && nrhs == 2 && nlhs == 0) {
	     
		      modify_wf::norm_wf((mxArray*) prhs[1]); 
		      
  } else mexErrMsgTxt("Bad input. One or no output and 3 or more input argumets needed. First inputargument needs to be the operator e.g. +,-,*,c*,p*,norm");
}