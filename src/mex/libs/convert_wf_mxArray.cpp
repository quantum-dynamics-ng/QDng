
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
    * inits Wavefunctions from File(s)
    */
void convert_wf_mxArray::init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename) {
  
  /* If mxArray_filename is a single string */
  if (mxIsChar(mxArray_filename) && ((mxGetN(mxArray_filename) ==1 && mxGetM(mxArray_filename)>1) || (mxGetM(mxArray_filename) ==1 && mxGetN(mxArray_filename) >1 ) ) ) {
    
    char *string_File_name = mxArrayToString(mxArray_filename); /*convert the filename*/
    WaveFunction* WF = convert_wf_mxArray::loadWF(string_File_name); /*load the WF*/
    mxFree(string_File_name);
    wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF);/*return the ObjectHandle*/
  
   /* If mxArray_filename is a string Array*/
  } else if (mxIsChar(mxArray_filename) && mxGetN(mxArray_filename) >1 && mxGetM(mxArray_filename) >1) {
    
    /*convert the string Array to an chr* Array
    *as MATLAB stors this Array in row-major order*/
    int States = mxGetM(mxArray_filename);
    int str_length = mxGetN(mxArray_filename);
    char *string_File_name_all = mxArrayToString(mxArray_filename);
    char **files = new char*[States];
      for (int i =0 ; i<States; i++) {
	files[i] = new char [str_length+1];
	for(int j =0; j<str_length;j++) {
	  files[i][j]=string_File_name_all[i+(States*j)];
	}
	files[i][str_length] = string_File_name_all[States*str_length];
      }
      
      /*create the Multistate WF */
      WaveFunction *wfsub;
      WFMultistate *multi = new WFMultistate();
      for (int i = 0; i < States; i++) {	
	wfsub = convert_wf_mxArray::loadWF( files[i] ); /*load the sub WF*/
	multi->Add( wfsub, i);
	//mxFree(files[i]);
	delete[] files[i];
      }
      
      /*return the ObjectHandle*/
      wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
      mxFree(string_File_name_all);
  
  /*  If mxArray_filename is a cell Array containing strings*/  
  } else if (mxIsCell(mxArray_filename) && (mxGetN(mxArray_filename) == 1 || mxGetM(mxArray_filename) == 1)) {
    
    /*create the Multistate WF */
    int States = mxGetN(mxArray_filename)*mxGetM(mxArray_filename);
    WaveFunction *wfsub;
    WFMultistate *multi = new WFMultistate();
    
    
    for (int i=0; i<States;i++) {
      mxArray *field = mxGetCell(mxArray_filename,i);
      if (mxIsChar(field)) {
	char *file = mxArrayToString(field);
	wfsub = convert_wf_mxArray::loadWF( file );/*load the sub WF*/
	multi->Add( wfsub, i);
	mxFree(file);
      }
    }
    
    
    /*return the ObjectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,multi);
  
  }
}       

/**
    * inits Wavefunctions from File(s) (sequenzed)
    */
void convert_wf_mxArray::init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_number) {
  
  /* If mxArray_filename is a single string */
  if (mxIsChar(mxArray_filename) && ((mxGetN(mxArray_filename) ==1 && mxGetM(mxArray_filename)>1) || (mxGetM(mxArray_filename) ==1 && mxGetN(mxArray_filename) >1 ) ) ) {
    
    char *string_File_name = mxArrayToString(mxArray_filename);
    int Number = (int) *mxGetPr(mxArray_number); /*Convert the squenze Number*/
    
    WaveFunction* WF = convert_wf_mxArray::loadWF(string_File_name, Number); /*load the WF*/
    mxFree(string_File_name);
    
    /*return the ObjectHandle*/
    wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF);
  
  }
}    

/**
    * loads a Wavefunction from structured mxArray
    */
