#include "qdlib/WFGridCartesian.h"

using namespace QDLIB;

extern "C" {
   WaveFunction* InstanceWF()
   {
      WFGridCartesian *p;
      p = new WFGridCartesian();
      return p;
   }
}