#ifndef QDLIBGLOBALPARAMS_H
#define QDLIBGLOBALPARAMS_H

#include "ParamContainer.h"

namespace QDLIB {

   /**
    * Singleton for Parameter Container.
    * 
    * This is used to exchanged global parameters.
    * 
    *     @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class GlobalParams {
      private:
         GlobalParams(){};
         GlobalParams(const GlobalParams &p){};
         ~GlobalParams(){};
      public:
         static ParamContainer& Instance();
         static void Destroy();
   };

}

#endif
