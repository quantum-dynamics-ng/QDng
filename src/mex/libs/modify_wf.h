#ifndef modify_wf_H
#define modify_wf_H


#include "mex.h"

#include "qdlib/WaveFunction.h"


using namespace QDLIB;

class modify_wf
   {
      public:
	 static void add_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2);
	 static void sub_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2);
	 static void mult_wf_double(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_double);
	 static void mult_wf_pointwise(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_double);
	 static void scalar_prod_wf(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2);
	 static void mult_wf_complex(mxArray **handle_result, mxArray *handle_WF1, mxArray *handle_WF2);
	 static void test_wf(mxArray **handle_result, mxArray *handle_WF1);
	 static void norm_wf(mxArray **handle_result, mxArray *handle_WF1);
	 
	 static void add_wf(mxArray *handle_WF1, mxArray *handle_WF2);
	 static void sub_wf(mxArray *handle_WF1, mxArray *handle_WF2);
	 static void mult_wf_double(mxArray *handle_WF1, mxArray *handle_double);
	 static void mult_wf_pointwise(mxArray *handle_WF1, mxArray *handle_double);
	 static void mult_wf_complex(mxArray *handle_WF1, mxArray *handle_WF2);
	 static void norm_wf(mxArray *handle_WF1);
   };

#endif