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

#define QDLIB_DATA_ALIGNMENT 16    /* 16 byte - Alignment for SIMD (SSE2) */

#ifdef HAVE_MPI
#include "mpi.h"
#endif

#include "tools/Memory.h"

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
    T** v_;
    lint n_;

    Vector<T> **_StrideRefObj; /** If a stride is just reference, the pointer to master instance is here */
    lint *_SourceStride;       /** Stride Ref source */
    bool _StorageLocked;       /** Storage can not be retired */

    size_type nstrides_;
    size_type stride_size_;

    bool isRef_;

    int _mpirank;
    int _mpisize;

#ifdef HAVE_MPI
    MPI::Intracomm *_comm;
#endif
    
    
    /**
     * Initialize/allocate memory for the vector.
     * 
     * \param N       size of the vector.
     * \param strides number of strides.
     */
    void initialize(lint N, lint strides)
    {
#ifdef HAVE_MPI
       if (_comm != NULL){
          _mpirank = _comm->Get_rank();
          _mpisize = _comm->Get_size();
       }
#endif
      nstrides_ = strides;
      stride_size_ = N / strides;
      n_ = N;
      
      v_ = new T*[strides];
      _StrideRefObj = new Vector<T>*[strides];
      _SourceStride = new lint[strides];
      
      isRef_ = false;
      _StorageLocked = false;
      
      for (lint i=0; i < nstrides_; i++){
         if (stride_size_ == 0)
            v_[i] = NULL;
         else {
            Memory& mem = Memory::Instance();
            mem.Align((void**) &(v_[i]), sizeof(T)*stride_size_);
         }
         _StrideRefObj[i] = NULL;
         _SourceStride[i] = -1;
      }
    }
   
    /** copy only part out of the strides. */
    void copy(T* const *v, lint begin, lint n)
    {
       for (lint s=0; s < nstrides_; s++)
          for (lint i=0; i< n; i++)
             v_[s][i] = v[s][begin+i];
    }
       
    void copy(T* const *v)
    {
	for (lint s=0; s < nstrides_; s++){
           lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	   for (i=0; i< stride_size_; i++)
            v_[s][i] = v[s][i];
        }
    }

    void copy(const T *v)
    {
       for (lint s=0; s < nstrides_; s++){
          lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	  for (i=0; i< stride_size_; i++)
	     v_[s][i] = v[i];
       }
    }

    
    void set(const T& val)
    {
       for (lint s=0; s < nstrides_; s++){
          lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	  for (i=0; i< stride_size_; i++){
	     v_[s][i] = val;
	  }
       }
	     

    }
    


    void destroy()
    {     
        /* do nothing, if no memory has been previously allocated */
       if (v_ != NULL && !isRef_){  /* Only destroy data if it's our own*/
         for (lint s=0; s < nstrides_; s++){
            if(v_[s] != NULL){
               Memory::Instance().Free(v_[s]);
               v_[s] = NULL;
            }
         }
       }
       if (v_ != NULL) { /* This pointer is always our own. */
          delete[] v_;
          v_ = NULL;
       }
       if (_StrideRefObj != NULL) {
          delete[] _StrideRefObj;
          delete[] _SourceStride;
          _StrideRefObj = NULL;
          _SourceStride = NULL;
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
        * \param dest   The destination stride (Number must be larger!!!)
        */
       bool StrideRef(Vector<T> &vec, lint source, lint dest = 0)
         {
            if (isRef_ && vec.stride_size_ != stride_size_)
               return false;

            if (!isRef_) {
               if (dest >= nstrides_)
                  return false;
               if (stride_size_ > 0)
                  destroy();

               initialize(0, nstrides_);

               stride_size_ = vec.stride_size_;
               isRef_ = true;
            }
            if (nstrides_ <= dest + 1)
               nstrides_ = dest + 1;

            n_ = nstrides_ * stride_size_;
            v_[dest] = vec.v_[source];
            _StrideRefObj[dest] = &vec;
            _SourceStride[dest] = source;

            return true;
         }
        
       /**
        * Reduce the size of the vector.
        * 
        * This is an irreversibel action.
        */
       void downsize(int N)
       {
          if (N <= n_) n_ = N;
       }
       
  public:

       /**
        * Lock the internal storage against retirement.
        */
       void LockStorage() { _StorageLocked = true; }
       void UnLockStorage() { _StorageLocked = false; }

       /**
        * Frees the data storage but not the properties.
        *
        * Use this feature to save memory. Advantegous if
        * the object is used as temporary buffer.
        */
       void RetireStorage()
       {
          if (_StorageLocked)  return;
          for (int s=0; s < nstrides_; s++){
             if (_StrideRefObj[s] == NULL)
                Memory::Instance().Free(v_[s]);
             else
                _StrideRefObj[s]->RetireStorage();

             v_[s] = NULL; /* Invalidate pointer */
          }
       }

       /**
        * Get back storage freed with RetireStorage.
        */
       void ReaquireStorage()
       {
          if (_StorageLocked)  return;
          for (int s=0; s < nstrides_; s++){
             if (v_[s] == NULL){ /* Check if reaquire is needed*/
                if ( _StrideRefObj[s] == NULL )
                   Memory::Instance().Align((void**) &(v_[s]), sizeof(T)*stride_size_);
                else {
                   _StrideRefObj[s]->ReaquireStorage();
                   v_[s] = _StrideRefObj[s]->v_[_SourceStride[s]];  /* Re-link the stride */
                }
             }
          }

       }

       /**
        * Returns the rank in the stride distribution.
        */
       int MPIrank()
       {
          return _mpirank;
       }

       /**
        * Returns the number of procs over which the strides are distributed.
        */
       int MPIsize()
       {
          return _mpisize;
       }

#ifdef HAVE_MPI
       /**
        * Returns the communicator over which the strides are spread.
        *
        * \return NULL the vector is pure local.
        */
       MPI::Intracomm* GetComm()
       {
          return _comm;
       }
#endif
       /**
        * Sync Strides between processes.
        *
        * The strides of the local rank are broadcasted to all other ranks.
        * Function is empty if mpi is not used.
        */
       void SyncStrides()
       {
#ifdef HAVE_MPI
          for (int s=0; s < nstrides_; s++){
             MPI::COMM_WORLD.Bcast( (void*) v_[s], sizeof(T) * stride_size_, MPI_BYTE, s % _mpisize);
          }
#endif
       }
    
   /**
    * Return a pointer to the local stride.
    */
    inline T* begin(lint stride) const { return v_[stride];}
    
    /**
     * Return a pointer to the stride set.
     * 
     * This is a 2D array with sizes [strides][stride_size]
     */
    T** begin() const { return v_;}
    
    /**
     * Get Size of a stride in bytes.
     */
    size_t sizeBytes() const { return stride_size_ * sizeof(T); }
    
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
      if (n_ == A.n_ && nstrides_ == A.nstrides_) {
         /* no need to re-alloc */
         lint s;
#ifdef _OPENMP  
#pragma omp parallel for default(shared) private(s)
#endif
         for (s = 0; s < nstrides_; s++) {
            Memory::Copy( (char*) v_[s], (char*) A.v_[s], sizeof(T) * stride_size_);

         }
      } else {
         if (!isRef_)
            destroy();

         initialize(A.n_, A.nstrides_);

         for (lint s = 0; s < nstrides_; s++)
            Memory::Copy((char*) v_[s], (char*) A.v_[s], sizeof(T) * stride_size_);
      }
   }
    
    // destructor
    ~Vector() 
    {
      destroy();
    }

    // constructors

    Vector() : n_(0),
               nstrides_(1),
               stride_size_(0),
               isRef_(false),
               _mpirank(0),
               _mpisize(1)
#ifdef HAVE_MPI
              ,_comm(NULL)
#endif
    {
       initialize(n_, nstrides_);
    }

    
    Vector(const Vector<T> &A) : n_(0),
                                 nstrides_(1),
                                 stride_size_(0),
                                 isRef_(false),
                                 _mpirank(0),
                                 _mpisize(1)
#ifdef HAVE_MPI
                                 , _comm(NULL)
#endif
    {
       initialize(A.n_, A.nstrides_);
        copy(A.v_);
    }
   
    Vector(lint N) : n_(0),
                     nstrides_(1),
                     stride_size_(0),
                     isRef_(false),
                     _mpirank(0),
                     _mpisize(1)
#ifdef HAVE_MPI
                    ,_comm(NULL)
#endif
    {
       initialize(N, nstrides_);
    }
    
    Vector(lint N, lint strides) : n_(0),
                                   nstrides_(0),
                                   stride_size_(0),
                                   isRef_(false),
                                   _mpirank(0),
                                   _mpisize(1)
#ifdef HAVE_MPI
                                   ,_comm(NULL)
#endif
    {
       initialize(N, strides);
    }
    
    Vector(lint N, const T& value) : n_(0),
                                     nstrides_(1),
                                     stride_size_(0),
                                     isRef_(false),
                                     _mpirank(0),
                                     _mpisize(1)
#ifdef HAVE_MPI
                                     ,_comm(NULL)
#endif
    {
        initialize(N, nstrides_);
        set(value);
    }

    Vector(lint N, lint strides, const T** v) :  n_(N),
                                                 nstrides_(strides),
                                                 stride_size_(0),
                                                 isRef_(false),
                                                 _mpirank(0),
                                                 _mpisize(1)
#ifdef HAVE_MPI
                                                 ,_comm(NULL)
#endif
    {
        initialize(N, strides);
        copy(v);
    }

    Vector(lint N, const T* v) :  n_(0),
                                  nstrides_(1),
                                  stride_size_(0),
                                  isRef_(false),
                                  _mpirank(0),
                                  _mpisize(1)
#ifdef HAVE_MPI
                                  ,_comm(NULL)
#endif
    {
       initialize(N, nstrides_);
       copy(v);
    }
    
    /**
     * Resize the vector.
     */
    Vector<T>& newsize(lint N)
    {
        if (n_ == N) return *this;

        if (!isRef_) destroy();
        initialize(N, 1);

        return *this;
    }

    /**
     * Resize the vector with multiple strides.
     * 
     * \param N   Total size
     * \param s   Number of strides
     * \param comm Intracommunicator for stride split.
     */
#ifdef HAVE_MPI
    Vector<T>& newsize(lint N, lint s, MPI::Intracomm* comm = NULL)
    {
       _comm = comm;
#else
    Vector<T>& newsize(lint N, lint s)
    {
#endif

       if (n_ == N && nstrides_ == s) return *this;

       destroy();
       initialize(N, s);

       return *this;
    }
    

    void operator *=(const T &scalar)
    {
       for (lint s=0; s < nstrides_; s++){
	  lint i;
#ifdef _OPENMP    
#pragma omp parallel for default(shared) private(i)
#endif	  
          for (i=0; i< stride_size_; i++){
             v_[s][i] *= scalar;
          }
       }
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
    
    /**
     */
    Vector<T>& CopySub(Vector<T> &A, lint begin, lint end)
    {
       if (v_ == A.v_)
          return *this;

       lint rsize = abs(end-begin)+1;
       
       if ( abs(rsize) > A.n_) return *this;
	
       if (n_ >= rsize  && nstrides_ == A.nstrides_){
          /* no need to re-alloc */
          copy(A.v_, begin, rsize);
       } else {
          if (!isRef_) destroy();
          initialize(rsize, A.nstrides_);
          copy(A.v_, begin, rsize);
       }
       return *this;
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
     * Gives the global end index+1 of the actual stride.
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


    inline T Norm() const
    {
       T sum=0;
       for (lint i=0; i < n_; i++)
          sum += conj((*this)[i]) * (*this)[i];

       return sqrt(sum);
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
void operator+=(Vector<T> &A, const T val)
{
   lint N = A.size();

   for (int i=0; i<N; i++)
      A[i] += val;
}

template <class T>
void operator-=(Vector<T> &A, const T val)
{
   lint N = A.size();

   for (int i=0; i<N; i++)
      A[i] -= val;
}


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

