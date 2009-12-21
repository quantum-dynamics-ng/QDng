#ifndef COLLECTOR_T_H_
#define COLLECTOR_T_H_

#include "mex/ObjectHandle.h"
#include <mex.h>
#include <typeinfo>
#include <map>


#include <string>
#include <math.h>



#include "qdlib/WaveFunction.h"


// --------------------------------------------------------- 
// ------------------ Garbage Collection -------------------
// --------------------------------------------------------- 

// Garbage collection singleton (one collector object for each type T).
// Ensures that registered handles are deleted when the dll is released (they
// may also be deleted previously without problem).
//    The Collector provides protection against resource leaks in the case
// where 'clear all' is called in MatLab. (This is because MatLab will call
// the destructors of statically allocated objects but not free-store allocated
// objects.)
template <typename T>
class Collector {
	std::map<double,ObjectHandle<T>*> objlist;
public:
	~Collector();
	static Collector<T>* Instance () ;
	void register_handle (ObjectHandle<T>* obj) ;
	void delete_handle (const mxArray* val) ;
	double find_handle (const mxArray* val);

private: // prevent construction
	Collector();
//	static Collector<T> *_ref;
	Collector(const Collector&);
};

template <typename T>
Collector<T>::~Collector() {
		typename std::map<double,ObjectHandle<T>*>::iterator it;
		typename std::map<double,ObjectHandle<T>*>::iterator end= objlist.end();
		for (it= objlist.begin(); it!=end; ++it) {
			//if ((*it)->signature == *it) // check for valid signature
				//std::cout << it->first << std::endl;
				delete it->second;
				std::cout << "Delete Collector" << std::endl;
		}
}

template <typename T>
Collector<T>* Collector<T>::Instance () {
		


 			static Collector singleton;
			std::cout << "Coll: " << &singleton << std::endl;
 			return &singleton;
// 			std::cout << "New Collector: " << typeid(T).name() << std::endl;

}

template <typename T>
void Collector<T>::register_handle (ObjectHandle<T>* obj) {
		mxArray* val = obj->to_mex_handle();
		objlist[*mxGetPr(val)] = obj;
}

template <typename T>
void Collector<T>::delete_handle (const mxArray* val) {
		//std::cout << "delete_handle" << std::endl;
		typename std::map<double,ObjectHandle<T>*>::iterator it = objlist.find(*mxGetPr(val));
		objlist.erase (it); 
}

template <typename T>
double Collector<T>::find_handle (const mxArray* val) {
		double obj = objlist.find(*mxGetPr(val))->first;
		return obj;
}

template <typename T>
Collector<T>::Collector(){}


#endif