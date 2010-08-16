#ifndef GLOBALOPLIST_H
#define GLOBALOPLIST_H

#include <map>

#include "qdlib/Operator.h"

#define OP_LIST_ANONYMOUS_PREFIX "__"

namespace QDLIB {
   
   /**
    * Global operator container.
    */
   class GlobalOpList
   {
     private:
        struct _OpEntry {
           Operator* Op;
           bool initialized;
        };
        
         static GlobalOpList *_ref;
	 int OpListAnonKey; /* Counter for anonymous key names */ 

         map<string,_OpEntry> _OpStore;
         
	 GlobalOpList() : OpListAnonKey(0) {};
         GlobalOpList(const GlobalOpList &p){};
      public:
         ~GlobalOpList();
         static GlobalOpList& Instance();
         static void Destroy();
         
         void Add(string &key, Operator* op);
         
	 void Init(Operator* Op, WaveFunction* Psi);
         
         void Init(const string &key, WaveFunction* Psi);
         
         Operator* operator[]( const string &key );
         
         void PrintList();
         
   };

} /* namespace QDLIB */
#endif // GLOBALOPLIST_H
