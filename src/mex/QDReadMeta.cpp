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
 * Read a KeyVal/.meta file a convert it to a matlab struct.
 */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
   if (nlhs != 1)
      mexErrMsgTxt("No return value defined!");

   if (nrhs != 1)
      mexErrMsgTxt("File name needed as input value");

   if ( mxGetClassID(prhs[0]) != mxCHAR_CLASS )
      mexErrMsgTxt("Input argument 1 must be string");

   /* Init KeyValFile */
   QDLIB::KeyValFile file;
   char* key = mxArrayToString(prhs[0]);

   if (key == NULL)
      mexErrMsgTxt("String conversion failed");

   file.SetName(string(key));
   mxFree(key);

   /* Parse & convert */
   QDLIB::ParamContainer pm;
   if (!file.Parse(pm))
      mexErrMsgTxt("Can not parse file");

   plhs[0] =  QDMEX::DataConversion::ParamContainerToMxStruct(pm);
   if (plhs[0]  == NULL)
      mexErrMsgTxt("Conversion failed");
}
