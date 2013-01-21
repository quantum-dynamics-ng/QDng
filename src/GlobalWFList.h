/*
 * GlobalWFList.h
 *
 *  Created on: Jan 17, 2013
 *      Author: markus
 */

#ifndef GLOBALWFLIST_H_
#define GLOBALWFLIST_H_

#include "qdlib/WaveFunction.h"

#define WF_LIST_ANONYMOUS_PREFIX "__"

namespace QDLIB
{

   /**
    * Global Wavefunction container.
    */
   class GlobalWFList
   {
      private:
          struct _WFEntry {
             WaveFunction* wf;
             bool persist;
             _WFEntry() : wf(NULL), persist(false) {}
          };

          int _WFListAnonKey; /* Counter for anonymous key names */

          map<string,_WFEntry> _WFStore;

          GlobalWFList() : _WFListAnonKey(0) {};
          GlobalWFList(const GlobalWFList &p){};
       public:
          ~GlobalWFList();
          static GlobalWFList& Instance();
          static void Destroy();

          void Clear();

          void Add(const string &key, WaveFunction* wf, bool persist = false);

          WaveFunction* Get( const string &key );

          WaveFunction* GetCopy( const string &key );

          WaveFunction* operator[]( const string &key ) { return Get(key); }

          void Set(const string &key, WaveFunction* wf);

          void PrintList();

   };

} /* namespace QDLIB */
#endif /* GLOBALWFLIST_H_ */
