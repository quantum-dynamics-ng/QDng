#ifndef TYPEDEFS_H
#define TYPEDEFS_H


namespace QDLIB {
   	       
   /**
    * Counter/index integer
    * 
    * This should be used for all indices.
    * Helps us to avoid potential size problems
    * 
    */
   typedef  int lint;
}


#include "dcomplex.h"
#include "tnt/tnt.h"
#include "Vector.h"
#include "VectorView.h"
#include "Matrix.h"

#ifdef _OPENMP
 #include <omp.h>
#endif
	       
namespace QDLIB {
   /**
    * Enable multiplication of type double with arbitrary Types of Vector.
    */
   template <class T>
   Vector<T> operator*(const double &a, const Vector<T> &A)
   {
      lint N = A.dim();
      Vector<T> r(N);
   
      for (int i=0; i<N; i++)
      r[i] = A[i] * a;
   
      return r;
   }

   /**
    * Enable multiplication of type double with arbitrary Types of Vector.
    */   
   template <class T>
   inline Vector<T> operator*(const Vector<T> &A, const double& a)
   {
	   return a * A;
   }
   
   
   using namespace QDLIB;
   /**
    * Enable scalar product for complex vectors.
    */
   inline dcomplex operator*(const Vector<dcomplex> &A, const Vector<dcomplex> &B)
   {
       lint N = A.size();
       assert(N == B.size());
   
       lint i;
       dcomplex sum = 0;
   
       for (i=0; i<N; i++)
	   sum += (A[i]).conj() * B[i];
       return sum;
   }

   /**
    * Define an operation for dMat * cVec
    */
   template <class T>
   inline Vector<dcomplex> operator*(const Matrix<T> &A, const Vector<dcomplex> &B)
   {
      lint N = B.size();
      Vector<dcomplex> r(N);
   
      for (int i=0; i<N; i++){
	 r[i] = 0;
	 for (int j=0; j<N; j++){
            r[i] += A[i][j] * B[j];
	 }
      }
   
      return r;
   }



   /** Variant of TNT Vector of doubles */
   typedef QDLIB::Vector<double> dVec;
   
   /** Variant of TNT Vector of dcomplex */
   typedef QDLIB::Vector<dcomplex> cVec;

   /** Variant of TNT Matrix of doubles */
   typedef QDLIB::Matrix<double> dMat;
   
   /** Variant ofTNT Matrix of dcomplex */
   typedef QDLIB::Matrix<dcomplex> cMat;

   /** Multidimensional view on dVec */
   typedef QDLIB::VectorView<double> dVecView;
   
   /** Multidimensional view on dVec */
   typedef QDLIB::VectorView<dcomplex> cVecView;
   
   
   
/*   inline void MultMatTranspose(dMat *A, cVec* B)
   {
      int rows = A->num_rows();
      int cols = A->num_cols();
      lint strides = A->strides();
      
      dcomplex c;
      dcomplex *a;
      
      for (int i=0; i < rows; i++){
// 	 for (s=0; s < strides; s++){
	    dcomplex *a;
	    for (int i=0; i < cols; i++){
	       
	    }
	 }
      }
   }*/
   
   
   /**
    * Exponentiate elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void ExpElements(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 for (lint i=0; i < size; i++){
	    {
	       a[i] = cexp(b[i] * c);
	    }
	 }
      }
   }

      /**
    * Exponentiate elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void ExpElements(cVec *A, dVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      double *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 for (lint i=0; i < size; i++){
	    {
	       a[i] = cexp(b[i] * c);
	    }
	 }
      }
   }
   
    /**
    * Add vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *b;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 for (lint i=0; i < size; i++){
	    a[i] += b[i];
	 }
      }
   }

   /**
    * Add vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *b;
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         for (lint i=0; i < size; i++){
            a[i] += b[i] * d;
         }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * \param C    Complex Vector, result
    * \param A    Complex vector
    * \param B    Vector is interpreted as imaginary part.
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
   */
   inline void MultElementsComplex(cVec *C ,cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *c=NULL;
      dcomplex *a=NULL;
      double *b=NULL;
      
      dcomplex comp(0,0);
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
         c = C->begin(s);
	 for (lint i=0; i < size; i++){
            c[i] *= a[i] * I * b[i] * d;
	 }
      }
   }
   
      /**
    * Multiply vectors by elements.
    * 
    * \param A    Complex vector
    * \param B    Vector is interpreted as imaginary part.
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void MultElementsComplex(cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      double *b=NULL;
      
      dcomplex comp(0,0);
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         for (lint i=0; i < size; i++){
            a[i] *= I * b[i] * d;
         }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 for (lint i=0; i < size; i++){
	    {
	       a[i] *= b[i];
	    }
	 }
      }

   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(dVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      double *a=NULL;
      double *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++)
	 a = A->begin(s);
         b = B->begin(s);
	 for (lint i=0; i < size; i++){
	 {
	    a[i] *= b[i];
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      double *b;
      
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
         b = B->begin(s);
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= b[i];
	 }
      }
   }

   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, cVec *B, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *b;
      
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
#ifdef _OPENMP
	 #pragma omp parallel for
#endif
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= b[i] * c;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      double *b;
      
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
#ifdef _OPENMP
         #pragma omp parallel for
#endif
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= b[i] * c;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *C, cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *c;
      double *b;
      
      
      lint s;
//#pragma omp parallel private(a,b,c)
  //    {
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 c = C->begin(s);
#ifdef _OPENMP
         #pragma omp parallel for
#endif
	 for (lint i=0; i < size; i++)
	 {
	    c[i] = a[i] * b[i] * d;
	 }
      }
     // } /* parallel */
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void MultElements(cVec *C, cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *c;
      double *b;
      
//       #pragma omp parallel private(a,b,c)
//       {
      lint s;
//       #pragma omp for private(s)
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 c = C->begin(s);
//          #pragma omp for
	 for (lint i=0; i < size; i++)
	 {
	    c[i] = a[i] * b[i];
	 }
      }
//       } /* parallel */
   }
   
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      double *b;
      
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
#ifdef _OPENMP
         #pragma omp parallel for
#endif
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= b[i] * c;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, double c)
   {      
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= c;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(dVec *A, double c)
   {      
      lint size = A->lsize();
      lint strides = A->strides();
      
      double *a;
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= c;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
#ifdef _OPENMP
         #pragma omp parallel for
#endif
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= c;
	 }
      }
   }
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElementsCopy(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 for (lint i=0; i < size; i++){
	    {
	       a[i] = d * b[i];
	    }
	 }
      }

   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         for (lint i=0; i < size; i++){
            {
               a[i] += c * b[i];
            }
         }
      }

   }
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElementsCopy(cVec *C, cVec *A, dcomplex d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *c;

      lint s;

      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 c = C->begin(s);
	 for (lint i=0; i < size; i++)
	 {
	    c[i] = a[i] * d;
	 }
      }
   }
   
}



#endif /* TYPEDEFS_H */ 