WaveFunction* convert_wf_mxArray::loadWF(const mxArray *s_mxArray) {
   try {
	
	/*Check the s_mxArray for validity */
	if (!mxIsStruct(s_mxArray))
	  throw ( EParamProblem("WaveFunction loading failed due to non structred array") );
	if (mxGetFieldNumber(s_mxArray, "CLASS") == -1)
	  throw ( EParamProblem("WaveFunction contains no CLASS field") );
	/*get CLASS field*/
	mxArray *mx_class = mxGetField(s_mxArray, 0,"CLASS");
	
	/*Check is CLASS is Char*/
	if (mxIsChar(mx_class)) {
	   
	   char *s = mxArrayToString(mx_class);
	   std::string s_class =  s;
	   mxFree((void*) s );
	   
	   /*If class is WFMultistate */ 
	   if (!s_class.compare("WFMultistate")) {
	    
	    /*Check the s_mxArray for validity */
	    if (mxGetFieldNumber(s_mxArray, "states") == -1)
	      throw ( EParamProblem("Struct contains no states field") );
	
	    if (mxGetFieldNumber(s_mxArray, "State") == -1)
	      throw ( EParamProblem("Struct contains no State field") );
	    
	    char *s = mxArrayToString(mxGetField(s_mxArray, 0,"states"));
	    std::string s_States = s; 
	    mxFree((void*) s );
	    
	    unsigned int States;
	    sscanf( s_States.c_str(), "%d",(int*) &States);
	    
	    /*Copy the  Field State in new mxArray all_States*/
	    mxArray *all_States = mxDuplicateArray(mxGetField(s_mxArray,0,"State"));
	    if (States != mxGetNumberOfElements(all_States))
	      throw ( EParamProblem("Struct contains more of less States than defined in the states Parameter") );
	    
	    
	    /*Prepare the fieldnames
	    * for the Copy procedure */
	    int elements = mxGetNumberOfFields(all_States);
	    std::string *fname = new std::string[elements];
	    //std::string fname[elements];
	    mwSize dims[2] = {1, 1};
	    for (int j=0;j<elements;j++) {
	      fname[j] = mxGetFieldNameByNumber(all_States,j); 
	    }
	    
	    /*init MS WF*/
	    WaveFunction *wfsub;
	    WFMultistate *multi = new WFMultistate();
	    
	    for (int s = 0;s<(int)States;s++) {
	      
	      
	      /*Create StructArray*/
	      mxArray *State = mxCreateStructArray(2, dims, elements , (const char **) fname);
	      
	      /*Copy each element importend: Duplicate the Array otherwise a Sec. Fault will occure by clear all*/
	      for (int k =0;k<elements;k++) {
		mxSetFieldByNumber(State,0,k,mxDuplicateArray(mxGetFieldByNumber(all_States,s,k))); /*Copy*/
	      }
	      
	      /*If the field "handle" exists remove it (not crucial and to avoid handle confusions)*/
	      int handle_field = mxGetFieldNumber(State,"handle");
	      if (handle_field != -1) {
		mxRemoveField(State, handle_field);
	      }
	      
	      /*load sub-WF from struct and add to Multistate*/
	      wfsub = convert_wf_mxArray::loadWF( State );
	      multi->Add( wfsub, s);
	      mxDestroyArray(State);
	      //delete[] fname;
	      
	    }
	   
	    
	    /*Set the ParameterCotainer for the Multistate right (copy the struct-Fields)*/
	    std::string name;
	    std::string value;
	    ParamContainer& mparam = multi->Params();
	    std::string str_handle = "handle";
	    std::string str_State = "State";
	    int nr_fields = mxGetNumberOfFields(s_mxArray);
	    for (int k = 0; k<nr_fields;k++) {
	      if (str_handle.compare(mxGetFieldNameByNumber(s_mxArray,k)) && str_State.compare(mxGetFieldNameByNumber(s_mxArray,k))) {
		char *s = mxArrayToString(mxGetFieldByNumber(s_mxArray,0,k));
		mparam.SetValue(mxGetFieldNameByNumber(s_mxArray,k),s);
		mxFree((void*) s);
	      }
	    }
	    delete[] fname;
	    
	    /*Clean up*/
	    mxDestroyArray(all_States);

	    return multi;
	     
	   } else { /*CLASS != WFMultistate*/
	   
	
	      if (mxGetFieldNumber(s_mxArray, "data") == -1)
		throw ( EParamProblem("WaveFunction contains no data field") );
	      
	      /*Get WF-type (e.g.GridCartesian) and nodify it if needed*/
	      char *s = mxArrayToString(mxGetFieldByNumber(s_mxArray,0,mxGetFieldNumber(s_mxArray, "CLASS")));
	      std::string sWF_type = s;
	      mxFree((void*) s);
	      std::string s_WF = "WF";
	      if ( !s_WF.compare(0,2,sWF_type,0,2) ) {
		sWF_type.erase(0,2);
	      }
	      
	      /*init WF*/
	      WaveFunction *WF=NULL;
	      ModuleLoader* mods = ModuleLoader::Instance();
	      WF = mods->LoadWF( sWF_type );
	      
	      /*check the succses*/
	      if (WF == NULL)
		  throw ( EParamProblem("WaveFunction module loading failed") );
	      
	      /*Copy the content of the struct to the ParameterConatainer (exept "data" and "handle")*/
	      int elements = mxGetNumberOfFields(s_mxArray);
	      ParamContainer param;
	      for (int i=0;i<elements;i++) {
		std::string fname = mxGetFieldNameByNumber(s_mxArray,i);
		if (fname.compare("data") && fname.compare("handle") ) {
		  char *s= mxArrayToString(mxGetFieldByNumber(s_mxArray,0,i));
		  param.SetValue((const std::string) fname,(const std::string) s);
		  mxFree((void*) s);
		}
	      }
	      
	      /*init WF by ParameterCantainer*/
	      WF->Init(param);
	      
	      /*Copy the data*/
	      double *pdr,*pdi;
	      pdi = NULL;
	      mxArray * data = mxGetField(s_mxArray,0,"data");
	      pdr = mxGetPr(data);
	      
	      /*if data is complex set pointer to imaginary part*/
	      if (mxIsComplex(data)) {
		pdi = mxGetPi(data);
	      }
	      
	      
	      dcomplex* val_WF = WF->begin(0);
	      int elem = WF->lsize();
	      int length = mxGetM(data);
	      
	      /*check whether sizes are matching */
	      if (elem == length && mxIsComplex(data)) {
		for (int p = 0; p<elem; p++) {
			dcomplex val(*pdr,*pdi); /* if data is complex*/
			*val_WF = val;
			val_WF ++;pdr++;pdi++;
		}
	      } else if (elem == length && !mxIsComplex(data)) {
		for (int p = 0; p<elem; p++) {
			dcomplex val(*pdr,(double) 0.0);/* if data is real*/
			*val_WF = val;
			val_WF ++;pdr++;
		}
	      } else throw ( EParamProblem("WaveFunction inizialization failed") );
	      
	      /*Check whether init was succsesfull*/
	      if (WF == NULL)
		  throw ( EParamProblem("WaveFunction loading failed") );
	      
	      /*return WF*/
	      
	      return WF;
	   }
	} else {throw ( EParamProblem("Field CLASS contains no string"));}
	
   } catch (Exception e) {
     
	mexErrMsgTxt(e.GetMessage().c_str());
	
   }
   return NULL;
}
        


