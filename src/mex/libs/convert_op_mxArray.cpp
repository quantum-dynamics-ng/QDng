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
    * inits Operator from Struct
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
    * Calculates Expectation vale of an Operator
    */
void convert_op_mxArray::op_expec(mxArray **out_arg, const mxArray *OP_h_mxArray, const mxArray *WF_h_mxArray ) {
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
  
  /*Search for the right pointer and pass it to OP*/
  OP =  op_ObjectHandle_interface::search_OP (OP_h_mxArray);
  
  /*if loading fails*/
  if (OP == NULL)
      throw ( EParamProblem("Operator loading failed") );
  
  /*Check for the right Wavefunction*/
  bool valid = OP->Valid(WF);
  //std:: cout << "Valid Check: " << valid << std::endl;
    
  if (valid == false)
    throw ( EParamProblem("Mismatching Operator and Wavefunction") );
  
  /*Apply Operator*/
  double expect_val = OP->Expec(WF);
  
  //std::cout << "Value: " << expect_val << std::endl;
  
  /*init the result mxArray*/
  out_arg[0] = mxCreateDoubleMatrix(1,1,mxREAL);
  double *pdr;
  pdr=mxGetPr(out_arg[0]);
  
  *pdr = expect_val;
  
  
  } catch (Exception e) {
     
    mexErrMsgTxt(e.GetMessage().c_str());
	
 }
}

/**
    * Applies an Operator to a Wavefunction
    */
void convert_op_mxArray::apply_op(const mxArray *OP_h_mxArray, const mxArray *WF_h_mxArray ) {
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
  
  /*Search for the right pointer and pass it to OP*/
  OP =  op_ObjectHandle_interface::search_OP (OP_h_mxArray);
  
  /*if loading fails*/
  if (OP == NULL)
      throw ( EParamProblem("Operator loading failed") );
  
  /*Check for the right Wavefunction*/
  bool valid = OP->Valid(WF);
  //std:: cout << "Valid Check: " << valid << std::endl;
    
  if (valid == false)
    throw ( EParamProblem("Mismatching Operator and Wavefunction") );
  
  /*Apply Operator*/
  WF = OP->Apply(WF);
  
  
  } catch (Exception e) {
     
    mexErrMsgTxt(e.GetMessage().c_str());
	
 }
}

/**
    * Applies an Operator to a Wavefunction
    */
