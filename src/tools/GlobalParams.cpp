#include "GlobalParams.h"

namespace QDLIB {

   ParamContainer& GlobalParams::Instance()
   {
      static ParamContainer ref;
      return ref;
   }

   void GlobalParams::Destroy()
   {
      Instance().clear();
   }

}

