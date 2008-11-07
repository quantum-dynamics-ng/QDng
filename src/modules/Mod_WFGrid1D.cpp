#include "qdlib/WFGrid1D.h"

using namespace QDLIB;

extern "C" {
   WaveFunction* InstanceWF()
   {
      WFGrid1D *p;
      p = new WFGrid1D ();
      return p;
   }
}