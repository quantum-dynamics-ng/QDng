#include "qdlib/OCheby.h"

using namespace QDLIB;

extern "C" {
   Operator* InstanceOP()
   {
      OCheby *p;
      p = new OCheby ();
      return p;
   }
}