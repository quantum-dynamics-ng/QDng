#include "qdlib/OGridPotential.h"

using namespace QDLIB;

extern "C" {
   Operator* InstanceOP()
   {
      OGridPotential *p;
      p = new OGridPotential();
      return p;
   }
}