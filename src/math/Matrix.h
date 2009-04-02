#ifndef QDLIBMATRIX_H
#define QDLIBMATRIX_H

#include "Vector.h"
#include "config.h"

namespace QDLIB {

   /**
    * Dense matrix class.
    *
    * This Matrix class uses a coloumn major ordering (Fortran order)
    * to comply natively with LAPACK routines.
    *
    * The matrix is not initialized with values by default.
    * 
    * \todo Add support for strides & strided vectors
    *	 @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   template <class T>
   class Matrix {
      private:
	 lint _m;   /* rows */
	 lint _n;   /* cols */
	 lint _mn;  /* total size */
	 T* _v;     /* pointer to storage */
	 T** _col;  /* Coloumn pointers */
	 
	 void _init(lint m, lint n);
	 void _destroy();
      public:
         Matrix();
	 Matrix(int i, int j);
	 ~Matrix();
	 
	 void newsize(lint m, lint n);
	 
	 T* begin();
	 T* begin(lint col);
	 
	 lint rows() const { return _m;}
	 lint cols() const { return _n;}
	 
	 lint sizeBytes();
	 
	 Vector<T>* coloumn(lint c) const;
	 Vector<T>* diag() const;
	 
	 T& operator() (int i, int j);
	 
	 void operator=(const Matrix<T> &M);
	 
	 void operator=(T scalar);
	 
	 
   
   };

   
   /* 
    * Class Implementations
    */
   
   /**
    * Array initialization.
    */
   template <class T>
   void Matrix<T>::_init(lint m, lint n)
   {
      _mn = m*n;
      _m = m;
      _n = n;
      
#ifdef HAVE_STDLIB_H
      posix_memalign((void**) (&_v), QDLIB_DATA_ALIGNMENT, sizeof(T) * _mn );
#else
      _v = new T[_nm]; 
#endif
      _col = new T*[_n];
      
      /* init col pointers */
      T *p = _v;
      for(int i=0; i < _n; i++){
	 _col[i] = p;
	 p += m;
      }
   }

   template <class T>
	 void Matrix<T>::_destroy()
   {
#ifdef HAVE_STDLIB_H
      free( (void*) _v);
#else
      delete[] _v;
#endif
      delete[] _col;
      
      _mn=0;
      _m=0;
      _n=0;
   }
	   
   /**
    * Default constructor
    */
   template <class T>
   Matrix<T>::Matrix() : _m(0), _n(0), _mn(0)
   {}
   
   /**
    * Constructor with init size.
    */
   template <class T>
   Matrix<T>::Matrix(int i, int j) : _m(i), _n(j), _mn(i*j)
   {
      _init(i,j);
   }
   
   
   template <class T>
   Matrix<T>::~Matrix()
   {
      _destroy();
   }
   
   /**
    * Resize the matrix.
    * 
    * Fresh memory is allocated if needed.
    */
   template <class T>
   void Matrix<T>::newsize(lint m, lint n)
   {
      if (m != _m || n != _n){
	 _destroy();
	 _init(m,n);
      }
   }
   
   /**
    * Returns a pointer to the storage.
    */
   template <class T>
   T* Matrix<T>::begin()
   {
      return _v;
   }
   
   /**
    * Returns a pointer to coloumn.
    */
   template <class T>
   T* Matrix<T>::begin(lint col)
   {
      return _col[col];
   }
	 
   template <class T>
   lint Matrix<T>::sizeBytes()
   {
      return _mn * sizeof(T);
   }
	 
   /**
    * Returns a copy of a column vector.
    */
   template <class T>
   Vector<T>* Matrix<T>::coloumn(lint c) const
   {
      Vector<T>* covec = new Vector<T>(_m, _col[c]);
      
      return covec;
   }
   
   /**
    * Returns a copy of the diagonal.
    * 
    * Only works if number of rows and cols are equal.
    */
   template <class T>
   Vector<T>* Matrix<T>::diag() const
   {
      if (_m != _n) return NULL;
      
      Vector<T>* diagvec = new Vector<T>(_m);
      
      for(int i=0; i < _m; i++)
      {
	 (*diagvec)[i] = _col[i][i];
      }
      return diagvec;
   }
   
   /**
    * Access Matrix elements.
    * 
    * \param i row
    * \param j coloumn
    */
   template <class T> 
   T& Matrix<T>::operator()(int i, int j)
   {
      return _v[j][i];
   }
   
   /**
    * Copy operator.
    */
   template <class T>
   void Matrix<T>::operator=(const Matrix<T> &M)
   {
      if (M._m != _m || M._n != _n){
	 _destroy();
	 _init(M._m,M._n);
      }
#ifdef HAVE_STRING_H
      memcpy( (void*) _v, (void*) M._v, sizeof(T) * _mn);
#else
      for (int i=0; i < _mn; i++)
	 _v[i] = M._v[i];
#endif
   }
   
   /**
    * Fill the matrix with scalar value.
    */
   template <class T>
   void Matrix<T>::operator=(T scalar)
   {
      for (int i=0; i < _mn; i++)
	 _v[i] = scalar;
   }
   
   /*
    * Matrix operations
    */
   
   /**
    * b = M*a
    */
   template <class T, class U>
   inline void MatVecMult(Vector<T> *B, Matrix<U> *M, Vector<T> *A, bool transpose = false)
   {
      int cols = M->cols();
      int rows = M->rows();
      
      T *b = B->begin(0);
      T *a = A->begin(0);
      U** m = M->begin();
      
      if (transpose){
	 for(int i=0; i < rows; i++){
	    b[i] = 0;
	    for(int j=0; j < cols; j++){
	       b[i] += m[i][j] * a[i];
	    }
	 }
      } else {
	 for(int i=0; i < rows; i++){
	    b[i] = 0;
	    for(int j=0; j < cols; j++){
	       b[i] += m[j][i] * a[i];
	    }
	 }
      }
   }
   
}

#endif
