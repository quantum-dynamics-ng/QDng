/*
*
* Template Numerical Toolkit (TNT)
*
* Mathematical and Computational Sciences Division
* National Institute of Technology,
* Gaithersburg, MD USA
*
*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*
*/



#ifndef QDLIBVECTOR_H
#define QDLIBVECTOR_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string.h>

#define TNT_MAX_STRIDES 32
#define QDLIB_DATA_ALIGNMENT 16    /* 16 byte - Alignment for SIMD */

using namespace std;

namespace QDLIB
{

//namespace Linear_Algebra
// {


/**
 * Linear algebra vector. 
 * 
 * Supports strided vectors which are discontinous in memory.
 */
template <class T>
class Vector 
{


  public:

    typedef lint   size_type;
    typedef         T   value_type;
    typedef         T   element_type;
    typedef         T*  pointer;
    typedef         T*  iterator;
    typedef         T&  reference;
    typedef const   T*  const_iterator;
    typedef const   T&  const_reference;

    lint lbound() const { return 1;}
 
    
  private:
    T* v_[TNT_MAX_STRIDES];
    lint n_;

    size_type nstrides_;
    size_type stride_size_;
    
    bool isRef_;
    
    bool align_;
    // internal helper function to create the array
    // of row pointers

    /**
     * Initialize/allocate memory for the vector.
     * 
     * \param N       size of the vector.
     * \param strides number of strides.
     */
    void initialize(lint N, lint strides)
    {
      // adjust pointers so that they are 1-offset:
      // v_[] is the internal contiguous array, it is still 0-offset
      nstrides_ = strides;
      stride_size_ = N / strides;
      n_ = N;
      
      for (lint i=0; i < nstrides_; i++){
	 if (align_){
	    posix_memalign((void**) &v_[i], QDLIB_DATA_ALIGNMENT, sizeof(T)*stride_size_);
	 } else {
            v_[i] = new T[stride_size_];
	 }
      }

      
    }
   
    void copy(T* const *v)
    {
	for (lint s=0; s < nstrides_; s++)
	   for (lint i=0; i< stride_size_; i++)
            v_[s][i] = v[s][i];
    }

    void copy(const T *v)
    {
       for (lint s=0; s < nstrides_; s++)
	  for (lint i=0; i< stride_size_; i++)
	     v_[s][i] = v[i];
    }

    
    void set(const T& val)
    {
       for (lint s=0; s < nstrides_; s++){
	  for (lint i=0; i< stride_size_; i++){
	     v_[s][i] = val;
	  }
       }
	     

    }
    


    void destroy()
    {     
        /* do nothing, if no memory has been previously allocated */
       for (lint s=0; s < nstrides_; s++){
 	  if (v_[s] != NULL){
	     delete [] v_[s];
	     v_[s] = NULL;
 	  }
       }
    }


    protected:
       /**
        * Inject the pointer of a vector into a stride.
        * 
        * Use with extreme care. 
        * If you destroy the original instance data in the strides
        * it will be no longer valid!
        * 
        * \param vec    The vector to inject
        * \param source The source stride in vec
        * \param dest   The destination stride
        */
       bool StrideRef(Vector<T> &vec, lint source, lint dest = 0)
       {
	  if ( isRef_  && vec.stride_size_ != stride_size_ )
	     return false;
	  
	  if (!isRef_){
	     if (dest >= TNT_MAX_STRIDES) return false;
	     destroy();
	     if (nstrides_ < dest) nstrides_ = dest + 1;
	     stride_size_ = vec.stride_size_;
	  }
	
	  v_[dest] = vec.v_[source];
	  
	  return true;
       }
        
       
  public:

    
   /**
    * Return a pointer to the local stride.
    */
    T* begin(lint stride) { return v_[stride];}
    
    /**
     * Return a pointer to the tride set.
     * 
     * This is a 2D array with sizes [strides][stride_size]
     */
    T** begin() { return v_;}
    
    /**
     * Get Size of a stride in bytes.
     */
    size_t sizeBytes() { return stride_size_ * sizeof(T); }
    
