#include "config.h"

#ifdef USE_DYNMODS
 #include <dlfcn.h>
#endif 

#include "ModuleLoader.h"
#include "qdlib/InternalMods.h"

namespace QDLIB {

   /* Static member initalisation */
   ModuleLoader*  ModuleLoader::_ref=0;
   
   ModuleLoader::ModuleLoader() : _user_path("~/qdmods/")
   {
   }
   
   /**
    * Get an instance of the singleton.
    */
   ModuleLoader * QDLIB::ModuleLoader::Instance( )
   {
      if (_ref == NULL){
	 _ref = new ModuleLoader();
      }
      
      return _ref;
   }
   
   /** 
    * Although it's a singleton it should be destructable.
    * 
    * Module unloading is done here.
    */

   ModuleLoader::~ModuleLoader()
   {
#ifdef USE_DYNMODS
      map<string, module>::iterator it;
      
      /* Unload the modules */
      it = _mod_map.begin();
      while (it != _mod_map.end() && it->second.handle != NULL){
	 dlclose( it->second.handle );
	 it++;
      }
#endif
   }
   
   /**
    * Check if a modules is already loaded.
    */
   bool ModuleLoader::_isLoaded( const string &name )
   {
     if ( _mod_map.find(name) != _mod_map.end() ) return true;
     else return false;
   }

   /**
    * Load an compiled in WF module.
    * 
    * \return true if compiled in module was found
    */
   bool ModuleLoader::_InternalWF(const string &name)
   {
      string s;
      
      if (name.substr(0,2)  == "WF") /* check for prefix WF & remove it */
         s = INTERNAL_BASENAME_WF + name.substr(2);
      else
         s = INTERNAL_BASENAME_WF + name;
      
      instWF* InstanceWF = InternalWF(s);
      
      if (InstanceWF != NULL){
	 _mod_map[name].InstanceWF = InstanceWF;
	 _mod_map[name].InstanceOP = NULL;
	 _mod_map[name].link_count++;
	 _mod_map[name].handle = NULL;
	 return true;
      } else {
	 return false;
      }
      
   }
   
   /**
    * Register a WF module.
    */
#ifdef USE_DYNMODS
   void ModuleLoader::_RegisterWF(void *handle, const string &name)
   {

       
      instWF* InstanceWF = (instWF*) dlsym(handle, "InstanceWF");

      if (InstanceWF != NULL){
	 _mod_map[name].InstanceWF = InstanceWF;
	 _mod_map[name].InstanceOP = NULL;
	 _mod_map[name].link_count++;
	 _mod_map[name].handle = handle;
      } else {
	 throw ( Exception("Invalid WaveFunction module") );
      }
   }
#endif
   
   /**
    * Load an compiled in WF module.
    * 
    * \return true if compiled in module was found
    */
   bool ModuleLoader::_InternalOP(const string &name)
   {
      string s;
      
      
      s = INTERNAL_BASENAME_OP + name;
      instOP* InstanceOP = InternalOP(s);
      
      if (InstanceOP != NULL){
	 _mod_map[name].InstanceWF = NULL;
	 _mod_map[name].InstanceOP = InstanceOP;
	 _mod_map[name].link_count++;
	 _mod_map[name].handle = NULL;
	 return true;
      } else {
	 return false;
      }
      
   }
   
   /**
    * Register an operator module.
    */
#ifdef USE_DYNMODS
   void ModuleLoader::_RegisterOP(void *handle, const string &name)
   {
    
      
      instOP* InstanceOP = (instOP*) dlsym(handle, "InstanceOP");
      if (InstanceOP != NULL){
	 _mod_map[name].InstanceWF = NULL;
	 _mod_map[name].InstanceOP = InstanceOP;
	 _mod_map[name].link_count++;
	 _mod_map[name].handle = handle;
      } else {
	 throw ( Exception("Invalid Operator module") );
      }
   }
#endif
   
   /**
    * Set the user-defined module search path.
    * 
    * Only one path is allowed. Must be an absolute path (not a relative!)
    */
#ifdef USE_DYNMODS
   void ModuleLoader::UserPath( const string &path )
   {
      _user_path = path;
      if ( path[path.size()-1] != '/' && path.size() != 0 )
	 _user_path += '/';
     
   }
#endif
   
   /**
    * Load a WaveFunction Module.
    * 
    * If the the module is non-existend or provides the wrong interface,
    * an exception will be thrown.
    * 
    * \return A fresh, empty instance of the wf class.
    */
   WaveFunction * ModuleLoader::LoadWF(const string &name )
   {
      string s;
      
#ifdef USE_DYNMODS
      void* handle;
      /* is already loaded? */
      if (_isLoaded(name)) {
	 _mod_map[name].link_count++;
	 return _mod_map[name].InstanceWF();
      }
#endif
      /* try compiled in module */
      if (_InternalWF(name)){
	 return _mod_map[name].InstanceWF();
      }
#ifdef USE_DYNMODS   
      /* try user path */
      s = _user_path + MOD_BASENAME_WF + name;
   
      handle = dlopen(s.c_str(), RTLD_NOW | RTLD_LOCAL);
      if ( handle != NULL )
      {
	 _RegisterWF(handle, s);
	 return _mod_map[name].InstanceWF();
      }
      
      /* try system path */
      s = MOD_BASENAME_WF + name;
      handle = dlopen(s.c_str(), RTLD_NOW | RTLD_LOCAL);
      if ( handle != NULL )
      {
	 _RegisterWF(handle, name);
	 return _mod_map[name].InstanceWF();
      } else {
	 throw ( Exception( dlerror() ) );
      }
#else
      s = "Module not found: " + name;
      throw ( Exception( s.c_str() ) );
#endif

   }
   
   /**
    * Load a Operator Module.
    * 
    * If the the module is non-existend or provides the wrong interface,
    * an exception will be thrown.
    * 
    * \return A fresh, empty instance of the operator class.
    */
   Operator * ModuleLoader::LoadOp(const string & name )
   {
      string s;
#ifdef USE_DYNMODS
      void *handle;

      /* is already loaded? */
      if (_isLoaded(name)) {
	 _mod_map[name].link_count++;
	 return _mod_map[name].InstanceOP();
      }
#endif
      /* try compiled in module */
      if (_InternalOP(name)){
	 return _mod_map[name].InstanceOP();
      }
      
#ifdef USE_DYNMODS
      /* try user path */
      s = _user_path + MOD_BASENAME_OP + name + ".so";
      handle = dlopen(s.c_str(), RTLD_NOW | RTLD_LOCAL);
      if ( handle != NULL )
      {
	 _RegisterOP(handle, name);
	 return _mod_map[name].InstanceOP();
      }
      
      /* try system path */
      s = MOD_BASENAME_OP + name + ".so";
      handle = dlopen(s.c_str(), RTLD_NOW | RTLD_LOCAL);
      if ( handle != NULL )
      {
	 _RegisterOP(handle, name);
	 return _mod_map[name].InstanceOP();
      } else {
	 throw ( Exception( dlerror() ) );
      }
#else
      s = "Module not found: " + name;
      throw ( Exception( s.c_str() ) );
#endif
   }



}





