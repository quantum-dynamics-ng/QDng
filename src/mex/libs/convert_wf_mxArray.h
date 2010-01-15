#ifndef convert_wf_mxArray_H
#define convert_wf_mxArray_H


#include "mex.h"

#include "qdlib/WaveFunction.h"


using namespace QDLIB;
class convert_wf_mxArray
   {
      public:
	 static void init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_filetype);
	 static void init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_Multistate, const mxArray *mxArray_filetype);
	 static void init_wf_from_file(mxArray **out_arg, const mxArray *mxArray_filename, const mxArray *mxArray_filetype, const mxArray *mxArray_States, const mxArray *mxArray_number);
	 static void init_wf_from_struct(mxArray **out_arg, const mxArray *Struct_mxArray, const mxArray *mxArray_filetype);
	 static void delete_wf_handle(const mxArray *handle);
	 static void get_wf_from_handle(mxArray **out_arg, const mxArray *handle);
	 static void write_wf_to_file(mxArray **out_arg, const mxArray *handle, const mxArray *mxArray_filename);
	 static void write_wf_to_file(mxArray **out_arg, const mxArray *handle, const mxArray *mxArray_filename, const mxArray *mxArray_number);
	 static WaveFunction* loadWF(char *string_File_name , char *string_WF_type);
	 static WaveFunction* loadWF(const mxArray *s_mxArray , char *string_WF_type);
	 static int write_WF(char *string_File_name , WaveFunction* WF);
	 static int write_WF(char *string_File_name , WaveFunction* WF, int number);
	 static void WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF);
	 static void WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF, lint stri);
	 static void WF_make_Struct( mxArray **mx_WF, WaveFunction* WF);
	 static void WF_to_Struct_mxArray( mxArray **mx_WF, WaveFunction* WF);
   };

#endif