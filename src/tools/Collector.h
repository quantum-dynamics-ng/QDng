#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>

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
	       reflist(T* pointer) : p(pointer), refcount(1){}
	 };
	 
         vector<reflist> _RefCount;

         int find(T* p);
         Collector(){}
         Collector(const Collector&){}
      public:
	 ~Collector();
         static Collector<T>* Instance();
         void Register(T* obj);
	 bool IsAllocated(T* obj);
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
   void Collector<T>::Register(T* obj)
   {
      if (obj == NULL) return;
      int ind;

      ind = find(obj);
      if (ind != -1)
         _RefCount[ind].refcount++;
      else
	 _RefCount.push_back(reflist(obj));
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
    * Delete the pointer to an object.
    *
    */
   template<typename T>
   void Collector<T>::Delete(T* obj)
   {
      if (obj == NULL) return;
      
      int ind;
      
      ind = find(obj);
      if (ind > -1){
	 _RefCount[ind].refcount--;
	 if (_RefCount[ind].refcount == 0){
	    delete _RefCount[ind].p;
	    _RefCount.erase (_RefCount.begin()+ind);
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
      for (unsigned int i= 0; i < _RefCount.size(); i++){
	 delete _RefCount[i].p;
      }
      _RefCount.clear();
   }
   
}

#endif // COLLECTOR_H

