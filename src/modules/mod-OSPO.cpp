#include "qdlib/OSPO.h"

using namespace QDLIB;

extern "C" {
   Operator* InstanceOP()
   {
      OSPO *p;
      p = new OSPO();
      return p;
   }
}