/**
    * inits Wavefunction from MATLAB struct
    */
void convert_wf_mxArray::init_wf_from_struct(mxArray **out_arg, const mxArray *Struct_mxArray) {
  try{ 
      /*Check the s_mxArray for validity */
      if (!mxIsStruct(Struct_mxArray))
	  throw ( EParamProblem("WaveFunction loading failed due to non structred array") );
	
	
      /*If the field "handle" exists remove it (not crucial and to avoid handle confusions)*/
      mxArray *s_mxArray = mxDuplicateArray(Struct_mxArray);
      int handle_field = mxGetFieldNumber(s_mxArray,"handle");
      if (handle_field != -1) {
	mxRemoveField(s_mxArray, handle_field);
      }
      
      /*load WF from struct*/
      WaveFunction* WF = convert_wf_mxArray::loadWF(Struct_mxArray);
      
      if (WF == NULL) 
	throw ( EParamProblem("WaveFunction loading failed") );
      
      /*return the ObjectHandle*/
      wf_ObjectHandle_interface::WF_to_handle_mxArray(out_arg,WF);
      
      /*Clean up*/
      mxDestroyArray(s_mxArray);
      
  } catch (Exception e) {
    
      mexErrMsgTxt(e.GetMessage().c_str());
      
  }
 
}


