#include "GlobalParams.h"

namespace QDLIB {

   ParamContainer& GlobalParams::Instance()
   {
      if (_ref == NULL){
         _ref = new ParamContainer();
      }
      return *_ref;
   }

   void GlobalParams::Destroy()
   {
      if (_ref != NULL){
         delete _ref;
         _ref = NULL;
      }
   }

   ParamContainer* GlobalParams::_ref = 0;

}

