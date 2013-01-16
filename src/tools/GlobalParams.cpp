#include "GlobalParams.h"
#include "fstools.h"
#include <stdlib.h>

namespace QDLIB {

   ParamContainer& GlobalParams::Instance()
   {
      if (_ref == NULL)
      {
         _ref = new ParamContainer();

         /* Look for config file in home */
         string home(getenv("HOME"));
         if ( FS::IsFile(home + "/" + QD_CONFIG_FILE_HOME) ){
            _ref->ReadFromFile(home + "/" + QD_CONFIG_FILE_HOME);
         }
         if ( FS::IsFile(QD_CONFIG_FILE_LOCAL) ){ /* Look for config in current directory */
            _ref->ReadFromFile(QD_CONFIG_FILE_LOCAL);
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