    /**
     * Swap the content of two vectors.
     * 
     * Only the pointers to the data will exchanged.
     * Works only if both have the same size.
     */
    bool swap(Vector<T> &A)
    {
       T *p;
       
       if (n_ != A.n_) return false;
       
       for (lint s=0; s < nstrides_; s++){
	 p = v_[s];
	 v_[s] = A.v_[s];
	 A.v_[s] = p;
       }
       return true;
    }
    
    void FastCopy(Vector<T> &A)
    {
       if (n_ == A.n_ && nstrides_ == A.nstrides_){
	  /* no need to re-alloc */
	  for (lint s=0; s < nstrides_; s++)
	     memcpy(v_[s], A.v_[s], sizeof(T) *  stride_size_);
       } else {
	  if (!isRef_) destroy();
	  initialize(A.n_, A.nstrides_);

	  for (lint s=0; s < nstrides_; s++)
	     memcpy(v_[s], A.v_[s], sizeof(T) *  stride_size_);
       }
    }
    
    // destructor
    ~Vector() 
    {
       if (!isRef_) destroy();   /* Only destroy data if it's our own*/
    }

    // constructors

    Vector() : n_(0), nstrides_(1), stride_size_(0), isRef_(false), align_(false)
    {
       for(lint i=0; i < TNT_MAX_STRIDES; i++)
       {
	  v_[i] = NULL;
       }
    }

    
    Vector(const Vector<T> &A) : n_(0), nstrides_(1), stride_size_(0), isRef_(false), align_(false)
    {
       initialize(A.n_, A.nstrides_);
        copy(A.v_);
    }

    Vector(lint N, const bool align = false, const T& value = T()) : n_(0), nstrides_(1), stride_size_(0), isRef_(false), align_(align)
    {
        initialize(N, nstrides_);
        set(value);
    }

    Vector(lint N, lint strides, const T** v) :  n_(N), nstrides_(strides), stride_size_(0), isRef_(false), align_(false)
    {
        initialize(N, strides);
        copy(v);
    }

    Vector(lint N, const T* v) :  n_(0), nstrides_(1), stride_size_(0), isRef_(false), align_(false)
    {
       initialize(N, nstrides_);
       copy(v);
    }


    /**
     * Turn on data alignment.
     * 
     * This works only for non-class type
     */
    void Align()
    {
       align_ = true;
    }
    
    /**
     * Resize the vector.
     */
    Vector<T>& newsize(lint N)
    {
        if (n_ == N) return *this;

        if (!isRef_) destroy();
        initialize(N, 1);
	isRef_ = false;

        return *this;
    }

    /**
     * Resize the vector with multiple strides.
     * 
     * \param N   Total size
     * \param s   Number of strides
     */
    Vector<T>& newsize(lint N, lint s)
    {
       if (n_ == N && nstrides_ == s) return *this;

       destroy();
       initialize(N, s);

       return *this;
    }
    

    // assignments
    //
    Vector<T>& operator=(const Vector<T> &A)
    {
        if (v_ == A.v_)
            return *this;

	
        if (n_ == A.n_ && nstrides_ == A.nstrides_){
	   /* no need to re-alloc */
            copy(A.v_);
	} else {
            if (!isRef_) destroy();
            initialize(A.n_, A.nstrides_);
            copy(A.v_);
        }
        return *this;
    }
        
    Vector<T>& operator=(const T& scalar)
    { 
        set(scalar);
        return *this;
    }

    /**
     * Copy a vector into a single stride.
     *
     * \param vec    The vector to inject
     * \param source The source stride in vec
     * \param dest   The destination stride
     */
    bool StrideCopy(Vector<T> &vec, lint source, lint dest = 0)
    {
       if ( dest > nstrides_ || vec.stride_size_ != stride_size_ || source > vec.strides() )
	  return false;
	
       for (lint i=0; i < stride_size_; i++)
         v_[dest][i] = vec.v_[source][i];
       
       return true;
    }
    
    inline lint size() const
    {
        return  n_;
    }

    /**
     * Gives the local size of the actual stride.
     * 
     * We need this later for vectors which are strided by over multiple processes (e.g. MPI);
     */
    inline lint lsize() const
    {
       return  stride_size_;
    }

