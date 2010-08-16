#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <map>

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
         map<T*,int> _RefCount;
         
         Collector(){}
         Collector(const Collector&){}
	 ~Collector();
      public:
         static Collector<T>* Instance();
         void Register(T* obj);
         void Delete(T* obj);
         void Delete();
         
   };

   template<typename T>
   Collector<T>::~Collector()
   {
      cout << "Coll Destruction" << endl;
      this->Delete();
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
    * Register the pointer to an object
    *
    */
   template<typename T>
   void Collector<T>::Register(T* obj)
   {
      if (obj == NULL) return;
      
      typename map<T*, int>::iterator it;
      
      it = _RefCount.find(obj);
      if (it != _RefCount.end())
         (*it).second++;
      else
         _RefCount[obj] = 1;
   }

   /**
    * Delete the pointer to an object.
    *
    */
   template<typename T>
   void Collector<T>::Delete(T* obj)
   {
      if (obj == NULL) return;
      
      typename map<T*,int>::iterator it;
      
      it = _RefCount.find(obj);
      if (it != _RefCount.end()){
         (*it).second--;
         if ((*it).second == 0) {
            delete (*it).first;
            _RefCount.erase(it);
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
      cout << "Coll Del" << endl;
      typename map<T*,int>::iterator it;

      for ( it = _RefCount.begin(); it != _RefCount.end(); it++ ){
	 delete (*it).first;
         _RefCount.erase(it);
      }
      _RefCount.clear();
   }
   
}

#endif // COLLECTOR_H