void convert_op_mxArray::apply_op(mxArray **out_arg, const mxArray *OP_h_mxArray, const mxArray *WF_h_mxArray ) {
try {  
  /*Create WF pointer*/
  WaveFunction *WF=NULL;
  
  /*Search for the right pointer and pass it to WF*/
  WF = wf_ObjectHandle_interface::search_WF (WF_h_mxArray); 
  //std::cout << "WaveFunction: " << WF << std::endl;
  
  /*if loading fails*/
  if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
  
  /* Init WF_result*/
  WaveFunction *WF_result=NULL;
  WF_result = WF->NewInstance ();
  *WF_result = *WF;
  
  /*mult WF_result with 0*/
  dcomplex dc;
  dc._real = (double) 0;
  dc._imag = (double) 0;
  *WF_result *= dc;
  
  /*Create OP pointer*/
  Operator* OP=NULL;
  
  /*Search for the right pointer and pass it to OP*/
  OP =  op_ObjectHandle_interface::search_OP (OP_h_mxArray);
  
  /*if loading fails*/
  if (OP == NULL)
      throw ( EParamProblem("Operator loading failed") );
  
  /*Check for the right Wavefunction*/
  bool valid = OP->Valid(WF);
  //std:: cout << "Valid Check: " << valid << std::endl;
    
  if (valid == false)
    throw ( EParamProblem("Mismatching Operator and Wavefunction") );
  
  /*Apply Operator*/
  WF = OP->Apply(WF_result,WF);
  
  /*return the ObjectHandle (Wavefunction)*/
  wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF_result);
  
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
	
	/*Check the s_mxArray for validity */
	if (mxGetFieldNumber(s_mxArray, "operators") == -1)
	  throw ( EParamProblem("Struct contains no operators field") );
    
	if (mxGetFieldNumber(s_mxArray, "Operator") == -1)
	  throw ( EParamProblem("Struct contains no Operator field") );
	
	char *s = mxArrayToString(mxGetField(s_mxArray, 0,"operators"));
	std::string s_operators = s; 
	mxFree((void*) s );
	
	unsigned int operators;
	sscanf( s_operators.c_str(), "%d", (int*) &operators);
	
	/*Copy the  Field State in new mxArray all_States*/
	mxArray *all_Operators = mxDuplicateArray(mxGetField(s_mxArray,0,"Operator"));
	if (operators != mxGetNumberOfElements(all_Operators))
	  throw ( EParamProblem("Struct contains more of less Operators than defined in the operators Field") );
	
	
	/*Set the ParameterCotainer for the Multistate right (copy the struct-Fields)*/
	std::string name;
	std::string value;
	ParamContainer mparam;
	std::string str_handle = "handle";
	std::string str_State = "Operator";
	int nr_fields = mxGetNumberOfFields(s_mxArray);
	for (int k = 0; k<nr_fields;k++) {
	  if (str_handle.compare(mxGetFieldNameByNumber(s_mxArray,k)) && str_State.compare(mxGetFieldNameByNumber(s_mxArray,k))) {
	    char *s = mxArrayToString(mxGetFieldByNumber(s_mxArray,0,k));
	    mparam.SetValue(mxGetFieldNameByNumber(s_mxArray,k),s);
	    mxFree((void*) s);
	  }
	}
	
	/*init MS OP*/
	Operator *osub;
	OMultistate *matrix = new OMultistate();
	matrix->Init(mparam);
	
	/*Prepare the fieldnames
	* for the Copy procedure */
	int elements = mxGetNumberOfFields(all_Operators);
	std::string *fname = new std::string[elements];
	mwSize dims[2] = {1, 1};
	for (int j=0;j<elements;j++) {
	  fname[j] = mxGetFieldNameByNumber(all_Operators,j); 
	}
	
	for (int s = 0;s<(int) operators;s++) {
	  
	  
	  /*Create StructArray*/
	  mxArray *Operator = mxCreateStructArray(2, dims, elements , (const char **) fname);
	  
	  /*Copy each element importend: Duplicate the Array otherwise a Sec. Fault will occure by clear all*/
	  for (int k =0;k<elements;k++) {
	    mxSetFieldByNumber(Operator,0,k,mxDuplicateArray(mxGetFieldByNumber(all_Operators,s,k))); /*Copy*/
	  }
	  
	  /*If the field "handle" exists remove it (not crucial and to avoid handle confusions)*/
	  int handle_field = mxGetFieldNumber(Operator,"handle");
	  if (handle_field != -1) {
	    mxRemoveField(Operator, handle_field);
	  }
	  
	  /*Check whether the struct contains row and coll Field and set the values*/
	  if (mxGetFieldNumber(Operator, "row") == -1)
	    throw ( EParamProblem("One Operator contains no row field") );
	  if (mxGetFieldNumber(Operator, "col") == -1)
	    throw ( EParamProblem("One Operator contains no col field") );
	  
	  char *str_r = mxArrayToString(mxGetField(Operator, 0,"row"));
	  std::string s_row = str_r; 
	  mxFree((void*) str_r );
	  int row;
	  sscanf( s_row.c_str(), "%d", &row);
	  
	  char *str_c = mxArrayToString(mxGetField(Operator, 0,"col"));
	  std::string s_col = str_c; 
	  mxFree((void*) str_c );
	  int col;
	  sscanf( s_col.c_str(), "%d", &col);
	  
	  /*load sub-OP from struct and add to Multistate*/
	  osub = convert_op_mxArray::loadOP(Operator);
	  matrix->Add( osub, row, col);
	  mxDestroyArray(Operator);
	  
	  
	}
	
	delete[] fname;
	return matrix;
	
      } else if (!s_class.compare("OPSum") || !s_class.compare("Sum")) {
	/*Check the s_mxArray for validity */
	if (mxGetFieldNumber(s_mxArray, "operators") == -1)
	  throw ( EParamProblem("Struct contains no operators field") );
    
	if (mxGetFieldNumber(s_mxArray, "Operator") == -1)
	  throw ( EParamProblem("Struct contains no Operator field") );
	
	char *s = mxArrayToString(mxGetField(s_mxArray, 0,"operators"));
	std::string s_operators = s; 
	mxFree((void*) s );
	
	unsigned int operators;
	sscanf( s_operators.c_str(), "%d",(int*) &operators);
	
	/*Copy the  Field State in new mxArray all_States*/
	mxArray *all_Operators = mxDuplicateArray(mxGetField(s_mxArray,0,"Operator"));
	if (operators != mxGetNumberOfElements(all_Operators))
	  throw ( EParamProblem("Struct contains more of less Operators than defined in the operators Field") );
	
	
	/*Set the ParameterCotainer for the Multistate right (copy the struct-Fields)*/
	std::string name;
	std::string value;
	ParamContainer sparam;
	std::string str_handle = "handle";
	std::string str_State = "Operator";
	int nr_fields = mxGetNumberOfFields(s_mxArray);
	for (int k = 0; k<nr_fields;k++) {
	  if (str_handle.compare(mxGetFieldNameByNumber(s_mxArray,k)) && str_State.compare(mxGetFieldNameByNumber(s_mxArray,k))) {
	    char *s = mxArrayToString(mxGetFieldByNumber(s_mxArray,0,k));
	    sparam.SetValue(mxGetFieldNameByNumber(s_mxArray,k),s);
	    mxFree((void*) s);
	  }
	}
	
	/*init MS OP*/
	Operator *osub;
	OSum *sum = new OSum();
	sum->Init(sparam);
	
	/*Prepare the fieldnames
	* for the Copy procedure */
	int elements = mxGetNumberOfFields(all_Operators);
	std::string *fname = new std::string[elements];
	mwSize dims[2] = {1, 1};
	for (int j=0;j<elements;j++) {
	  fname[j] = mxGetFieldNameByNumber(all_Operators,j); 
	}
	
	for (int s = 0;s<(int) operators;s++) {
	  
	  
	  /*Create StructArray*/
	  mxArray *Operator = mxCreateStructArray(2, dims, elements , (const char **) fname);
	  
	  /*Copy each element importend: Duplicate the Array otherwise a Sec. Fault will occure by clear all*/
	  for (int k =0;k<elements;k++) {
	    mxSetFieldByNumber(Operator,0,k,mxDuplicateArray(mxGetFieldByNumber(all_Operators,s,k))); /*Copy*/
	  }
	  
	  /*If the field "handle" exists remove it (not crucial and to avoid handle confusions)*/
	  int handle_field = mxGetFieldNumber(Operator,"handle");
	  if (handle_field != -1) {
	    mxRemoveField(Operator, handle_field);
	  }
	  
	  
	  /*load sub-OP from struct and add to Multistate*/
	  osub = convert_op_mxArray::loadOP(Operator);
	  sum->Add( osub );
	  mxDestroyArray(Operator);
	  
	  
	}
	delete[] fname;
	return sum;
      } else if (!s_class.compare("OPGridSum") || !s_class.compare("GridSum")) {
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

/**
    * Deletes the OPerator and the corresponding handle
    */
void convert_op_mxArray::delete_op_handle(const mxArray *handle) {
  
  /*delete handle and WF*/
  op_ObjectHandle_interface::deleteOP( handle );
  
}