
#include "mex/libs/convert_wf_mxArray.h"
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
    * Dynamic library (for matlab) that handles the the wavefunction objects in the memory
    * together with the ObjectHandle and Collector tamplate class.
    * all functions that call ObjectHandle functions have to be in this library. Else a new Collector will be instanced. 
    */

using namespace QDLIB;

/**
    * Creates mxArray containing the handle from a Wavefunction
    */
void wf_ObjectHandle_interface::WF_to_handle_mxArray( mxArray **mx_handle, WaveFunction* WF) {
	ObjectHandle<WaveFunction> *handle = new ObjectHandle<WaveFunction>(WF);
	//std::cout << "WaveFunction: " << WF << std::endl;
	*mx_handle = handle->to_mex_handle();
	
}

/**
    * Creates mxArray containing the all handle from a Wavefunction
    */
void wf_ObjectHandle_interface::all_WF_handle_mxArray( mxArray **mx_handle) {
	ObjectHandle<WaveFunction> *handle = new ObjectHandle<WaveFunction>();
	*mx_handle = handle->all_to_mex_handle();
	
}

/**
    * deletes all Wavefunction from the memory
    */
void wf_ObjectHandle_interface::delete_all_WF() { 
  ObjectHandle<WaveFunction>* handle = new ObjectHandle<WaveFunction>();
  handle->destroy_all_object();
  delete handle;
}

/**
    * deletes Wavefunction from the memory
    */
void wf_ObjectHandle_interface::deleteWF( const mxArray *mxh) {
  try {
	  ObjectHandle<WaveFunction>* handle = ObjectHandle<WaveFunction>::from_mex_handle( mxh );
	  if (handle==NULL) throw ( EParamProblem("WaveFunction not found in Collector") );
	  handle->destroy_object(mxh);
  }catch (Exception e) {
	  std::cout << e.GetMessage() << std::endl;
	  mexErrMsgTxt(e.GetMessage().c_str());
  }
}

/**
    * searches a Wavefunction in the Collector
    */
WaveFunction* wf_ObjectHandle_interface::search_WF ( const mxArray* mxh) {
  try {
	  WaveFunction* WF;
	  ObjectHandle<WaveFunction> *handle = ObjectHandle<WaveFunction>::from_mex_handle( mxh );
	  if (handle !=NULL) {;
		  WF = handle->get_object();
		  return WF;
	  }else {
	    throw ( EParamProblem("WaveFunction not found in Collector") );
	  }
  }catch (Exception e) {
	  std::cout << e.GetMessage() << std::endl;
	  mexErrMsgTxt(e.GetMessage().c_str());
  }
  return NULL;
}