/**
    * Deletes the Wavefunction and the corresponding handle
    */
void convert_wf_mxArray::delete_wf_handle(const mxArray *handle) {
  
  /*delete handle and WF*/
  wf_ObjectHandle_interface::deleteWF( handle );
  
}

/**
    * Converts the Wavefunction handle to MATLAB mxArray
    */
void convert_wf_mxArray::get_wf_from_handle(mxArray **out_arg,const mxArray *handle) {
  try {
	
	/*Create WF pointer*/
	WaveFunction *WF=NULL;
	
	/*Search for the right pointer and pass it to WF*/
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	
	/*if loading fails*/
	if (WF == NULL)
		 throw ( EParamProblem("WaveFunction loading failed") );
	
	/*else convert it to a MATLAB structur*/
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
    
	/*Create WF pointer*/
	WaveFunction *WF=NULL;
	
	/*Search for the right pointer and pass it to WF*/
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	/*if loading fails*/
	if (WF == NULL)
	  throw ( EParamProblem("WaveFunction loading failed") );
	
	/*convert the Filename and write the WF*/
	char *string_File_name = mxArrayToString(mxArray_filename);
	int succ = convert_wf_mxArray::write_WF(string_File_name,WF);
	
	/* if writing fails*/
	if (succ != 1)
	   throw ( EParamProblem("Writing WaveFunction failed") );
	mxFree(string_File_name);
	
	/*pass throught the returvalue*/
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
    
	/*Create WF pointer*/
	WaveFunction *WF=NULL;
	
	/*Search for the right pointer and pass it to WF*/
	WF = wf_ObjectHandle_interface::search_WF ( handle); 
	//std::cout << "WaveFunction: " << WF << std::endl;
	
	/*if loading fails*/
	if (WF == NULL)
   	 throw ( EParamProblem("WaveFunction loading failed") );
	
	/*convert the Filename and write the WF sequenzed*/
	char *string_File_name = mxArrayToString(mxArray_filename);
	int number = (int) *mxGetPr(mxArray_number);
	int succ = convert_wf_mxArray::write_WF(string_File_name,WF, number);
	
	/* if writing fails*/
	if (succ != 1)
	   throw ( EParamProblem("Writing WaveFunction failed") );
	mxFree(string_File_name);
	
	/*pass throught the returvalue*/
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
  
	/* get the number of strides and the elements in each stride*/
	lint stri = WF->strides ();
	lint elem = WF->lsize();
	
	/*set the mxArray to the right size*/
	*mx_WF= mxCreateNumericMatrix(elem,stri,mxDOUBLE_CLASS,mxCOMPLEX);
	
	/*Set the pointer for real and imaginary part*/
	double *pdr,*pdi;
	pdr = mxGetPr(mx_WF[0]);
	pdi = mxGetPi(mx_WF[0]);
	
	/*Go through all strides*/
	for (int s=0; s<stri; s++) {
	  
		/* set the stride */
		dcomplex* val_WF = WF->begin(s);
		
		/*Go through all points*/
		for (int p = 0; p<elem; p++) {
		  
			*pdr = val_WF->real(); /*Copy real part*/
			*pdi = val_WF->imag(); /*Copy imag part*/
			
			val_WF ++;pdr++;pdi++;/*increase all pointers*/
		}
	}
}


/**
    * Writes data of a single stride of the Wavefunction in a mxArray
    */
void convert_wf_mxArray::WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF, lint stri) {
  
	/* get the elements in each stride*/
	lint elem = WF->lsize();
	
	/*set the mxArray to the right size*/
	*mx_WF= mxCreateNumericMatrix(elem,stri,mxDOUBLE_CLASS,mxCOMPLEX);
	
	/*Set the pointer for real and imaginary part*/
	double *pdr,*pdi;
	pdr = mxGetPr(mx_WF[0]);
	pdi = mxGetPi(mx_WF[0]);
	
	/* set the spesiffic stride */
	dcomplex* val_WF = WF->begin(stri);
	
	/*Go through all points*/
	for (int p = 0; p<elem; p++) {
	  
		*pdr = val_WF->real();/*Copy real part*/
		*pdi = val_WF->imag();/*Copy imag part*/
		
		val_WF ++;pdr++;pdi++;/*increase all pointers*/
	}

}
/**
    * Creates a structured mxArray from a single Wavefunction
    */
