#ifndef GLOBALOPLIST_H
#define GLOBALOPLIST_H

#include <map>

#include "qdlib/Operator.h"

#define OP_LIST_ANONYMOUS_PREFIX "__"

namespace QDLIB {
   
   /**
    * Global operator container.
    * 
    * Operators have a named key to access them (must be unique).
    * If you put an Operator in the container without a key, a name will be generated.
    * Additionally you can give operators a label to group them together (the label is not unique).
    */
   class GlobalOpList
   {
     private:
         struct _OpEntry {
            Operator* Op;
            bool initialized;
            bool persist;
            string label;
            _OpEntry() : Op(NULL), initialized(false), persist(false), label() {}
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
         
         void Clear();
         
         void Add(string &key, Operator* op, bool persist = false);
         
         void SetLabel(const string& key, const string& label);
         
         int GetLabels(const string& label, Operator** list, int max);
         
         bool CheckLabel(const Operator* Op, const string& label);
         
         void Init(Operator* Op, WaveFunction* Psi);
         
         void Init(const string &key, WaveFunction* Psi);
         
         Operator* operator[]( const string &key );
         
         void PrintList();
         
   };

} /* namespace QDLIB */
#endif // GLOBALOPLIST_H
