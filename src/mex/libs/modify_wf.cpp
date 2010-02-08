
#include "mex/libs/modify_wf.h"
#include "mex/libs/wf_ObjectHandle_interface.h"
#include "mex/libs/ObjectHandle.h"
#include "mex/libs/Collector.h"

#include "mex.h"

#include <iostream>
#include <cstring>
#include <string>

#include "qdlib/WaveFunction.h"
#include "qdlib/WFGridCartesian.h"

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
/**
   * Dynamic library (for matlab) that modifies wavefunction objects in the memory.
   */

using namespace QDLIB;

/**
    * Adds two Wavefunctions pointwise (returns result as an new Wavefunction)
    */

void modify_wf::add_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    
    /*add WF1 and WF2*/
    *WF_result = *WF1;
    *WF_result += WF2;
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Substacts two Wavefunctions pointwise (returns result as an new Wavefunction)
    */

void modify_wf::sub_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    
    /*sub WF1 and WF2*/
    *WF_result = *WF1;
    *WF_result -= WF2;
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * assignes a value to each element of a Wavefunction (returns result as an new Wavefunction)
    */
void modify_wf::wf_assigne(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_double) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /*get the double and check whether complex or not*/
    double *pdr,*pdi;
    dcomplex dc;
    pdr = mxGetPr(handle_double);
    dc._real=*pdr;
    if (mxIsComplex(handle_double)) {
        pdi = mxGetPi(handle_double);
        dc._imag=*pdi;
    } else  {
        dc._imag=0;
    }
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = *WF1;
    
    /*nult WF1 with the double*/
    *WF_result = dc;
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}


/**
    * Multiplies a Wavefunctions pointwise with a double (returns result as an new Wavefunction)
    */
void modify_wf::mult_wf_double(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_double) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /*get the double and check whether complex or not*/
    double *pdr,*pdi;
    dcomplex dc;
    pdr = mxGetPr(handle_double);
    dc._real=*pdr;
    if (mxIsComplex(handle_double)) {
        pdi = mxGetPi(handle_double);
        dc._imag=*pdi;
    } else  {
        dc._imag=0;
    }
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = *WF1;
    
    /*nult WF1 with the double*/
    *WF_result *= dc;
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * calculates the scalar product of two Wavefunctions (returns result as an new Wavefunction)
    */
void modify_wf::scalar_prod_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/ 
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /*init the result mxArray*/
    handle_result[0] = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
    double *pdr,*pdi;
    pdr=mxGetPr(handle_result[0]);
    pdi=mxGetPi(handle_result[0]);
    
    /*asigne result*/
    dcomplex result = *WF1 * WF2;
    *pdr = result.real();
    *pdi = result.imag();
}

/**
    * calculates the complex product of two Wavefunctions pointwise (returns result as an new Wavefunction)
    * \f$ Psi = Psi_a^*  Psi_b \f$
    */
void modify_wf::mult_wf_complex(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    
    /*calc the direct conjugate Product*/
    *WF_result = DirectProductConugate(WF1, WF2);
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Multiplies two Wavefunctions pointwise (returns result as an new Wavefunction)
    */
void modify_wf::mult_wf_pointwise(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    
    /*calc the direct Product*/
    *WF_result = DirectProduct(WF1, WF2);
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

void modify_wf::test_wf(mxArray **handle_result, mxArray *handle_WF1) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    
    /*assigne the pointer*/ 
    WF_result = WF1;
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Norm Wavefunction (returns result as an new Wavefunction)
    */
void modify_wf::norm_wf(mxArray **handle_result, mxArray *handle_WF1) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init WF_result*/
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    
    /*Norm WF*/
    *WF_result = *WF1; 
    dcomplex c_norm = *WF1 * WF1;
    
    if (c_norm.imag() != (double) 0) 
      mexErrMsgTxt("Scalarproduct failed");
    
    double norm = c_norm.real();
    norm = 1/sqrt(norm);
    
    *WF_result *= (const double) norm;
    
    
    /*return ObectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}


/**
    * Adds two Wavefunctions pointwise 
    */

void modify_wf::add_wf(mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /*add WF1 and WF2*/
    *WF1 += WF2;
    
}

/**
    * Substacts two Wavefunctions pointwise 
    */

void modify_wf::sub_wf(mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    
    /*sub WF1 and WF2*/
    *WF1 -= WF2;

}

/**
    * Multiplies a Wavefunctions pointwise with a double 
    */
void modify_wf::wf_assigne(mxArray *handle_WF1, mxArray *handle_double) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /*get the double and check whether complex or not*/
    double *pdr,*pdi;
    dcomplex dc;
    pdr = mxGetPr(handle_double);
    dc._real=*pdr;
    if (mxIsComplex(handle_double)) {
        pdi = mxGetPi(handle_double);
        dc._imag=*pdi;
    } else  {
        dc._imag=0;
    }
    
    
    /*nult WF1 with the double*/
    *WF1 = dc;

}

/**
    * Multiplies a Wavefunctions pointwise with a double 
    */
void modify_wf::mult_wf_double(mxArray *handle_WF1, mxArray *handle_double) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /*get the double and check whether complex or not*/
    double *pdr,*pdi;
    dcomplex dc;
    pdr = mxGetPr(handle_double);
    dc._real=*pdr;
    if (mxIsComplex(handle_double)) {
        pdi = mxGetPi(handle_double);
        dc._imag=*pdi;
    } else  {
        dc._imag=0;
    }
    
    
    /*nult WF1 with the double*/
    *WF1 *= dc;

}


/**
    * calculates the complex product of two Wavefunctions pointwise 
    * \f$ Psi = Psi_a^*  Psi_b \f$
    */
void modify_wf::mult_wf_complex(mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    /*calc the direct conjugate Product*/
    *WF1 = DirectProductConugate(WF1, WF2);
    
}

/**
    * Multiplies two Wavefunctions pointwise 
    */
void modify_wf::mult_wf_pointwise(mxArray *handle_WF1, mxArray *handle_WF2) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    /* Init second WF*/
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    
    /*Check ifmodification is possible*/
    if (WF1->strides () != WF2->strides () || WF1->lsize () != WF2->lsize () )
      mexErrMsgTxt("Bad input. Wavefunction do not match!");
    
    
    /*calc the direct Product*/
    *WF1 = DirectProduct(WF1, WF2);
    
}



/*
    * Norm Wavefunction (returns result as an new Wavefunction)
    */
void modify_wf::norm_wf(mxArray *handle_WF1) {
  
    /* Init first WF*/
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    
    dcomplex c_norm = *WF1 * WF1;
    
    if (c_norm.imag() != (double) 0) 
      mexErrMsgTxt("Scalarproduct failed");
    
    double norm = c_norm.real();
    norm = 1/sqrt(norm);
    
    *WF1 *= (const double) norm;
    
}
