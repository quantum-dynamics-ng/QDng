#include "mex/convert_to_mxArray.h"

#include "mex.h"
#include <iostream>
#include <cstring>
#include <string>
#include "qdlib/WaveFunction.h"
#include "qdlib/WFGridCartesian.h"
#include "mex/ObjectHandle.h"
#include "mex/Collector.h"
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
void convert_to_mxArray::WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF) {
	lint stri = WF->strides ();
	lint elem = WF->lsize();
	*mx_WF= mxCreateNumericMatrix(elem,stri,mxDOUBLE_CLASS,mxCOMPLEX);
	double *pdr,*pdi;
	pdr = mxGetPr(mx_WF[0]);
	pdi = mxGetPi(mx_WF[0]);
	for (int s=0; s<stri; s++) {
		dcomplex* val_WF = WF->begin(s);
		for (int p = 0; p<elem; p++) {
			*pdr = val_WF->real();
			*pdi = val_WF->imag();
			val_WF ++;pdr++;pdi++;
		}
	}
}


void convert_to_mxArray::WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF, lint stri) {
	lint elem = WF->lsize();
	*mx_WF= mxCreateNumericMatrix(elem,stri,mxDOUBLE_CLASS,mxCOMPLEX);
	double *pdr,*pdi;
	pdr = mxGetPr(mx_WF[0]);
	pdi = mxGetPi(mx_WF[0]);
	dcomplex* val_WF = WF->begin(stri);
	for (int p = 0; p<elem; p++) {
		*pdr = val_WF->real();
		*pdi = val_WF->imag();
		val_WF ++;pdr++;pdi++;
	}

}

