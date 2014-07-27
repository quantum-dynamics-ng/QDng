#ifndef QDLIBMODULELOADER_H
#define QDLIBMODULELOADER_H

#ifdef USE_DYNMODS
 #include <dlfcn.h>
#endif

#include "tools/Logger.h"

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"

#include "config.h"

#define INTERNAL_BASENAME_WF "INT_WF"
#define INTERNAL_BASENAME_OP "INT_O"

#define MOD_BASENAME_WF "libqd-WF"
#define MOD_BASENAME_OP "libqd-O"

namespace QDLIB {

   /**
    * Loader for dynamic linked objects and compiled in objects.
    * 
    * This is a singleton class;
    * 
    * With the first try is load an internal module. This has precedence over
    * external modules. Thus external modules should use different names
    * than internal. In the next step the Loader looks for the module
    * in either in the path specified by UserPath or otherwise in a system
    * library directory. Or you can specify it by the environment variable LD_LIBRARY_PATH.
    * 
    * @author Markus Kowalewski
    */
   template <class C>
   class ModuleLoader
   {
      private:
         typedef C* (func)();
	 
         /** Union to avoid iso c++ casting problems */
         typedef union {
            void*   voidptr;
            func* Cptr;
         } fptr;
         
         struct module {
            void *handle;
            int link_count;
            fptr Instance;
            module() : handle(NULL), link_count(0)
            {
               Instance.voidptr = NULL;
            }
         };
	 
	 string _user_path;
	 string _basename_lib;
	 string _basename_int;
	 map<string, module> _mod_map;
	 
	 bool _isLoaded( const string &name );
	 bool try_lib_path(const string& full_name, const string& name);
	 
#ifdef USE_DYNMODS
	 void _Register(void *handle, const string &name);
#endif
	 bool _Internal(const string &name);
	 
	 ModuleLoader();
	 ModuleLoader(ModuleLoader &m){}
	 
      public:

	 //~ModuleLoader();
	 
	 static ModuleLoader* Instance();
	 
#ifdef USE_DYNMODS
	 void UserPath(const string &path);
#endif
	 C* Load(const string &name);
   };



   /**
    * Get an instance of the singleton.
    */
   template <class C>
   ModuleLoader<C>* ModuleLoader<C>::Instance( )
   {
      static ModuleLoader ref;
      return &ref;
   }

   /**
    * Check if a modules is already loaded.
    */
   template <class C>
   bool ModuleLoader<C>::_isLoaded( const string &name )
   {
     if ( _mod_map.find(name) != _mod_map.end() ) return true;
     else return false;
   }

   /**
    * Register a module.
    */
#ifdef USE_DYNMODS
   template <class C>
   void ModuleLoader<C>::_Register(void *handle, const string &name)
   {
      fptr Instance;
      dlerror();
      Instance.voidptr = dlsym(handle, "mod_instance_func");
      const char *err = dlerror();
      if (err == NULL){
         _mod_map[name].Instance = Instance;
         _mod_map[name].link_count++;
         _mod_map[name].handle = handle;
      } else {
         throw ( EIncompatible ("Invalid module. mod_instance_func function not found", name) );
      }
   }
#endif


   /**
    * Set the user-defined module search path.
    *
    * Only one path is allowed. Must be an absolute path (not a relative!)
    */
#ifdef USE_DYNMODS
   template <class C>
   void ModuleLoader<C>::UserPath( const string &path )
   {
      _user_path = path;
      if ( path[path.size()-1] != '/' && path.size() != 0 )
       _user_path += '/';

   }
#endif

   /**
    * Try to load a module by a specfic lib name.
    *
    * full_name is any name that dlopen accepts.
    */
#ifdef USE_DYNMODS
   template<class C>
   bool ModuleLoader<C>::try_lib_path(const string& full_name, const string& name)
   {
      Logger& log = Logger::InstanceRef();

      void* handle = dlopen(full_name.c_str(), RTLD_NOW | RTLD_LOCAL);

      if ( handle != NULL )
      {
         _Register(handle, name);
         log.coutdbg() << "load module " << name << " from " << full_name << endl;
         log.flush();
         return true;
      } else {
         log.coutdbg() << "load module " << name << " failed: " << dlerror() << endl;
      }

      return false;
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
   template<class C>
   C* ModuleLoader<C>::Load(const string &name )
   {
      string s;

#ifdef USE_DYNMODS
      /* is already loaded? */
      if (_isLoaded(name)) {
       _mod_map[name].link_count++;
       return _mod_map[name].Instance.Cptr();
      }
#endif
      /* try compiled in module */
      if (_Internal(name)){
         return _mod_map[name].Instance.Cptr();
      }
#ifdef USE_DYNMODS
      /* try user path */
      if (_user_path.length() > 0){
         s = _user_path + _basename_lib + name + ".so";

         if (try_lib_path(s, name))
            return _mod_map[name].Instance.Cptr();
      }

      /* try path form env */
      const char *mod_dir = getenv("QD_MOD_PATH");
      if (mod_dir != NULL){
         s = mod_dir + _basename_lib + name + ".so";

         if (try_lib_path(s, name))
            return _mod_map[name].Instance.Cptr();
      }

      /* try the compiled in path */
      s = string(QD_SYS_MOD_PATH) + _basename_lib + name + ".so";
      if (try_lib_path(s, name))
         return _mod_map[name].Instance.Cptr();

      /* try system path */
      s = _basename_lib + name;

      if (try_lib_path(s, name))
         return _mod_map[name].Instance.Cptr();


#endif
      s = "Module not found: " + name;
      throw ( Exception( s.c_str() ) );

      return NULL;
   }
}

#endif
