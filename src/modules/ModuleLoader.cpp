#include "config.h"


#include "ModuleLoader.h"
#include "InternalMods.h"

namespace QDLIB {


   template<>
   ModuleLoader<WaveFunction>::ModuleLoader() : _user_path("~/qdmods/"),
            _basename_lib(MOD_BASENAME_WF), _basename_int(INTERNAL_BASENAME_WF)
   {}

   template<>
   ModuleLoader<Operator>::ModuleLoader() : _user_path("~/qdmods/"),
            _basename_lib(MOD_BASENAME_OP), _basename_int(INTERNAL_BASENAME_OP)
   {}

   
   
   /** 
    * Although it's a singleton it should be destructable.
    * 
    * Module unloading is done here.
    */

//   ModuleLoader::~ModuleLoader()
//   {
//#ifdef USE_DYNMODS
//      map<string, module>::iterator it;
//
//      /* Unload the modules */
//      it = _mod_map.begin();
//      while (it != _mod_map.end() && it->second.handle != NULL){
//	 dlclose( it->second.handle );
//	 it++;
//      }
//#endif
//   }
   
   /**
    * Load an compiled in WF module.
    * 
    * \return true if compiled in module was found
    */
   template<>
   bool ModuleLoader<WaveFunction>::_Internal(const string &name)
   {
      string s;
      
      if (name.substr(0,2)  == "WF") /* check for prefix WF & remove it */
         s = _basename_int + name.substr(2);
      else
         s = _basename_int + name;
      
      fptr InstanceWF;
      InstanceWF.Cptr = InternalWF(s);
      
      if (InstanceWF.voidptr != NULL){
       _mod_map[name].Instance = InstanceWF;
       _mod_map[name].link_count++;
       _mod_map[name].handle = NULL;
       return true;
      } else {
       return false;
      }
      
   }


   /**
    * Load an compiled in Operator module.
    *
    * \return true if compiled in module was found
    */
   template<>
   bool ModuleLoader<Operator>::_Internal(const string &name)
   {
      string s;

      s = _basename_int + name;

      fptr InstanceOp;
      InstanceOp.Cptr = InternalOP(s);

      if (InstanceOp.voidptr != NULL){
       _mod_map[name].Instance = InstanceOp;
       _mod_map[name].link_count++;
       _mod_map[name].handle = NULL;
       return true;
      } else {
       return false;
      }

   }

}





