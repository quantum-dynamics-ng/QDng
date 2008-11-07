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
	 
	 bool _isLoaded();
	 void _RegisterWF(void *handle, string &name);
	 void _RegisterOP(void *handle, string &name);
	 
	 class mod {
	    public:
	    static WaveFunction* InstanceWF();
	 };
      public:
	 ModuleLoader();
      
	 ~ModuleLoader();
	 
	 
	 void UserPath(string &path);
	 WaveFunction* LoadWF(string &name);
	 Operator* LoadOp(string &name);
      
   };

}

#endif
