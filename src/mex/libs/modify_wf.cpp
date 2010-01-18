
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
    * Adds two Wavefunctions pointwise
    */

void modify_wf::add_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = *WF1;
    *WF_result += WF2;
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Substacts two Wavefunctions pointwise
    */

void modify_wf::sub_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = *WF1;
    *WF_result -= WF2;
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Multiplies a Wavefunctions pointwise with a double
    */
void modify_wf::mult_wf_double(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_double) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
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
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = *WF1;
    *WF_result *= dc;
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * calculates the scalar product of two Wavefunctions
    */
void modify_wf::scalar_prod_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    handle_result[0] = mxCreateDoubleMatrix(1,1,mxCOMPLEX);
    double *pdr,*pdi;
    pdr=mxGetPr(handle_result[0]);
    pdi=mxGetPi(handle_result[0]);
    dcomplex result = *WF1 * WF2;
    *pdr = result.real();
    *pdi = result.imag();
}

/**
    * calculates the complex product of two Wavefunctions pointwise
    * \f$ Psi = Psi_a^*  Psi_b \f$
    */
void modify_wf::mult_wf_complex(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = DirectProductConugate(WF1, WF2);
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

/**
    * Multiplies two Wavefunctions pointwise
    */
void modify_wf::mult_wf_pointwise(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF2=NULL;
    WF2 = wf_ObjectHandle_interface::search_WF ( handle_WF2);
    WaveFunction *WF_result=NULL;
    WF_result = WF1->NewInstance ();
    *WF_result = DirectProduct(WF1, WF2);
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}

void modify_wf::test_wf(mxArray **handle_result, mxArray *handle_WF1) {
    WaveFunction *WF1=NULL;
    WF1 = wf_ObjectHandle_interface::search_WF ( handle_WF1);
    WaveFunction *WF_result=NULL;
    WF_result = WF1;
    wf_ObjectHandle_interface::WF_to_handle_mxArray(handle_result, WF_result);
}