    /**
     * Returns the number of strides.
     */
    inline lint strides() const 
    {
       return  nstrides_;
    }

    /**
     * Gives the global start index of the actual stride.
     * 
     * We need this later for vectors which are strided by over multiple processes (e.g. MPI);
     */
    inline lint start() const 
    {
       return  0;
    }

    /**
     * Gives the global end index of the actual stride.
     * 
     * We need this later for vectors which are strided by over multiple processes (e.g. MPI);
     */
    inline lint stop() const 
    {
       return  n_;
    }


    inline reference operator[](lint i)
    { 
	lint s = i / stride_size_;
	lint j = i % stride_size_;
        return v_[s][j];
    }

    inline const_reference operator[](lint i) const
    {
	lint s = i / stride_size_;
	lint j = i % stride_size_;
        return v_[s][j];
    }



};


/* ***************************  I/O  ********************************/

template <class T>
std::ostream& operator<<(std::ostream &s, const Vector<T> &A)
{
    lint N=A.size();

    s <<  N << "\n";

    for (lint i=0; i<N; i++)
        s   << A[i] << " " << "\n";
    s << "\n";

    return s;
}

template <class T>
std::istream & operator>>(std::istream &s, Vector<T> &A)
{

    lint N;

    s >> N;

    if ( !(N == A.size() ))
    {
        A.newsize(N);
    }


    for (lint i=0; i<N; i++)
            s >>  A[i];


    return s;
}

// *******************[ basic matrix algorithms ]***************************


template <class T>
Vector<T> operator+(const Vector<T> &A, 
    const Vector<T> &B)
{
    lint N = A.size();

    assert(N==B.size());

    Vector<T> tmp(N);
    lint i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] + B[i];

    return tmp;
}

template <class T>
Vector<T> operator+=(Vector<T> &A, 
    const Vector<T> &B)
{
   lint N = A.size();

   assert(N==B.size());

    lint i;

    for (i=0; i<N; i++)
            A[i] += B[i];

    return A;
}

template <class T>
Vector<T> operator-(const Vector<T> &A, 
    const Vector<T> &B)
{
   lint N = A.size();

   assert(N==B.size());

    Vector<T> tmp(N);
    lint i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] - B[i];

    return tmp;
}

template <class T>
Vector<T> operator-=(Vector<T> &A, 
    const Vector<T> &B)
{
   lint N = A.size();

   assert(N==B.size());

    lint i;

    for (i=0; i<N; i++)
            A[i] -= B[i];

    return A;
}




template <class T>
Vector<T> elementwise_mult(const Vector<T> &A, const Vector<T> &B)
{
   lint N = A.size();

   assert(N==B.size());

    Vector<T> tmp(N);
    lint i;

    for (i=0; i<N; i++)
            tmp[i] = A[i] * B[i];

    return tmp;
}


template <class T>
double norm(const Vector<T> &A)
{
	lint N = A.size();

	double sum = 0.0;
	for (int i=0; i<N; i++)
		sum +=  abs(A[i])*abs(A[i]);
	return sqrt(sum);
}



template <class T>
inline T dot_prod(const Vector<T> &A, const Vector<T> &B)
{
    lint N = A.size();
    assert(N == B.size());

    lint i;
    T sum = 0;

    for (i=0; i<N; i++)
        sum += A[i] * B[i];
    return sum;
}

template <class T>
inline T dot_product(const Vector<T> &A, const Vector<T> &B)
{
	return dot_prod(A, B);
}


template <class T>
inline T operator*(const Vector<T> &A, 
    const Vector<T> &B)
{
    return dot_prod(A,B);
}


template <class T>
Vector<T> operator*(const T &a, const Vector<T> &A)
{
   lint N = A.size();
	Vector<T> r(N);

	for (int i=0; i<N; i++)
		r[i] = A[i] * a;

	return r;
}

template <class T>
inline Vector<T> operator*(const Vector<T> &A, const T& a)
{
	return a * A;
}


}   /* namespace QDLIB */


#endif

