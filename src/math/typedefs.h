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
//#include "tnt/tnt.h"
#include "Vector.h"
#include "VectorView.h"
#include "Matrix.h"

#ifdef _OPENMP
 #include <omp.h>
#endif
	       
#ifdef HAVE_SSE2
 #include "cplx_sse2.h"
#endif
	       
using namespace std;
	 
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
            r[i] += A(i,j) * B[j];
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
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
   inline void AddElementsEq(cVec *C, cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *b;
      dcomplex *c;

      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 c = C->begin(s);
	 lint i;

#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
	    c[i] = a[i] + b[i];
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
         lint i;

#ifdef HAVE_SSE2
	 __m128d ma, mb;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i,ma,mb)
#endif
	 for (i=0; i < size; i++){
	    ma = _mm_set_pd(a[i]._imag, a[i]._real);
	    mb = _mm_set_pd(b[i]._imag, b[i]._real);
	    ma = _mm_add_pd(ma, mb);
	    QDSSE::Store(a[i], ma);
	 }
#else
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
	    a[i] += b[i];
	 }
#endif
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
         lint i;
#ifdef HAVE_SSE2
	 __m128d ma, mb, md;
	 md = _mm_set1_pd(d);
 #ifdef _OPENMP
 #pragma omp parallel for default(shared) private(i,ma,mb)
 #endif
	 for (i=0; i < size; i++){
	    ma = _mm_set_pd(a[i]._imag, a[i]._real);
	    mb = _mm_set_pd(b[i]._imag, b[i]._real);
	    mb = _mm_mul_pd(mb, md);
	    ma = _mm_add_pd(ma, mb);
	    QDSSE::Store(a[i], ma);
	 }
#else
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] += b[i] * d;
         }
#endif
      }
   }
   
   /**
    * Add up vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      dcomplex *b;
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] += b[i] * c;
         }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * \f$ c_i = a_i i b_i d \f$
    * 
    * \param C    Complex Vector, result
    * \param A    Complex vector
    * \param B    Vector is interpreted as imaginary part.
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElementsComplexEq(cVec *C ,cVec *A, dVec *B, double d)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            c[i] = a[i] * (I * b[i]) * d;
         }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * \f$ c_i = c_i a_i i b_i d \f$
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
            c[i] *= a[i] * I * b[i] * d;
	 }
      }
   }
   
   /**
    * Multiply vectors by elements.
    * \f$ c_i = a_i^*  b_i d \f$
    * 
    * \param C    Complex Vector, result
    * \param A    Complex vector
    * \param B    Complex vector
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
   */
   inline void MultElementsConugate(cVec *C ,cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *c=NULL;
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      dcomplex comp(0,0);
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
            c[i] = a[i].conj()  * b[i] * d;
	 }
      }
   }
   
         /**
    * Multiply vectors by elements.
    * \f$ a_i = a_i^*  b_i d \f$
    * 
    * \param A    Complex vector
    * \param B    Complex vector
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void MultElementsConugate(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      dcomplex comp(0,0);
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] = a[i].conj() * b[i] * d;
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
         lint i;
#ifdef HAVE_SSE2
	 __m128d ma, mb, md;
	 md = _mm_set1_pd( d );
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i,ma,mb)
#endif
	 for (i=0; i < size; i++){
	    mb = _mm_set1_pd( b[i] );
	    ma = _mm_set_pd( a[i]._real, -1*a[i]._imag);
	    ma = _mm_mul_pd(ma, mb);
	    ma = _mm_mul_pd(ma, md);
	    QDSSE::Store(a[i], ma);
	 }
#else
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] = a[i] * I * b[i] * d;
         }
#endif
      }
   }
   
     /**
    * Multiply vectors by elements.
    * 
    * \param A    Complex vector
    * \param B    Complex vector
    * \param d    Scale the whole vector by d
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void MultElementsComplex(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      dcomplex comp(0,0);
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] = (a[i] * I) * b[i] * d;
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
	    {
	       a[i] *= b[i];
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
   inline void MultElementsCopy(cVec *C, cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      dcomplex *c=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               c[i] = a[i] * b[i];
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
   inline void MultElementsCopy(cVec *C, cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      dcomplex *c=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               c[i] = a[i] * b[i] * d;
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
/*   inline void MultElements(Vector<double> *A, Vector<double> *B)
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
	    a[i] *=  b[i];
	 }
      }
   }*/
   
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
         lint i;
#ifdef HAVE_SSE2
	 __m128d ma, mb;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i,ma,mb)
#endif
	 for (i=0; i < size; i++)
	 {
	    ma = _mm_set_pd(a[i]._imag, a[i]._real);
	    mb = _mm_load1_pd(&(b[i]));
	    ma = _mm_mul_pd(ma, mb);
	    QDSSE::Store(a[i], ma);
	 }
#else
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif

	 for (i=0; i < size; i++)
	 {
	    a[i] *= b[i];
	 }
#endif
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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

      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
	 {
	    c[i] = a[i] * b[i] * d;
	 }
      }
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
      

      lint s;

      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 c = C->begin(s);
         lint i;

#ifdef HAVE_SSE2
	 __m128d ma, mb;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i,ma,mb)
#endif
	 for (i=0; i < size; i++)
	 {
	    ma = _mm_set_pd(a[i]._imag, a[i]._real);
	    mb = _mm_set1_pd(b[i]);
	    ma = _mm_mul_pd(ma, mb);
	    QDSSE::Store(c[i], ma);
	 }
#else
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
	 {
	    c[i] = a[i] * b[i];
	 }
#endif
      }
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               a[i] += c * b[i];
            }
         }
      }

   }
   
   /**
    * Multiply + Add vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a=NULL;
      dcomplex *b=NULL;
      
      lint s;
      for (s=0; s < strides; s++){
	 a = A->begin(s);
	 b = B->begin(s);
	 lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++){
	    {
	       a[i] += d * b[i];
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
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
	 for (i=0; i < size; i++)
	 {
	    c[i] = a[i] * d;
	 }
      }
   }
   
      /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
       */
   inline void MultElementsCopy(cVec *C, cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      double *b;
      dcomplex *c;

      lint s;

      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i] * b[i];
         }
      }
   }
   
         /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
          */
   inline void MultElementsCopy(cVec *C, cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();
      
      dcomplex *a;
      double *b;
      dcomplex *c;

      lint s;

      for (s=0; s < strides; s++){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i] * b[i] * d;
         }
      }
   }
   
}



#endif /* TYPEDEFS_H */ 
