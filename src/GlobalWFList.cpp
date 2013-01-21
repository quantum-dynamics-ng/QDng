/*
 * GlobalWFList.cpp
 *
 *  Created on: Jan 17, 2013
 *      Author: markus
 */

#include "GlobalWFList.h"
#include "tools/Logger.h"

#include <sstream>

namespace QDLIB
{

   GlobalWFList::~GlobalWFList()
   {
      Clear();
   }


   GlobalWFList& GlobalWFList::Instance()
   {
      static GlobalWFList ref;
      return ref;
   }

   /**
    * Add a new wave function to the global list.
    *
    * An Exception is thrown if wave function with the same identifier already exists.
    *
    * \param key String identifier. May be empty, in this case an internal name is generated
    * \param op  A parameter initialized operator
    * \param persist Register also for destruction
    */
   void GlobalWFList::Add(const string &key, WaveFunction* wf, bool persist)
   {
      string nkey = key;

      /* Check if key exists */
      if (_WFStore.find(key) != _WFStore.end())
         throw (EIncompatible("Key already exists in wave function list: ", key));

      if (wf == NULL)
         throw (EIncompatible("GlobalWFList: Wave function is NULL!"));

       if (strlen(key.c_str()) < 1) { /* If no key is given, we produce an internal key with a serial number */
          stringstream ss;
          ss << WF_LIST_ANONYMOUS_PREFIX << wf->Name() << "#" << _WFListAnonKey;
          nkey = ss.rdbuf()->str();
          _WFListAnonKey++;
       }

       Collector<WaveFunction>::Instance()->Register(wf);
      _WFStore[nkey].wf = wf;
      _WFStore[nkey].persist = persist;
   }

   /**
    * Get pointer to a named WF.
    */
   WaveFunction* GlobalWFList::Get( const string &key )
   {
      /* Check if key exists */
      if (_WFStore.find(key) == _WFStore.end())
         throw (EIncompatible("Key doesn't exists in wave function list: ", key));

      return _WFStore[key].wf;
   }

   /**
    * Get a copy of named WF.
    */
   WaveFunction* GlobalWFList::GetCopy( const string &key )
   {
      WaveFunction* org = Get(key);
      WaveFunction* neu = org->NewInstance();

      *neu = org;

      return neu;
   }

   /**
    * Set WF the with name.
    *
    * If the key doesn't exist the entry is created.
    */
   void GlobalWFList::Set(const string &key, WaveFunction* wf)
   {
      if (_WFStore.find(key) == _WFStore.end())
         Add(key, wf, true);
      else {
         Collector<WaveFunction>::Instance()->Delete(_WFStore[key].wf);
         Collector<WaveFunction>::Instance()->Register(wf);
         _WFStore[key].wf = wf;
      }
   }

   /**
    * Clear the Operators in the storage, but leave persistent Ops untouched.
    */
   void GlobalWFList::Clear()
   {
      map<string, _WFEntry>::iterator it;

      it = _WFStore.begin();
      while ( it != _WFStore.end() ) {
         if ( ! ((*it).second).persist ) {
            DELETE_WF(((*it).second).wf);
            _WFStore.erase(it);
            it = _WFStore.begin(); /* iterator is now invalid => start over */
         } else it++;
      }
   }


   void GlobalWFList::PrintList()
   {
      Logger& log = Logger::InstanceRef();
      map<string, _WFEntry>::iterator it;

      for (it = _WFStore.begin(); it != _WFStore.end(); it++) {
         log.cout() << (*it).first;
         log.flush();
         log.cout() << "\t\t\t" << ((*it).second).wf->Name();
         log.cout() << "\t\t";

         if (((*it).second).persist)
            log.cout() << "persistent\t";

         log.cout() << endl;
      }

   }
} /* namespace QDLIB */
