#ifndef convert_to_mxArray_H
#define convert_to_mxArray_H


#include "mex.h"

#include "qdlib/WaveFunction.h"


using namespace QDLIB;
class convert_to_mxArray
   {
      public:
	 static WaveFunction* loadWF(char *string_File_name , char *string_WF_type);
	 static void WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF);
	 static void WF_to_mxArray( mxArray **mx_WF, WaveFunction* WF, lint stri);
	 static void WF_make_Struct( mxArray **mx_WF, WaveFunction* WF);
	 static void WF_to_Struct_mxArray( mxArray **mx_WF, WaveFunction* WF);
	 static void WF_to_handle_mxArray( mxArray **mx_handle, WaveFunction* WF);
	 static WaveFunction*  search_WF ( const mxArray* mxh);
	 static void deleteWF( const mxArray *mxh);
   };

#endif