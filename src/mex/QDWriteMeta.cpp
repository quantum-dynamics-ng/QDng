/*
 * QDReadMeta.cpp
 *
 *  Created on: 05.08.2011
 *      Author: mko
 */

#include "mex/libs/DataConversion.h"
#include "tools/KeyValFile.h"
#include "tools/Exception.h"
#include "mex.h"

/**
 * Write a matlab struct into a KeyVal/.meta.
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
   if (nlhs > 0)
      mexErrMsgTxt("Function has no return value");

   if (nrhs != 2)
      mexErrMsgTxt("File name and struct needed as input value");

   if ( mxGetClassID(prhs[0]) != mxCHAR_CLASS )
      mexErrMsgTxt("Input argument 1 must be the file name string");

   /* Init KeyValFile */
   QDLIB::KeyValFile file;
   char* key = mxArrayToString(prhs[0]);

   if (key == NULL)
      mexErrMsgTxt("String conversion failed");

   file.SetName(string(key));
   mxFree(key);

   /* Convert and write */
   QDLIB::ParamContainer *pm = QDMEX::DataConversion::MxStructToParamContainer(prhs[1]);

   if (pm == NULL)
      mexErrMsgTxt("ParamContainer Conversion failed");

   if (!file.Write(*pm))
      mexErrMsgTxt("Can not write file");
}
