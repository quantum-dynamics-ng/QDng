#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"
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
#include "qdlib/FileWF.h"
#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridPosition.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/Operator.h"
 /**
    * Dynamic library (for matlab) that handles the the Operators objects in the memory
    * together with the ObjectHandle and Collector tamplate class.
    * all functions that interact with mxArray are placed here.
    */

using namespace QDLIB;

/**
    * inits Operator from File(s)
    */
void convert_op_mxArray::init_op(mxArray **out_arg, const mxArray *Struct_mxArray, const mxArray *WF_h_mxArray ) {
try {  
  /*Create WF pointer*/
  WaveFunction *WF=NULL;
  
  /*Search for the right pointer and pass it to WF*/
  WF = wf_ObjectHandle_interface::search_WF (WF_h_mxArray); 
  //std::cout << "WaveFunction: " << WF << std::endl;
  
  /*if loading fails*/
  if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
  
  /*Create OP pointer*/
  Operator* OP=NULL;
  
  /*Copy Struct to be save*/
  mxArray *s_mxArray = mxDuplicateArray(Struct_mxArray);
  
  /*Load Operator with Parameter Struct*/
  OP = convert_op_mxArray::loadOP(s_mxArray);
  
  /*Delete Copy*/
  mxDestroyArray(s_mxArray);
  
  /*if loading fails*/
  if (OP == NULL)
      throw ( EParamProblem("Operator loading failed") );
  
  /*Init OP with Wavefunction*/
  OP->Init(WF);
  
  /*return the ObjectHandle*/
  op_ObjectHandle_interface::OP_to_handle_mxArray(out_arg,OP);
  
  } catch (Exception e) {
     
    mexErrMsgTxt(e.GetMessage().c_str());
	
 }
}

/**
    * loads a Operator from structured mxArray
    */
Operator* convert_op_mxArray::loadOP(mxArray *s_mxArray) {
  
  /*Check the s_mxArray for validity */
  if (!mxIsStruct(s_mxArray))
    throw ( EParamProblem("Operator loading failed due to non structred array") );
  if (mxGetFieldNumber(s_mxArray, "CLASS") == -1)
    throw ( EParamProblem("Operator contains no CLASS field") );
  /*get CLASS field*/
  mxArray *mx_class = mxGetField(s_mxArray, 0,"CLASS");

  /*Check is CLASS is Char*/
  if (mxIsChar(mx_class)) {
      
      char *s = mxArrayToString(mx_class);
      std::string s_class =  s;
      mxFree((void*) s );
      
      /*If class is WFMultistate */ 
      if (!s_class.compare("OPMultistate") || !s_class.compare("Multistate") ) {
	throw ( EParamProblem("Multistate not implemented jet") );
      } else if (!s_class.compare("OPSum") || !s_class.compare("OPSum")) {
	throw ( EParamProblem("Summ not implemented jet") );
      } else if (!s_class.compare("OPGridSum") || !s_class.compare("OPGridSum")) {
	throw ( EParamProblem("GridSumm not implemented jet") );
      } else {
	
	/*Get OP-type and nodify it if needed*/
	char *s = mxArrayToString(mxGetFieldByNumber(s_mxArray,0,mxGetFieldNumber(s_mxArray, "CLASS")));
	std::string sOP_type = s;
	mxFree((void*) s);
	std::string s_OP = "OP";
	if ( !s_OP.compare(0,2,sOP_type,0,2) ) {
	  sOP_type.erase(0,2);
	} else if ( !s_OP.compare(0,1,sOP_type,0,1) ) {
	  sOP_type.erase(0,1);
	}
	
	/*init WF*/
	Operator *OP=NULL;
	ModuleLoader* mods = ModuleLoader::Instance();
	OP = mods->LoadOp( sOP_type );
	
	/*check the succses*/
	if (OP == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
	
	 /*Copy the content of the struct to the ParameterConatainer (exept "data" and "handle")*/
	int elements = mxGetNumberOfFields(s_mxArray);
	ParamContainer param;
	for (int i=0;i<elements;i++) {
	  std::string fname = mxGetFieldNameByNumber(s_mxArray,i);
	  if (fname.compare("handle") ) {
	    char *s= mxArrayToString(mxGetFieldByNumber(s_mxArray,0,i));
	    param.SetValue((const std::string) fname,(const std::string) s);
	    mxFree((void*) s);
	  }
	}
	
	/*Init OP with Parametercontainer*/
	OP->Init(param);
	
	/*Return Operator*/
	return OP;
	
      }
  } {throw ( EParamProblem("Field CLASS contains no string"));}
  
}