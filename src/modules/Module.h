#ifndef MODULE_H
#define MODULE_H

/**
 * Use this macro to create the API function to load an Operator instance.
 */
#define QDNG_MODULE_OPERATOR_INSTANCE (CLASSNAME) \
extern "C" { \
   Operator* InstanceOP() \
   { \
      CLASSNAME *p; \
      p = new CLASSNAME(); \
      return p; \
   } \
} 


/**
 * Use this macro to create the API function to load a WaveFunction instance.
 */
#define QDNG_MODULE_OPERATOR_INSTANCE (CLASSNAME) \
extern "C" { \
   WaveFunction* InstanceWF() \
   { \
      CLASSNAME *p; \
      p = new CLASSNAME(); \
      return p; \
   } \
} 

#endif
