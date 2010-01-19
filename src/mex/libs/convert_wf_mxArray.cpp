
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
    * all functions that interact with mxArray are placed here.
    */

using namespace QDLIB;

/**
    * inits single Wavefunction from File
    */
void convert_wf_mxArray::init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_filetype) {
  char *string_File_name = mxArrayToString(mxArray_filename);
  char *string_WF_type = mxArrayToString(mxArray_filetype);
  WaveFunction* WF = convert_wf_mxArray::loadWF(string_File_name,string_WF_type);
  mxFree(string_File_name);
  mxFree(string_WF_type);
  wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF);
}

/**
    * inits Multistate Wavefunction from Files
    */
void convert_wf_mxArray::init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_filetype, const mxArray *mxArray_States, const mxArray *mxArray_number) {

  char *string_Base_File_name = mxArrayToString(mxArray_filename);
  char *string_WF_type = mxArrayToString(mxArray_filetype);
  int States = (int) *mxGetPr(mxArray_States);
  int Number = (int) *mxGetPr(mxArray_number);
  WaveFunction *wfsub;
  WFMultistate *multi = new WFMultistate();
  for (int i = 0; i<States; i++) {
	std::ostringstream ss;
	ss << string_Base_File_name << "-" << i << "_" << Number ;
	char *string_File_name = new char [ss.str().size()+1];
	strcpy (string_File_name,ss.str().c_str());
	wfsub = convert_wf_mxArray::loadWF( string_File_name , string_WF_type );
	multi->Add( wfsub, i);	
	mxFree(string_File_name);	
  }
  //std::cout << "Wavefunction: " << multi << std::endl;
  wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
  mxFree(string_Base_File_name);
  mxFree(string_WF_type);

}

/**
    * inits single Wavefunction from File-Array
    */
void convert_wf_mxArray::init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_Multisiate, const mxArray *mxArray_filetype) {
  if (mxIsChar(mxArray_filename)) {
    int States = mxGetM(mxArray_filename);
    int str_length = mxGetN(mxArray_filename);
    if (States > 1) {
	  char *string_File_name_all = mxArrayToString(mxArray_filename);
	  char *files[States];
	  for (int i =0 ; i<States; i++) {
		  files[i] = new char [str_length+1];
		  for(int j =0; j<str_length;j++) {
			  files[i][j]=string_File_name_all[i+(States*j)];
		  }
		  files[i][str_length] = string_File_name_all[States*str_length];
	  }
	  char *string_WF_type = mxArrayToString(mxArray_filetype);
	  WaveFunction *wfsub;
	  WFMultistate *multi = new WFMultistate();
	  for (int i = 0; i < States; i++) {	
		  wfsub = convert_wf_mxArray::loadWF( files[i], string_WF_type );
		  multi->Add( wfsub, i);
		  mxFree(files[i]);
	  }
	  wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
	  mxFree(string_File_name_all);
	  mxFree(string_WF_type);
    } else mexErrMsgTxt("Bad input. No Multistate WF");
  } else if (mxIsCell(mxArray_filename) && (mxGetN(mxArray_filename) == 1 || mxGetM(mxArray_filename) == 1)) {
    int States = mxGetN(mxArray_filename)*mxGetM(mxArray_filename);
    char *string_WF_type = mxArrayToString(mxArray_filetype);
    WaveFunction *wfsub;
    WFMultistate *multi = new WFMultistate();
    for (int i=0; i<States;i++) {
      mxArray *field = mxGetCell(mxArray_filename,i);
      if (mxIsChar(field)) {
	char *file = mxArrayToString(field);
	wfsub = convert_wf_mxArray::loadWF( file, string_WF_type );
	multi->Add( wfsub, i);
	mxFree(file);
      }
    }
    wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
    mxFree(string_WF_type);
  }
}


/**
    * inits Wavefunction from MATLAB struct
    */
