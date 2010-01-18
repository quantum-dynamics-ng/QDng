#ifndef wf_ObjectHandle_interface_H
#define wf_ObjectHandle_interface_H


#include "mex.h"

#include "qdlib/WaveFunction.h"


using namespace QDLIB;

class wf_ObjectHandle_interface
   {
      public:
	 static void WF_to_handle_mxArray( mxArray **mx_handle, WaveFunction* WF);
	 static void all_WF_handle_mxArray( mxArray **mx_handle);
	 static WaveFunction*  search_WF ( const mxArray* mxh);
	 static void deleteWF( const mxArray *mxh);
	 static void delete_all_WF();
   };

#endif