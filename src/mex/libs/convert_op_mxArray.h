#ifndef convert_op_mxArray_H
#define convert_op_mxArray_H

#include "mex.h"

#include "qdlib/Operator.h"

using namespace QDLIB;
class convert_op_mxArray
   {
      public:
	 static void init_op(mxArray **out_arg, const mxArray *Struct_mxArray, const mxArray *WF_h_mxArray);
	 static Operator* loadOP(mxArray *s_mxArray);
   };

#endif