void convert_to_mxArray::WF_make_Struct( mxArray **mx_WF, WaveFunction* WF) {
try {
	mxArray* WF_data;
	convert_to_mxArray::WF_to_mxArray( &WF_data, WF);
	ParamContainer param = WF->Params ();
	param.ResetPosition ();
	std::string name;
	std::string value;
	int i=0;
	while (param.GetNextValue (name, value)) {
		i++;
	}
	int number_of_fields = i+1;
	std::string field_names[number_of_fields];
	field_names[0] = "data";
	i=0;
	param.ResetPosition ();
	while (param.GetNextValue (name, value)) {
		i++;
		field_names[i] = name;
	}
	param.ResetPosition ();
	mwSize dims[2] = {1, 1};
	mx_WF[0]=mxCreateStructArray(2, dims, number_of_fields, (const char **) field_names);
	i=0;
	int field =0;
	while (param.GetNextValue (name, value)) {
		i++;
		field = mxGetFieldNumber(mx_WF[0],(const char*) field_names[i].c_str());
		if (field == -1) throw ( EParamProblem("Error creating WF struct") );
		const char *str[] = {value.c_str()};
		mxArray* text = mxCreateCharMatrixFromStrings(1,str);
		mxSetFieldByNumber(mx_WF[0],0,field,text);
	}
	field = mxGetFieldNumber(mx_WF[0],"data");
	mxSetFieldByNumber(mx_WF[0],0,field,WF_data);
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

void convert_to_mxArray::WF_to_Struct_mxArray( mxArray **mx_WF, WaveFunction* WF) {
	WFMultistate *wfm;
      	wfm = dynamic_cast<WFMultistate*>(WF);
      	if (wfm != NULL){
 		int States = wfm->States();
 		std::string field_names[3];
 		field_names[0] = "CLASS";
 		field_names[1] = "NStates";
 		field_names[2] = "States";
 		mwSize dims[2] = {1, 1};
 		mwSize dims_all[2] = {1, States};
 		mx_WF[0]=mxCreateStructArray(2, dims, 3, (const char **) field_names);
 		int field =0;
 		field = mxGetFieldNumber(mx_WF[0],"CLASS");
 		const char *name[] = {"WFMultistate"};
 		mxArray* text = mxCreateCharMatrixFromStrings(1,name);
 		mxSetFieldByNumber(mx_WF[0],0,field,text);
 		field = mxGetFieldNumber(mx_WF[0],"NStates");
 		std::ostringstream ss;
 		ss << States;
 		const char *nstates[] = {ss.str().c_str()};
 		mxArray* text2 = mxCreateCharMatrixFromStrings(1,nstates);
 		mxSetFieldByNumber(mx_WF[0],0,field,text2);
 		std::string fname[1];
 		fname[0] = "data";
 		mxArray *Struct_States=mxCreateStructArray(2, dims_all, 0, (const char **) fname );
 		for (int i = 0;i<States; i++) {
 			WaveFunction* WF;
 			WF=wfm->State(i);
 			mxArray *Struct_State;
 			convert_to_mxArray::WF_make_Struct(&Struct_State, WF);
 			int NoF = mxGetNumberOfFields((const mxArray *) Struct_State);
 			for (int j=0; j<NoF ;j++) {
 				const char * field_name =mxGetFieldNameByNumber(Struct_State,j);
 				int act_field = mxGetFieldNumber(Struct_States, field_name);
 				if (act_field == -1) {
 					act_field = mxAddField(Struct_States, field_name);
				}
 				mxSetFieldByNumber(Struct_States,i,act_field,mxDuplicateArray(mxGetFieldByNumber(Struct_State,0,j)));
 			}
 		}
		int NoF = mxGetNumberOfFields((const mxArray *) Struct_States);
		for (int i = 0;i<States; i++) {
			for (int j=0; j<NoF ;j++) {
				mxArray *check_value = mxGetFieldByNumber(Struct_States,i,j);
				if (check_value == NULL) {
					const char *null_text[] = {"NULL"};
 					mxArray* mx_null_text = mxCreateCharMatrixFromStrings(1,null_text);
 					mxSetFieldByNumber(Struct_States,i,j,mx_null_text);
				}
			}
		}
		field = mxGetFieldNumber(mx_WF[0],"States");
 		mxSetFieldByNumber(mx_WF[0],0,field,Struct_States);
// 		delete wfm;
//		mx_WF[0]=mxCreateNumericMatrix(1, 1,mxDOUBLE_CLASS,mxCOMPLEX);
	} else {
		convert_to_mxArray::WF_make_Struct(&mx_WF[0], WF);
	}
}

void convert_to_mxArray::WF_to_handle_mxArray( mxArray **mx_handle, WaveFunction* WF) {
	//std::cout << "WaveFunction: " << WF << std::endl;
	ObjectHandle<WaveFunction> *handle = new ObjectHandle<WaveFunction>(WF);
	//std::cout << "WaveFunction: " << WF << std::endl;
	*mx_handle = handle->to_mex_handle();
	
}

void convert_to_mxArray::deleteWF( const mxArray *mxh) {
try {
	ObjectHandle<WaveFunction>* handle = ObjectHandle<WaveFunction>::from_mex_handle( mxh );
	if (handle==NULL) throw ( EParamProblem("WaveFunction not found in Collector") );
        handle->destroy_object(mxh);
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

WaveFunction* convert_to_mxArray::search_WF ( const mxArray* mxh) {
try {
	WaveFunction* WF;
	ObjectHandle<WaveFunction> *handle = ObjectHandle<WaveFunction>::from_mex_handle( mxh );
	if (handle !=NULL) {;
		WF = handle->get_object();
		return WF;
	}
	throw ( EParamProblem("WaveFunction not found in Collector") );
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}
WaveFunction* convert_to_mxArray::loadWF(char *string_File_name , char *string_WF_type) {
   try {
  	std::string sFile_name = string_File_name;
   	std::string sWF_type = string_WF_type;
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
   	std::cout << "Filename: " << file.Name() << std::endl;
	std::cout << "Module: " << sWF_type << std::endl;
   	WaveFunction *WF=NULL;
   	ModuleLoader* mods = ModuleLoader::Instance();
   	WF = mods->LoadWF( sWF_type );
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
   	file >> WF;
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
	return WF;
   } catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
   }
}
