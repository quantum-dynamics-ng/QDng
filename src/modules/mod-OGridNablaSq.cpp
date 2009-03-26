#include "qdlib/OGridNablaSq.h"

using namespace QDLIB;

extern "C" {
   Operator* InstanceOP()
   {
      Operator *p;
      p = new OGridNablaSq();
      return p;
   }
}