void convert_wf_mxArray::WF_make_Struct( mxArray **mx_WF, WaveFunction* WF,const mxArray *handle) {
try {
	
	/* Get data from WF and store them in mxArray WF_data*/
	mxArray* WF_data;
	convert_wf_mxArray::WF_to_mxArray( &WF_data, WF);
	
	/*Get the handle and store them in mxArray WF_handle*/
	mxArray* WF_handle;
	WF_handle = mxDuplicateArray(handle);
	
	/*Get the ParameterContainer of WF*/
	ParamContainer param = WF->Params ();
	param.ResetPosition ();
	std::string name;
	std::string value;
	
	/*count elements in the ParameterContainer*/
	int i=0;
	while (param.GetNextValue (name, value)) {
		i++;
	}
	
	/*set the number of fields in the struct mxArray*/
	int number_of_fields = i+2;
	
	/*Set the fiednames form ParameterContainer and the one added (handle and data) */
	std::string *field_names = new std::string[number_of_fields];
	field_names[0] = "data";
	field_names[number_of_fields-1] = "handle";
	i=0;
	param.ResetPosition ();
	while (param.GetNextValue (name, value)) {
		i++;
		field_names[i] = name;
	}
	param.ResetPosition ();
	
	/*Create the struct mxArray with the right size*/
	mwSize dims[2] = {1, 1};
	mx_WF[0]=mxCreateStructArray(2, dims, number_of_fields, (const char **) field_names);
	
	/*Copy the content of the ParameterContainer into the struct*/
	i=0;
	int field =0;
	
	while (param.GetNextValue (name, value)) {
		i++;
		field = mxGetFieldNumber(mx_WF[0],(const char*) field_names[i].c_str()); /*Get fieldnumber by name (from ParameterContainer)*/
		if (field == -1) throw ( EParamProblem("Error creating WF struct") ); /*if field exists*/
		const char *str[] = {value.c_str()}; /*convert the string*/
		mxArray* text = mxCreateCharMatrixFromStrings(1,str);
		mxSetFieldByNumber(mx_WF[0],0,field,text);/*Set the field*/
	}
	
	/*Set the field "data"*/
	field = mxGetFieldNumber(mx_WF[0],"data");
	mxSetFieldByNumber(mx_WF[0],0,field,WF_data);
	
	/*Set the field "handle*/
	field = mxGetFieldNumber(mx_WF[0],"handle");
	mxSetFieldByNumber(mx_WF[0],0,field,WF_handle);
	
	delete[] field_names;
	
}catch (Exception e) {
	mexErrMsgTxt(e.GetMessage().c_str());
}
}

/**
    * Creates a structured mxArray from a Wavefunction e.g Multistate
    */
