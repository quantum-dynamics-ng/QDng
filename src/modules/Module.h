#ifndef MODULE_H
#define MODULE_H

#include "qdlib/Operator.h"
using namespace QDLIB;

/**
 * Use this macro to create the API function to load an Operator instance.
 */
#define QDNG_MODULE_OPERATOR_INSTANCE(CLASSNAME) \
extern "C" { \
   Operator* mod_instance_func() \
   { \
      CLASSNAME *p; \
      p = new CLASSNAME (); \
      return p; \
   } \
}

/**
 * Use this macro to create the API function to load a WaveFunction instance.
 */
#define QDNG_MODULE_WAVEFUNCTION_INSTANCE(CLASSNAME) \
extern "C" { \
   WaveFunction* mod_instance_func() \
   { \
      CLASSNAME *p; \
      p = new CLASSNAME (); \
      return p; \
   } \
}


#endif

