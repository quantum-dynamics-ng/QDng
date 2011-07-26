#include "GlobalParams.h"
#include "fstools.h"
#include "KeyValFile.h"
#include <stdlib.h>

namespace QDLIB {

   ParamContainer& GlobalParams::Instance()
   {
      if (_ref == NULL)
      {
         _ref = new ParamContainer();

         KeyValFile config;

         cout << "construct\n";

         /* Look for config file in home */
         string home(getenv("HOME"));
         if ( FS::IsFile(home + "/" + QD_CONFIG_FILE_HOME) ){
            config.SetName(home + "/" + QD_CONFIG_FILE_HOME);
            config.Parse(*_ref);
         }
         if ( FS::IsFile(QD_CONFIG_FILE_LOCAL) ){ /* Look for config in current directory */
            config.SetName(QD_CONFIG_FILE_LOCAL);
            config.Parse(*_ref);
         }

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


   /* Init Singleton */
   ParamContainer* GlobalParams::_ref = 0;
}

