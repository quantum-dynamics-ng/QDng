/*
 * DataConversion.h
 *
 *  Created on: 05.08.2011
 *      Author: mko
 */

#ifndef DATACONVERSION_H_
#define DATACONVERSION_H_


#include "matrix.h"
#include "tools/ParamContainer.h"

namespace QDMEX {

   class DataConversion {
      public:
         static void ParamContainerSetValue(QDLIB::ParamContainer& pm, const char* key, const mxArray* value);
         static mxArray* ParamContainerToMxStruct(QDLIB::ParamContainer& pm);
         static QDLIB::ParamContainer* MxStructToParamContainer(const mxArray *pmx);
   };
}

#endif /* DATACONVERSION_H_ */
