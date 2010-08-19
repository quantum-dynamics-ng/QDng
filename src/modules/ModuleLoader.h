#ifndef QDLIBMODULELOADER_H
#define QDLIBMODULELOADER_H

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"

#include "config.h"

#define INTERNAL_BASENAME_WF "INT_WF"
#define INTERNAL_BASENAME_OP "INT_O"

#define MOD_BASENAME_WF "qdWF"
#define MOD_BASENAME_OP "qdO"

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
   class ModuleLoader
   {
      private:
	 typedef WaveFunction* (instWF)();
	 typedef Operator* (instOP)();
	 
	 struct module {
	    void *handle;
	    int link_count;
	    instWF* InstanceWF;
	    instOP* InstanceOP;
	 };
	 
	 string _user_path;
	 map<string, module> _mod_map;
	 
	 bool _isLoaded( const string &name );
	 
#ifdef USE_DYNMODS
	 void _RegisterWF(void *handle, const string &name);
	 void _RegisterOP(void *handle, const string &name);
#endif
	 bool _InternalWF(const string &name);
	 bool _InternalOP(const string &name);
	 
	 static ModuleLoader *_ref;

	 ModuleLoader();
	 ModuleLoader(ModuleLoader &m){}
	 
      public:

	 ~ModuleLoader();
	 
	 static ModuleLoader* Instance();
	 
#ifdef USE_DYNMODS
	 void UserPath(const string &path);
#endif
	 WaveFunction* LoadWF(const string &name);
	 Operator* LoadOp(const string &name);
      
   };

}

#endif