void convert_wf_mxArray::init_wf_from_struct(mxArray **out_arg, const mxArray *Struct_mxArray, const mxArray *mxArray_filetype) {
    char *string_WF_type = mxArrayToString(mxArray_filetype);
    int nr_fields = mxGetNumberOfFields(Struct_mxArray);
    bool is_multi = false;
    for (int i = 0; i<nr_fields;i++) {
      mxArray *act_field = mxDuplicateArray(mxGetFieldByNumber(Struct_mxArray,0,i));
      if (mxIsStruct(act_field)) {
	is_multi = true;
	WaveFunction *wfsub;
 	WFMultistate *multi = new WFMultistate();
	int States = mxGetNumberOfElements(act_field);
	int elements = mxGetNumberOfFields(act_field);
	std::string fname[elements];
	mwSize dims[2] = {1, 1};
	for (int j=0;j<elements;j++) {
	    fname[j] = mxGetFieldNameByNumber(act_field,j);
	}
	for (int j=0;j<States;j++) { 
	  mxArray *State = mxCreateStructArray(2, dims, elements , (const char **) fname);
	  for (int k =0;k<elements;k++) {
	    mxSetFieldByNumber(State,0,k,mxDuplicateArray(mxGetFieldByNumber(act_field,j,k)));
	  }
	  int handle_field = mxGetFieldNumber(State,"handle");
	  if (handle_field != -1) {
	    mxRemoveField(State, handle_field);
	  }
	  wfsub = convert_wf_mxArray::loadWF( State , string_WF_type );
		  multi->Add( wfsub, j);
	}
	std::string name;
	std::string value;
	ParamContainer mparam = multi->Params ();
	std::string str_handle = "handle";
	for (int k = 0; k<nr_fields;k++) {
	  if (k==i) continue;
	    if (str_handle.compare(mxGetFieldNameByNumber(Struct_mxArray,k))) {
	      mparam.SetValue(mxGetFieldNameByNumber(Struct_mxArray,k),mxArrayToString(mxGetFieldByNumber(Struct_mxArray,0,k)));
	    }
	}
	wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
      }
    }
    if (is_multi == false) {
      mxArray *State = mxDuplicateArray(Struct_mxArray);
      int handle_field = mxGetFieldNumber(State,"handle");
      if (handle_field != -1) {
	mxRemoveField(State, handle_field);
      }
      WaveFunction* WF = convert_wf_mxArray::loadWF(State,string_WF_type);
      wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF);
    }
    mxFree(string_WF_type); 
}

/**
    * Deletes the Wavefunction and the corresponding handle
    */
void convert_wf_mxArray::delete_wf_handle(const mxArray *handle) {
  wf_ObjectHandle_interface::deleteWF( handle );
}

/**
    * Converts the Wavefunction handle to MATLAB mxArray
    */
void convert_wf_mxArray::get_wf_from_handle(mxArray **out_arg,const mxArray *handle) {
  try {
	WaveFunction *WF=NULL;
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	if (WF == NULL)
		 throw ( EParamProblem("WaveFunction loading failed") );
	convert_wf_mxArray::WF_to_Struct_mxArray( out_arg, WF , handle);
  } catch (Exception e) {
	mexErrMsgTxt(e.GetMessage().c_str());
  }
}


/**
    * Writes the Wavefunction handle to File
    */
void convert_wf_mxArray::write_wf_to_file(mxArray **out_arg, const mxArray *handle, const mxArray *mxArray_filename) {
  try {
	WaveFunction *WF=NULL;
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	if (WF == NULL)
	  throw ( EParamProblem("WaveFunction loading failed") );
	char *string_File_name = mxArrayToString(mxArray_filename);
	int succ = convert_wf_mxArray::write_WF(string_File_name,WF);
	if (succ != 1)
	   throw ( EParamProblem("Writing WaveFunction failed") );
	mxFree(string_File_name);
	*out_arg = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
	double *pdr;
	pdr = mxGetPr(*out_arg);
	*pdr = (double) succ;
  } catch (Exception e) {
	 mexErrMsgTxt(e.GetMessage().c_str());
  }
}

