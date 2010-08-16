#include "mex.h"

#include "mex/libs/convert_op_mxArray.h"
#include "mex/libs/op_ObjectHandle_interface.h"


/**
    * mexFunction to get all Wavefunction handles
    */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
  if (nlhs == 1 && nrhs == 0) {
     QDMEX::op_ObjectHandle_interface::all_OP_handle_mxArray(&plhs[0]);
   }else mexErrMsgTxt("Bad input. No output and no input argumets needed");
}