void convert_wf_mxArray::WF_to_Struct_mxArray( mxArray **mx_WF, WaveFunction* WF,const mxArray *handle) {
  
	/*Check whether WF is Multistate*/
	WFMultistate *wfm;
      	wfm = dynamic_cast<WFMultistate*>(WF);
	
	/*if WF is Multistate*/
      	if (wfm != NULL){
		
		/*Set inital walues for the Multistate struct*/
 		int States = wfm->States();
 		std::string field_names[4];
 		field_names[0] = "CLASS"; /*CLASS name*/
		field_names[1] = "handle"; /*handle*/
 		field_names[2] = "states"; /*Nr of states*/
 		field_names[3] = "State"; /*Array containing a struct for each State*/ 
 		mwSize dims[2] = {1, 1};
 		mwSize dims_all[2] = {1, States};
		
		/*Create the Multistate struct*/
 		mx_WF[0]=mxCreateStructArray(2, dims, 4, (const char **) field_names);
		
		/*set Value for field  "CLASS"*/
 		int field =0;
 		field = mxGetFieldNumber(mx_WF[0],"CLASS");
 		const char *name[] = {"WFMultistate"};
 		mxArray* text = mxCreateCharMatrixFromStrings(1,name);
 		mxSetFieldByNumber(mx_WF[0],0,field,text);
		
		/*set Value for field "handle"*/
		field = mxGetFieldNumber(mx_WF[0],"handle");
 		mxSetFieldByNumber(mx_WF[0],0,field,mxDuplicateArray(handle));
		
		/*set Value for field "states"*/
 		field = mxGetFieldNumber(mx_WF[0],"states");
 		std::ostringstream ss;
 		ss << States;
 		const char *nstates[] = {ss.str().c_str()};
 		mxArray* text2 = mxCreateCharMatrixFromStrings(1,nstates);
 		mxSetFieldByNumber(mx_WF[0],0,field,text2);
		
		/*Set the field names for the sub-structs (sub-WFs) per default needed a field called data; all othe will be copied from the ParameterContainr*/
 		std::string fname[1];
 		fname[0] = "data";
		
		/*Create StructArray for all States*/
 		mxArray *Struct_States=mxCreateStructArray(2, dims_all, 0, (const char **) fname );
		
		/*Create a struct mxArray for each state and fill it in the Multistate struct*/
 		for (int i = 0;i<States; i++) {
		  
			/*Create WF pointer*/
 			WaveFunction* WF;
 			WF=wfm->State(i);
			
			/*Create a struct mxArray for each state*/
 			mxArray *Struct_State;
			
			/*Fill the struct mxArray*/
 			convert_wf_mxArray::WF_make_Struct(&Struct_State, WF,handle);
			
			/*Get Nr of Fields*/
 			int NoF = mxGetNumberOfFields((const mxArray *) Struct_State);
			
			/*Check whether all field in Struct_State are present in Struct_States */
 			for (int j=0; j<NoF ;j++) {
			  
 				const char * field_name =mxGetFieldNameByNumber(Struct_State,j);
 				int act_field = mxGetFieldNumber(Struct_States, field_name);
				
				/*if not present Add the field*/
 				if (act_field == -1) {
 					act_field = mxAddField(Struct_States, field_name);
				}
				
				/*Copy content from Struct_State in Struct_States (array structs containing all states) */
 				mxSetFieldByNumber(Struct_States,i,act_field,mxDuplicateArray(mxGetFieldByNumber(Struct_State,0,j)));
 			}
			
			/*Clean up*/
			mxDestroyArray(Struct_State);
 		}
		
		/*Clean up the struct Array Struct_States*/
		int NoF = mxGetNumberOfFields((const mxArray *) Struct_States);
		
		/*Loop over all States*/
		for (int i = 0;i<States; i++) {
		  
			/*loop over the fields */
			for (int j=0; j<NoF ;j++) {
				
				/*Get the value of each field*/
				mxArray *check_value = mxGetFieldByNumber(Struct_States,i,j);
				
				/*if field is empty*/
				if (check_value == NULL) {
					
					/*set value to text "NULL"*/
					const char *null_text[] = {"NULL"};
 					mxArray* mx_null_text = mxCreateCharMatrixFromStrings(1,null_text);
 					mxSetFieldByNumber(Struct_States,i,j,mx_null_text);
				}
			}
			
			/* if in the states the field "handle" is present remove it handle is aleady present in the Multistate struct*/
			int act_field = mxGetFieldNumber(Struct_States,"handle");
			if (act_field != -1) {
			  
			  mxRemoveField(Struct_States, act_field);
			  
			}
		}
		
		/*Set the array of structs (Struct_States) in the output struct*/
		field = mxGetFieldNumber(mx_WF[0],"State");
 		mxSetFieldByNumber(mx_WF[0],0,field,Struct_States);
		
		/*Copy the contents of the ParameterContainer in the output struct*/
		std::string m_name;
		std::string value;
		ParamContainer mparam = wfm->Params ();
		mparam.ResetPosition ();
		
		/*Loop over all elements in the Container*/
		while (mparam.GetNextValue (m_name, value)) {
		  
		  /*Check whether field exists*/
		  const char * field_name =m_name.c_str();
		  int act_field = mxGetFieldNumber(mx_WF[0], field_name);
		  
		  /*if not add it*/
		  if (act_field == -1) {
		    
 			act_field = mxAddField(mx_WF[0], field_name);
			const char* content = value.c_str();
			
			mxArray *param_content=mxCreateString(content);
			mxSetFieldByNumber(mx_WF[0],0,act_field,mxDuplicateArray(param_content));
			/*Clean up*/
			mxDestroyArray(param_content);
		  }
		}
	} else {
		
		/*if no Multistate WF directly make the struct*/
		convert_wf_mxArray::WF_make_Struct(&mx_WF[0], WF,handle);
	}
}