/**
    * Writes the Wavefunction handle to File (sequenced)
    */
void convert_wf_mxArray::write_wf_to_file(mxArray **out_arg, const mxArray *handle, const mxArray *mxArray_filename, const mxArray *mxArray_number) {
  try {
	WaveFunction *WF=NULL;
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	if (WF == NULL)
   	 throw ( EParamProblem("WaveFunction loading failed") );
	char *string_File_name = mxArrayToString(mxArray_filename);
	int number = (int) *mxGetPr(mxArray_number);
	int succ = convert_wf_mxArray::write_WF(string_File_name,WF, number);
	if (succ != 1)
	   throw ( EParamProblem("Writing WaveFunction failed") );
	mxFree(string_File_name);
	*out_arg = mxCreateNumericMatrix(1,1,mxDOUBLE_CLASS,mxREAL);
	double *pdr;
        pdr = mxGetPr(*out_arg);
	*pdr = (double) succ;
  } catch (Exception e) {
	 mexErrMsgTxt(e.GetMessage().c_str());
  }
}


 /**
    * Writes data of the Wavefunction in a mxArray
    */
void convert_wf_mxArray::WF_to_mxArray(mxArray **mx_WF, WaveFunction* WF) {
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


/**
    * Writes data of a single stride of the Wavefunction in a mxArray
    */
void convert_wf_mxArray::WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF, lint stri) {
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
/**
    * Creates a structured mxArray from a single Wavefunction
    */
void convert_wf_mxArray::WF_make_Struct( mxArray **mx_WF, WaveFunction* WF,const mxArray *handle) {
try {
	mxArray* WF_data;
	convert_wf_mxArray::WF_to_mxArray( &WF_data, WF);
	mxArray* WF_handle;
	WF_handle = mxDuplicateArray(handle);
	ParamContainer param = WF->Params ();
	param.ResetPosition ();
	std::string name;
	std::string value;
	int i=0;
	while (param.GetNextValue (name, value)) {
		i++;
	}
	int number_of_fields = i+2;
	std::string field_names[number_of_fields];
	field_names[0] = "data";
	field_names[number_of_fields-1] = "handle";
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
	field = mxGetFieldNumber(mx_WF[0],"handle");
	mxSetFieldByNumber(mx_WF[0],0,field,WF_handle);
}catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

/**
    * Creates a structured mxArray from a Wavefunction e.g Multistate
    */
void convert_wf_mxArray::WF_to_Struct_mxArray( mxArray **mx_WF, WaveFunction* WF,const mxArray *handle) {
	WFMultistate *wfm;
      	wfm = dynamic_cast<WFMultistate*>(WF);
      	if (wfm != NULL){
 		int States = wfm->States();
 		std::string field_names[4];
 		field_names[0] = "CLASS";
		field_names[1] = "handle";
 		field_names[2] = "NStates";
 		field_names[3] = "States";
 		mwSize dims[2] = {1, 1};
 		mwSize dims_all[2] = {1, States};
 		mx_WF[0]=mxCreateStructArray(2, dims, 4, (const char **) field_names);
 		int field =0;
 		field = mxGetFieldNumber(mx_WF[0],"CLASS");
 		const char *name[] = {"WFMultistate"};
 		mxArray* text = mxCreateCharMatrixFromStrings(1,name);
 		mxSetFieldByNumber(mx_WF[0],0,field,text);
		field = mxGetFieldNumber(mx_WF[0],"handle");
 		mxSetFieldByNumber(mx_WF[0],0,field,mxDuplicateArray(handle));
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
 			convert_wf_mxArray::WF_make_Struct(&Struct_State, WF,handle);
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
			int act_field = mxGetFieldNumber(Struct_States,"handle");
			if (act_field != -1) {
			  mxRemoveField(Struct_States, act_field);
			}
		}
		field = mxGetFieldNumber(mx_WF[0],"States");
 		mxSetFieldByNumber(mx_WF[0],0,field,Struct_States);
	} else {
		convert_wf_mxArray::WF_make_Struct(&mx_WF[0], WF,handle);
	}
}

/**
    * loads a Wavefunction from file
    */
WaveFunction* convert_wf_mxArray::loadWF(char *string_File_name , char *string_WF_type) {
   try {
  	std::string sFile_name = string_File_name;
   	std::string sWF_type = string_WF_type;
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
   	std::cout << "Filename: " << file.Name() << " Module: " << sWF_type << std::endl;
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

/**
    * loads a Wavefunction from structured mxArray
    */
WaveFunction* convert_wf_mxArray::loadWF(const mxArray *s_mxArray , char *string_WF_type) {
   try {
	if (!mxIsStruct(s_mxArray))
	  throw ( EParamProblem("WaveFunction loading failed due to non structred array") );
	if (mxGetFieldNumber(s_mxArray, "data") == -1)
	  throw ( EParamProblem("WaveFunction contains no data field") );
   	std::string sWF_type = string_WF_type;
   	WaveFunction *WF=NULL;
   	ModuleLoader* mods = ModuleLoader::Instance();
   	WF = mods->LoadWF( sWF_type );
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction module loading failed") );
   	int elements = mxGetNumberOfFields(s_mxArray);
	ParamContainer param;
	for (int i=0;i<elements;i++) {
	  std::string fname = mxGetFieldNameByNumber(s_mxArray,i);
	  if (fname.compare("data") && fname.compare("handle") ) {
	    param.SetValue((const std::string) fname,(const std::string) mxArrayToString(mxGetFieldByNumber(s_mxArray,0,i)));
	  }
	}
	WF->Init(param);
	double *pdr,*pdi;
	mxArray * data = mxGetField(s_mxArray,0,"data");
	pdr = mxGetPr(data);
	if (mxIsComplex(data)) {
	  pdi = mxGetPi(data);
	}
	dcomplex* val_WF = WF->begin(0);
	int elem = WF->lsize();
	int length = mxGetM(data);
	if (elem == length && mxIsComplex(data)) {
	  for (int p = 0; p<elem; p++) {
		  dcomplex val(*pdr,*pdi);
		  *val_WF = val;
		  val_WF ++;pdr++;pdi++;
	  }
	} else if (elem == length && !mxIsComplex(data)) {
	  for (int p = 0; p<elem; p++) {
		  dcomplex val(*pdr,(double) 0.0);
		  *val_WF = val;
		  val_WF ++;pdr++;
	  }
	} else throw ( EParamProblem("WaveFunction inizialization failed") );
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
	return WF;
   } catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
   }
}

/**
    * writes a Wavefunction to file
    */
int convert_wf_mxArray::write_WF(char *string_File_name , WaveFunction* WF){
   try {
  	std::string sFile_name = string_File_name;
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
   	std::cout << "Filename: " << file.Name() << std::endl;
	WFMultistate *wfm;
           wfm = dynamic_cast<WFMultistate*>(WF);
           if (wfm != NULL){
              ParamContainer& pfm = wfm->Params();
              pfm = wfm->State(0)->Params();
           }
           file << WF;  /* Write wavefunction */
	   return 1;
   } catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
   }
}

/**
    * writes a Wavefunction to file (sequenced)
    */
int convert_wf_mxArray::write_WF(char *string_File_name , WaveFunction* WF, int number){
   try {
  	std::string sFile_name = string_File_name;
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
	file.ActivateSequence();
	file.Counter(number);
   	std::cout << "Filename: " << file.Name() << std::endl;
	WFMultistate *wfm;
           wfm = dynamic_cast<WFMultistate*>(WF);
           if (wfm != NULL){
              ParamContainer& pfm = wfm->Params();
              pfm = wfm->State(0)->Params();
           }
           file << WF;  /* Write wavefunction */
	   return 1;
   } catch (Exception e) {
	std::cout << e.GetMessage() << std::endl;
	mexErrMsgTxt(e.GetMessage().c_str());
   }
}

