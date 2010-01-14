/** class ObjectHandle: a handle class for mex-functions
 *
 * Notes:
 * 1. If passing an object to the first (pointer) constructor, be sure that
 * the object was allocated on the heap as the ObjectHandle takes ownership
 * of the objects and will eventually delete it. Be sure to not delete any aliases
 * to the passed object.
 * 2. The second (reference) constructor permits static objects to be passed via 
 * ObjectHandles by ensuring the ObjectHandle does not take ownership of
 * the object. 
 */

#ifndef OBJECT_HANDLE_T_H_
#define OBJECT_HANDLE_T_H_

#include <mex.h>
#include <typeinfo>
#include <map>

#include <iostream>
#include <string>
#include <math.h>


template<typename T> class Collector;

template <typename T>
class ObjectHandle {
public:
	/**
	* Constructor for free-store allocated objects.
	* Handle takes ownership, and will delete object when it is destroyed.
	*/
	ObjectHandle(T*& ptr);
	/**
	* Constructor for non-owned objects.
	* Object may be heap or statically allocated; the handle does NOT
	*take ownership, and the client is responsible for deleting it.
	*/
	ObjectHandle(T& obj);

	~ObjectHandle();
	/**
	* Convert ObjectHandle<T> to a mxArray handle (to pass back from mex-function).
	*/
	mxArray* to_mex_handle(); 
	/**
	* Convert mxArray (passed to mex-function) to an ObjectHandle<T>.
	*/
	static ObjectHandle* from_mex_handle( const mxArray* ma );
	/**
	* Get the actual object contained by handle
	*/
	T* get_object() const { return t; }
	void destroy_object(const mxArray *mxh);

private:
	ObjectHandle* signature; // use 'this' as a unique object signature 
	const std::type_info* type; // type checking information
	bool owns; // marks whether handle owns pointed-to object
	T *t; // object pointer

	friend class Collector<T>; // allow Collector access to signature
};
/**
* --------------------------------------------------------- 
* ------------------ Helper functions ---------------------
* --------------------------------------------------------- 
* These functions remove the need to deal with ObjectHandle<T>
* class directly for most common operations.
*
* Create mex handle to object t (where t is heap allocated). 
* Client no longer owns t, and so must not delete it.
*/
template <typename T>
mxArray *create_handle(T* t)

{
	ObjectHandle<T>* handle= new ObjectHandle<T>(t);
	return handle->to_mex_handle();
}

/**
* Obtain object represented by handle.
*/

template <typename T>
T* get_object(const mxArray *mxh)
{
	ObjectHandle<T>* handle= ObjectHandle<T>::from_mex_handle(mxh);
	return handle->get_object();
}

/**
* If deleting object, rather than leaving it to garbage collection,
* must delete it via the handle; do not delete T* directly.
*/
template <typename T>
void ObjectHandle<T>::destroy_object(const mxArray *mxh)
{
	//std::cout << "destroy_object" << std::endl;
	Collector<T>* coll = Collector<T>::Instance ();
	coll->delete_handle(mxh);
	delete this;
}


template <typename T>
ObjectHandle<T>::ObjectHandle(T*& ptr) : type(&typeid(T)), owns(true), t(ptr) { 
		signature = this; 
		Collector<T>* coll = Collector<T>::Instance ();
		coll->register_handle(this);
		ptr = 0;
	} 

template <typename T>
ObjectHandle<T>::ObjectHandle(T& obj) : type(&typeid(T)), owns(false), t(&obj) { 
		signature= this; 
	}

template <typename T>
ObjectHandle<T>::~ObjectHandle() { 
		std::cout << "Delete ObjectHandle" << std::endl;
		if (owns) {
			delete t; // destroy object
		}
		signature= 0; // destroy signature
	} 
/**
* Create a numeric array as handle for an ObjectHandle.
* We ASSUME we can store object pointer in the mxUINT64 (64 Bit arichtecture) element of mxArray.
*/
template <typename T>
mxArray* ObjectHandle<T>::to_mex_handle() 
{
	mxArray* handle  = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	Collector<T>* coll = Collector<T>::Instance ();
	ObjectHandle<T> *Ohandle = coll->find_ptr(this);
	*reinterpret_cast<ObjectHandle<T>**>(mxGetPr(handle)) = Ohandle;
	return handle;
}
/**
* --------------------------------------------------------- 
* ---------- Implementation of member functions ----------- 
* --------------------------------------------------------- 
*
* Import a handle from MatLab as a mxArray of UINT32. Check that
* it is actually a pointer to an ObjectHandle<T>.
*/
template <typename T>
ObjectHandle<T>* ObjectHandle<T>::from_mex_handle(const mxArray* handle) 
{
	if (mxGetClassID(handle) != mxUINT64_CLASS 
		|| mxIsComplex(handle) || mxGetM(handle)!=1 || mxGetN(handle)!=1)
		mexErrMsgTxt("Parameter is not an ObjectHandle type.");

	// We *assume* we can store ObjectHandle<T> pointer in the mxUINT32 of handle
	Collector<T>* coll = Collector<T>::Instance ();
	double test = coll->find_handle(handle);
	ObjectHandle* obj = NULL;
	//std::cout << obj->type->name() << " " <<   typeid(T).name() << std::endl;
	if (test == *mxGetPr(handle)) {
	      //std::cout << "Cast" << std::endl;
	      obj = *reinterpret_cast<ObjectHandle<T>**>(mxGetPr(handle));
	}
	return obj;
}




#endif
