#ifndef op_ObjectHandle_interface_H
#define op_ObjectHandle_interface_H


#include "mex.h"

#include "qdlib/Operator.h"


using namespace QDLIB;

class op_ObjectHandle_interface
   {
      public:
	 static void OP_to_handle_mxArray( mxArray **mx_handle, Operator* OP);
	 static void all_OP_handle_mxArray( mxArray **mx_handle);
	 static Operator*  search_OP ( const mxArray* mxh);
	 static void deleteOP( const mxArray *mxh);
	 static void delete_all_OP();
   };

#endif