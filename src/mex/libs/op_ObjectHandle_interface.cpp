#include "mex/libs/op_ObjectHandle_interface.h"
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
#include "qdlib/Operator.h"
 /**
    * Dynamic library (for matlab) that handles the Oparator objects in the memory
    * together with the ObjectHandle and Collector tamplate class.
    * all functions that call ObjectHandle functions have to be in this library. Else a new Collector will be instanced. 
    */

using namespace QDLIB;

/**
    * Creates mxArray containing the handle from a Operator
    */
void op_ObjectHandle_interface::OP_to_handle_mxArray( mxArray **mx_handle, Operator* OP) {
	ObjectHandle<Operator> *handle = new ObjectHandle<Operator>(OP);
	//std::cout << "WaveFunction: " << WF << std::endl;
	*mx_handle = handle->to_mex_handle();
	
}

/**
    * Creates mxArray containing the all handles from a Operators
    */
void op_ObjectHandle_interface::all_OP_handle_mxArray( mxArray **mx_handle) {
	ObjectHandle<Operator> *handle = new ObjectHandle<Operator>();
	*mx_handle = handle->all_to_mex_handle();
	
}

/**
    * deletes all Operators from the memory
    */
void op_ObjectHandle_interface::delete_all_OP() { 
  ObjectHandle<Operator>* handle = new ObjectHandle<Operator>();
  handle->destroy_all_object();
  delete handle;
}

/**
    * deletes Operator from the memory
    */
void op_ObjectHandle_interface::deleteOP( const mxArray *mxh) {
try {
	ObjectHandle<Operator>* handle = ObjectHandle<Operator>::from_mex_handle( mxh );
	if (handle==NULL) throw ( EParamProblem("Operator not found in Collector") );
        handle->destroy_object(mxh);
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

/**
    * searches a Operator in the Collector
    */
Operator* op_ObjectHandle_interface::search_OP ( const mxArray* mxh) {
try {
	Operator* OP;
	ObjectHandle<Operator> *handle = ObjectHandle<Operator>::from_mex_handle( mxh );
	if (handle !=NULL) {;
		OP = handle->get_object();
		return OP;
	}
	throw ( EParamProblem("Operator not found in Collector") );
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

