#include "GlobalOpList.h"
#include "tools/Logger.h"
#include "tools/Collector.h"


namespace QDLIB {
   
   GlobalOpList& GlobalOpList::Instance()
   {
      if (_ref == NULL){
         _ref = new GlobalOpList();
      }
      return *_ref;
   }


   GlobalOpList::~GlobalOpList()
   {      
      _ref = NULL;
   }

   void GlobalOpList::Destroy()
   {
      if (_ref != NULL){
         delete _ref;
         _ref = NULL;
      }
   }

   /**
    * Add a new operator to the global list.
    *
    * An Exception is thrown if an operator with the same identifier already exists.
    *
    * \param key String identifier. May be empty, in this case an internal name is generated
    * \param op  A parameter initialized operator
    * \param maintain Register also for destruction
    */
   void GlobalOpList::Add(string &key, Operator* Op, bool persist)
   {
      
      /* Check if key exists */
      if (_OpStore.find(key) != _OpStore.end())
         throw (EIncompatible("Key already exists in operator list: ", key));
      
      if (Op == NULL)
         throw (EIncompatible("Operator not initalized"));
      
       if (strlen(key.c_str()) < 1) { /* If no key is given, we produce an internal key with a serial number */
          stringstream ss;
          ss << OP_LIST_ANONYMOUS_PREFIX << Op->Name() << "#" << OpListAnonKey;
          key = ss.rdbuf()->str();
          OpListAnonKey++;
       }
      
      _OpStore[key].Op = Op;
      _OpStore[key].persist = persist;
   }

   /**
    * Initialize all registered operator with a wave function.
    *
    */
   void GlobalOpList::Init(Operator* Op, WaveFunction* Psi)
   {
      map<string, _OpEntry>::iterator it;
      bool found = false;

      for (it = _OpStore.begin(); it != _OpStore.end(); it++) {
         if (((*it).second).Op == Op && !((*it).second).initialized) { /* Operator exists but not initialized */
            ((*it).second).Op->Init(Psi);
            ((*it).second).initialized = true;
            found = true;
         } else if (((*it).second).Op == Op && ((*it).second).initialized) { /* Operator exists but already initialized */
            ((*it).second).Op->Valid(Psi); /* Check if Initialzation is valid */
            found = true;
         }
      }

      /* If the operator was not found, we register it and do the initialization */
      if (!found) {
         string key;
         Add(key, Op);
         _OpStore[key].Op->Init(Psi);
         _OpStore[key].initialized = true;
      }
   }
   
   /**
    * Initialize a registered operator with a wave function.
    *
    * Every registerd operator must be initialized through this method to
    * avoid ambigious double initalizations.
    */
   void GlobalOpList::Init(const string &key, WaveFunction* Psi)
   {
      /* Check if key exists */
      if (_OpStore.find(key) == _OpStore.end())
         throw (EIncompatible("Key doesn't exists in operator list: ", key));

      if ( ! _OpStore[key].initialized ){
         _OpStore[key].Op->Init(Psi);
         _OpStore[key].initialized = true;
      } else {
         _OpStore[key].Op->Valid(Psi); /* Check if initialization is compatible */
      }
   }
   
   /**
    * Retrive an operator from the list.
    * 
    * An Exception is thrown if the key doesn't exist.
    */
   Operator* GlobalOpList::operator[]( const string &key )
   {
      /* Check if key exists */
      if (_OpStore.find(key) == _OpStore.end())
         throw (EIncompatible("Key doesn't exists in operator list: ", key));
      
      return _OpStore[key].Op;
   }

   void GlobalOpList::PrintList()
   {
      Logger& log = Logger::InstanceRef();
      map<string, _OpEntry>::iterator it;

      for (it = _OpStore.begin(); it != _OpStore.end(); it++) {
         log.cout() << (*it).first;
         log.flush();
         log.cout() << "\t\t\t" << ((*it).second).Op->Name();
         log.cout() << "\t\t";
         if (((*it).second).initialized)
            log.cout() << "initialized\t";
         log.cout() << endl;
      }

   }

   /**
    * Clear the Operators in the storage, but leave persistent Ops untouched.
    */
   void QDLIB::GlobalOpList::Clear()
   {
      map<string, _OpEntry>::iterator it;

      for (it = _OpStore.begin(); it != _OpStore.end(); it++) {
         if ( ! ((*it).second).persist ) {
            DELETE_OP(((*it).second).Op);
            _OpStore.erase(it);
            it = _OpStore.begin(); /* iterator is now invalid => start over */
         }
      }
   }
   
   /* Singleton Initializer */
   GlobalOpList* GlobalOpList::_ref = 0;
}


