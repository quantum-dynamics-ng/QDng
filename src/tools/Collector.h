// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>
#include <cstdlib>

using namespace std;

namespace QDLIB {

   /**
    * Garabage Collector class.
    *
    * This is a destructable Singleton.
    * The Class keeps track of the Register/Delete actions.
    * An Object is really deleted if there are no more refcounts on it.
    */
   template<typename T>
   class Collector
   {
      private:
	 /* Structure holding the pointer and reference counters */
	 class reflist {
	    public:
	       T *p;
	       int refcount;
	       bool aligned; /* inidicate thtat memory was allocated with posix_memalign */
	       reflist(T* pointer) : p(pointer), refcount(1), aligned(false) {}
	       reflist(T* pointer, bool pmemalign) : p(pointer), refcount(1), aligned(pmemalign) {}
	 };
	 
         vector<reflist> _RefCount;
         
	 bool _BigClean;
	       
         int find(T* p);
         Collector() : _BigClean(false) {}
         Collector(const Collector&){}
      public:
	 ~Collector();
         static Collector<T>* Instance();
         void Register(T* obj, bool aligned = false);
	 bool IsAllocated(T* obj);
	 size_t Elements();
	 void ShowElements();
         void Delete(T* obj);
         void Delete();
         
   };

   template<typename T>
   Collector<T>::~Collector()
   {
      Delete();
   }

   
   /**
    * Get Reference to an instance of the Collector.
    */
   template<typename T>
   Collector<T>* Collector<T>::Instance()
   {
      static Collector<T> ref;
      return &ref;
   }
  

   /**
    * Find a pointer in the list
    */
   template<typename T>
   int Collector<T>::find(T* p)
   {
      for (unsigned int i=0; i < _RefCount.size(); i++) {
	 if (p == _RefCount[i].p) return i;
      }
      return -1;
   }



   /**
    * Register the pointer to an object
    *
    */
   template<typename T>
   void Collector<T>::Register(T* obj, bool aligned)
   {
      if (obj == NULL) return;
      int ind;

      ind = find(obj);
      if (ind > -1){
         _RefCount[ind].refcount++;
      }else {
	 _RefCount.push_back(reflist(obj, aligned));
      }
   }

   /**
    * Check wether the object to a pointer is still allocated
    */
   template<typename T>
   bool Collector<T>::IsAllocated(T* obj)
   {
      if (find(obj) > -1) return true;
      else return false;
   }

   /**
    * Ask for the number collected objects in the Collector
    */
   template<typename T>
   size_t Collector<T>::Elements()
   {
      return _RefCount.size();

   }

   /**
    * Print overview over the collectors content
    *
    */
   template<typename T>
   void Collector<T>::ShowElements()
   {
   
      /* Loop over whole list */
      int size=_RefCount.size();
      for (int i=0; i < size; i++) {
	    cout << _RefCount[i].p; /* retrive pointer & remove from list */
	    cout << "\t" << _RefCount[i].refcount;
	    if(  _RefCount[i].aligned)
	       cout << "\taligned";
	    cout << endl;
	 
      }
   }
   /**
    * Delete the pointer to an object.
    *
    */
   template<typename T>
   void Collector<T>::Delete(T* obj)
   {
      if (obj == NULL || _BigClean) return;
      
      int ind = find(obj);
      if (ind > -1){
	 _RefCount[ind].refcount--;
	 if (_RefCount[ind].refcount == 0){
	    bool aligned = _RefCount[ind].aligned;
	    _RefCount.erase (_RefCount.begin()+ind);
	    if (aligned)
	       free(obj);
	    else
	       delete obj;
	 }
      }
   }

   /**
    * Delete all pointers in the collector, no matter if there are open references.
    *
    */
   template<typename T>
   void Collector<T>::Delete()
   {   
      _BigClean = true;
      /* Loop over whole list */
      int size=_RefCount.size();
      for (int i=0; i < size; i++) {
	 if (_RefCount[i].refcount > 0){
	    T* p;
	    bool aligned;
	    p = _RefCount[i].p; /* retrive pointer & remove from list */
	    aligned = _RefCount[i].aligned;
	    
	    /* delete object after list is clear => avoid problems if the destructor calls Delete(T*) */
	    if (aligned)
	       free(p);
	    else
	       delete p;
            
            _RefCount[i].refcount = 0;
	 } 
	 
      }
      _RefCount.clear();
      _BigClean = false;
   }
   
}

#endif // COLLECTOR_H

