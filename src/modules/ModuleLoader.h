#ifndef QDLIBMODULELOADER_H
#define QDLIBMODULELOADER_H

#include "qdlib/WaveFunction.h"
#include "qdlib/Operator.h"

#define MOD_BASENAME_WF "QD-WF-"
#define MOD_BASENAME_OP "QD-OP-"

namespace QDLIB {

   /**
    * Loader for dynamic linked objects.
    * 
    * This is a singleton class;
    * 
    * The Loader looks for the module in either in the path specified by UserPath
    * or otherwise in a system library directory. Or you can specify it by the
    * environment variable LD_LIBRARY_PATH.
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
	 void _RegisterWF(void *handle, const string &name);
	 void _RegisterOP(void *handle, const string &name);
	 
	 static ModuleLoader *_ref;

	 ModuleLoader();
	 ModuleLoader(ModuleLoader &m){}
	 ~ModuleLoader();
      public:

	 static ModuleLoader* Instance();
	 
	 void UserPath(const string &path);
	 WaveFunction* LoadWF(const string &name);
	 Operator* LoadOp(const string &name);
      
   };

}

#endif
