#include <dlfcn.h>
#include "ModuleLoader.h"


namespace QDLIB {

   ModuleLoader::ModuleLoader() : _user_path("~/qdmods/")
   {
   }
   
   
   ModuleLoader::~ModuleLoader()
   {
      map<string, module>::iterator it;
      
      /* Unload the modules */
      it = _mod_map.begin();
      while (it != _mod_map.end() ){
	 dlclose( it->second.handle );
	 it++;
      }
   }
   
   bool QDLIB::ModuleLoader::_isLoaded( )
   {
      return false;
   }

   
   /**
    * Register a WF module.
    */
   void ModuleLoader::_RegisterWF(void *handle, string &name)
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
   
   /**
    * Register an operator module.
    */
   void ModuleLoader::_RegisterOP(void *handle, string &name)
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
   
   /**
    * Set the user-defined module search path.
    * 
    * Only one path is allowed.
    */
   void ModuleLoader::UserPath( string &path )
   {
      _user_path = path;
      if ( path[path.size()-1] != '/')
	 _user_path += '/';
     
   }

   
   /**
    * Load a WaveFunction Module.
    * 
    * If the the module is non-existend or provides the wrong interface,
    * an exception will be thrown.
    * 
    * \return A fresh, empty instance of the wf class.
    */
   WaveFunction * ModuleLoader::LoadWF( string &name )
   {
      void* handle;
      string s;
      
      
      /* is already loaded? */
      if (_isLoaded()) {
	 _mod_map[name].link_count++;
	 return _mod_map[name].InstanceWF();
      }
	 
      /* try user path */
      s = _user_path + name;
      handle = dlopen(s.c_str(), RTLD_NOW );
      if ( handle != NULL )
      {
	 _RegisterWF(handle, name);
	 return _mod_map[name].InstanceWF();
      }
      
      /* try system path */
      handle = dlopen(name.c_str(), RTLD_NOW );
      if ( handle != NULL )
      {
	 _RegisterWF(handle, name);
	 return _mod_map[name].InstanceWF();
      } else {
	 throw ( Exception("No module found") );
      }
      
   }
   
   /**
    * Load a Operator Module.
    * 
    * If the the module is non-existend or provides the wrong interface,
    * an exception will be thrown.
    * 
    * \return A fresh, empty instance of the operator class.
    */
   Operator * ModuleLoader::LoadOp( string & name )
   {
      string s;
      void *handle;
      
      /* is already loaded? */
      if (_isLoaded()) {
	 _mod_map[name].link_count++;
	 return _mod_map[name].InstanceOP();
      }
	 
      /* try user path */
      s = _user_path + name;
      handle = dlopen(s.c_str(), RTLD_NOW );
      if ( handle != NULL )
      {
	 _RegisterOP(handle, name);
	 return _mod_map[name].InstanceOP();
      }
      
      /* try system path */
      handle = dlopen(name.c_str(), RTLD_NOW );
      if ( handle != NULL )
      {
	 _RegisterOP(handle, name);
	 return _mod_map[name].InstanceOP();
      } else {
	 throw ( Exception("No module found") );
      }
   }



}



