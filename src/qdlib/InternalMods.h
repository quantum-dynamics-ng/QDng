#ifndef QDLIBINTERNALMODS_H
#define QDLIBINTERNALMODS_H

#include "Operator.h"

namespace QDLIB
{

   typedef WaveFunction* (instWF)();
   typedef Operator* (instOP)();
   
   
   extern instWF* InternalWF(const string &name);
   extern instOP* InternalOP(const string &name);

}

#endif
