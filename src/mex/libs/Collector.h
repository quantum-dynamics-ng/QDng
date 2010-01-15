#ifndef COLLECTOR_T_H_
#define COLLECTOR_T_H_

#include "mex/libs/ObjectHandle.h"
#include <mex.h>
#include <typeinfo>
#include <map>


#include <string>
#include <math.h>



#include "qdlib/WaveFunction.h"

/**
* template singleton class to collect and manage C++ objects in matlab
*
* --------------------------------------------------------- 
* ------------------ Garbage Collection -------------------
* --------------------------------------------------------- 

* Garbage collection singleton (one collector object for each type T).
* Ensures that registered handles are deleted when the dll is released (they
* may also be deleted previously without problem).
*    The Collector provides protection against resource leaks in the case
* where 'clear all' is called in MatLab. (This is because MatLab will call
* the destructors of statically allocated objects but not free-store allocated
* objects.)
*/
template <typename T>
class Collector {
	std::map<double,ObjectHandle<T>*> objlist;
public:
	~Collector();
	static Collector<T>* Instance () ;
	void register_handle (ObjectHandle<T>* obj) ;
	void delete_handle (const mxArray* val) ;
	double find_handle (const mxArray* val);
	ObjectHandle<T>* find_ptr(ObjectHandle<T>* obj);

private: // prevent construction
	Collector();
//	static Collector<T> *_ref;
	Collector(const Collector&);
};
/**
    * destuctor: deletes all objects created in Matlab (clear all)
    */
template <typename T>
Collector<T>::~Collector() {
		typename std::map<double,ObjectHandle<T>*>::iterator it;
		typename std::map<double,ObjectHandle<T>*>::iterator end= objlist.end();
		for (it= objlist.begin(); it!=end; ++it) {
			//if ((*it)->signature == *it) // check for valid signature
				//std::cout << it->first << std::endl;
				delete it->second;
				//std::cout << "Delete Collector" << std::endl;
		}
}
/**
    * Instance collector (singleton)
    */
template <typename T>
Collector<T>* Collector<T>::Instance () {
 			static Collector singleton;
			//std::cout << "Collector::Instance Coll: " << &singleton << std::endl;
 			return &singleton;
// 			std::cout << "New Collector: " << typeid(T).name() << std::endl;

}

/**
    * register an ObjectHandel and checks whether ist is already existent
    */
template <typename T>
void Collector<T>::register_handle (ObjectHandle<T>* obj) {
		bool reg;
		reg= true;
		mxArray* val = obj->to_mex_handle();
		typename std::map<double,ObjectHandle<T>*>::iterator it;
		typename std::map<double,ObjectHandle<T>*>::iterator end= objlist.end();
		for (it= objlist.begin(); it!=end; ++it) {
		  ObjectHandle<T>* obj_test = it->second;
		  if ( obj_test->t == obj->t) {reg = false;} 
		}
		if (reg == true ) {
		  double map_val = *mxGetPr(val);
		  objlist[*mxGetPr(val)] = obj;
		}
}
/**
    * Checks whether on ObjectHandel with the same pointer is already existent
    */
template <typename T>
ObjectHandle<T>* Collector<T>::find_ptr (ObjectHandle<T>* obj) {
		bool exists = false;
		typename std::map<double,ObjectHandle<T>*>::iterator it;
		typename std::map<double,ObjectHandle<T>*>::iterator end= objlist.end();
		ObjectHandle<T>* obj_test;
		ObjectHandle<T>* obj_return = obj;
		for (it= objlist.begin(); it!=end; ++it) {
		  obj_test = it->second;
		  if ( obj_test->t == obj->t) obj_return = obj_test; 
		}
		return obj_return;
}
/**
    * Deletes and ObjectHandle and the pointer 
    */
template <typename T>
void Collector<T>::delete_handle (const mxArray* val) {
		//std::cout << "delete_handle" << std::endl;
		typename std::map<double,ObjectHandle<T>*>::iterator it = objlist.find(*mxGetPr(val));
		objlist.erase (it); 
}
/**
    * searches the map for an existen Handle 
    */
template <typename T>
double Collector<T>::find_handle (const mxArray* val) {
		double obj = objlist.find(*mxGetPr(val))->first;
		return obj;
}

template <typename T>
Collector<T>::Collector(){}


#endif