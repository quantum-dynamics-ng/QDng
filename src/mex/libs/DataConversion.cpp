/*
 * DataConversion.cpp
 *
 *  Created on: 05.08.2011
 *      Author: mko
 */

#include "DataConversion.h"
#include "mex.h"

namespace QDMEX {
   using namespace QDLIB;



   /**
    * Set a value in a parameter container and extract appropriate type matlab type.
    */
   void DataConversion::ParamContainerSetValue(QDLIB::ParamContainer& pm, const char* key, const mxArray* value)
   {
      char *sval;

      switch (mxGetClassID(value)){
         case mxCHAR_CLASS:
            sval = mxArrayToString(value);
            pm.SetValue(key, sval);
            mxFree(sval);
            break;
         case mxDOUBLE_CLASS:
            pm.SetValue(key, *(mxGetPr(value)) );
            break;
         default:
            mexErrMsgTxt("Value has incompatble type");
      }
   }

   /**
    * Convert a Parameter Container to matlab struct.
    * \return NULL if sometthing fails.
    */
   mxArray* DataConversion::ParamContainerToMxStruct(ParamContainer& pm)
   {
      string key, value;
      mwSize dims[] = {1, 1};  /* Matrix size is one - we have only one struct */

      /* Create empty struct */
      mxArray *pmx = mxCreateStructArray(1, dims, 0, NULL);
      if (pmx == NULL)  return NULL;

      /* Step over the elements */
      pm.ResetPosition();
      while (pm.GetNextValue(key, value)){
         mxArray *mxval = mxCreateString(value.c_str());
         if (mxval == NULL)  return NULL;

         int res = mxAddField(pmx, key.c_str());
         if (res == -1 ) return NULL;

         mxSetField(pmx, 0, key.c_str(), mxval);
      }

      return pmx;
   }


   /**
    * Convert a matlab struct to a Parameter Container.
    */
   ParamContainer* DataConversion::MxStructToParamContainer(const mxArray *pmx)
   {
      if (! mxIsStruct(pmx) ) return NULL;

      int nfields = mxGetNumberOfFields(pmx);

      ParamContainer *pm = new ParamContainer();

      for (int i=0; i < nfields; i++){
         /* Field name + content */
         const char *key = mxGetFieldNameByNumber(pmx, i);
         mxArray *val = mxGetFieldByNumber(pmx, 0, i);

         ParamContainerSetValue(*pm, key, val);
      }

      return pm;
   }
}
