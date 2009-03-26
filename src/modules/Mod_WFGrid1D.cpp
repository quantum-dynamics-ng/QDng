#include "qdlib/.h"

using namespace QDLIB;

extern "C" {
   WaveFunction* InstanceWF()
   {
      WF *p;
      p = new WF ();
      return p;
   }
}