/**
    * loads a Wavefunction from file
    */
WaveFunction* convert_wf_mxArray::loadWF(char *string_File_name) {
   try {
     
	/*convert Filename*/
  	std::string sFile_name = string_File_name;
	
	/*init FileWF object*/
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
   	std::cout << "Filename: " << file.Name()  << std::endl;
	
	/*init WF*/
   	WaveFunction *WF=NULL;
	
	/*write File to WF*/
   	file >> &WF;
	
	/*Check whether writing was succsessfull*/
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
	
	/*return WF*/
	return WF;
	
   } catch (Exception e) {
	mexErrMsgTxt(e.GetMessage().c_str());
   }
   return NULL;
}

/**
    * loads a Wavefunction from file (sequenzed)
    */
WaveFunction* convert_wf_mxArray::loadWF(char *string_File_name, int Number) {
   try {
     
	/*convert Filename*/
  	std::string sFile_name = string_File_name;
	
	/*init FileWF object (activate sequenze and set counter)*/
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
	file.ActivateSequence ();
	file.Counter(Number);
   	std::cout << "Filename: " << file.Name()  << std::endl;
	
	/*init WF*/
   	WaveFunction *WF=NULL;
	
	/*write File to WF*/
   	file >> &WF;
	
	/*Check whether writing was succsessfull*/
	if (WF == NULL)
	    throw ( EParamProblem("WaveFunction loading failed") );
	
	/*return WF*/
	return WF;
   } catch (Exception e) {
	mexErrMsgTxt(e.GetMessage().c_str());
   }
   return NULL;
}


/**
    * writes a Wavefunction to file
    */
int convert_wf_mxArray::write_WF(char *string_File_name , WaveFunction* WF){
   try {
	
	/*Convert Filename */
  	std::string sFile_name = string_File_name;
	
	/* init FileWF object*/
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
   	std::cout << "Filename: " << file.Name() << std::endl;
	
	 /* Write wavefunction */
        file << WF; 
	
	/*return succsess*/
	return 1;
	
   } catch (Exception e) {
     
	mexErrMsgTxt(e.GetMessage().c_str());
	
   }
   return 0;
}

/**
    * writes a Wavefunction to file (sequenced)
    */
int convert_wf_mxArray::write_WF(char *string_File_name , WaveFunction* WF, int number){
   try {
     
	/*Convert Filename */
  	std::string sFile_name = string_File_name;
	
	/* init FileWF object (sequenced)*/
   	FileWF file;
   	file.Suffix(BINARY_WF_SUFFIX);
   	file.Name(sFile_name);
	file.ActivateSequence();
	file.Counter(number);
	std::cout << "Filename: " << file.Name() << std::endl;
	
	/* Write wavefunction */
        file << WF;  
	
	/*return succsess*/
	return 1;
	
   } catch (Exception e) {
     
	mexErrMsgTxt(e.GetMessage().c_str());
	
   }
   return 0;
}

