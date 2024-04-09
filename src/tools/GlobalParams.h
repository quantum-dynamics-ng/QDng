// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBGLOBALPARAMS_H
#define QDLIBGLOBALPARAMS_H

#include "ParamContainer.h"

#define QD_CONFIG_FILE_HOME   ".qdconfig"
#define QD_CONFIG_FILE_LOCAL  ".qdconfig"

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
         static ParamContainer *_ref;

         GlobalParams();
         GlobalParams(const GlobalParams &p){};
         ~GlobalParams(){};
      public:
         static ParamContainer& Instance();
         static void Destroy();
   };

}

#endif
