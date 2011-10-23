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
	 
         bool _aligned;  /* Memaligned */
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
	 lint strides() {return 1;}
         
	 Vector<T>* coloumn(lint c) const;
	 Vector<T>* diag() const;
         void diag(Vector<T>* diagonal);
         
	 void transpose();
         
	 T& operator() (int i, int j);
         T& operator() (int i, int j) const;
	 
	 void operator=(const Matrix<T> &M);
	 
	 void operator=(T scalar);
	 
	 void operator*=(T scalar);
	 
   
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
      
      Memory& mem = Memory::Instance();
      mem.Align((void**) (&_v), sizeof(T) * _mn);

      _col = new T*[_n];
      
      /* init col pointers */
      for(int i=0; i < _n; i++){
	 _col[i] = &(_v[i*_m]);
	 
      }
   }

   template <class T>
	 void Matrix<T>::_destroy()
   {
      if (_v != NULL) {
         Memory::Instance().Free( (void*) _v);
         delete[] _col;
      }
      _mn=0;
      _m=0;
      _n=0;
   }
	   
   /**
    * Default constructor
    */
   template <class T>
   Matrix<T>::Matrix() : _m(0), _n(0), _mn(0), _v(NULL), _col(NULL)
   {}
   
   /**
    * Constructor with init size.
    */
   template <class T>
         Matrix<T>::Matrix(int i, int j) : _m(i), _n(j), _mn(i*j), _v(NULL), _col(NULL)
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
    * Set the diagonal from vector.
    */
   template <class T>
   void Matrix<T>::diag(Vector<T>* diagvec)
   {
      if (_m != _n || _m < diagvec->size() ) return;
      
      for(int i=0; i < _m; i++)
      {
         _col[i][i] = (*diagvec)[i];
      }
   }
   
   /**
    * Transpose the Matrix.
    * 
    * Only works if number of rows and cols are equal.
    */
   template <class T>
   void Matrix<T>::transpose()
   {
      if (_m != _n) return;
      
      T swap;
      for (int i=0; i < _m; i++) {
         for (int j=0; j < i; j++) {
            swap=_col[i][j];
            _col[i][j] = _col[j][i];
            _col[j][i] = swap;
         }
      }
   }
   
   /**
    * Access Matrix elements.
    * 
    * \param i row
    * \param j coloumn
    */
   template <class T> 
   inline T& Matrix<T>::operator()(int i, int j)
   {
      return _col[j][i];
   }
   
   template <class T> 
   inline T& Matrix<T>::operator()(int i, int j) const
   {
      return _col[j][i];
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
   
   /**
    * Scale the matrix.
    */
   template <class T>
   void Matrix<T>::operator*=(T scalar)
   {
      for (int i=0; i < _mn; i++)
	 _v[i] *= scalar;
   }
   
   /*
    * Matrix operations
    */
   
   /**
    * Matrix - Vector Multiplication.
    * 
    * \f$ b = M*a \f$
    * 
    * \param transpose Use the transpose of M
    * \param adjoint   Use adjoint matrix.
    */
   template <class T, class U>
   inline void MatVecMult(Vector<T> *B, Matrix<U> *M, Vector<T> *A, bool transpose = false, bool adjoint = false)
   {
      int cols = M->cols();
      int rows = M->rows();
      
      if (cols != A->size() && !transpose) return;
      if (rows != A->size() && transpose) return;
      
      if (B->size() != A->size())
         B->newsize(A->size());
      
      T *b = B->begin(0);
      T *a = A->begin(0);
      
      if (transpose && !adjoint){
	 for(int i=0; i < rows; i++){
            b[i] = T(0);
	    for(int j=0; j < cols; j++){
	       b[i] += (*M)(j,i) * a[j];
	    }
	 }
      } else if (transpose && adjoint){
         for(int i=0; i < rows; i++){
            b[i] = T(0);
            for(int j=0; j < cols; j++){
               b[i] += conj((*M)(j,i)) * a[j];
            }
         }         
      }
      else {
	 for(int i=0; i < rows; i++){
	    b[i] = T(0);
	    for(int j=0; j < cols; j++){
               b[i] += (*M)(i,j) * a[j];
	    }
	 }
      }
   }
   
   
   /**
    * ASCII output.
    */
   template <class T>
   std::ostream& operator<<(std::ostream &s, const Matrix<T> &A)
   {
      lint M=A.rows();
      lint N=A.cols();

      s << M << " " << N << "\n";
      for (lint i=0; i<M; i++)
      {
         for (lint j=0; j<N; j++)
         {
            s << A(i,j) << " ";
         }
         s << "\n";
      }


      return s;
   }
}

#endif
