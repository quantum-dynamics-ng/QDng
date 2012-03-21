/*
 * VectorPrimitives.h
 *
 *  Created on: 30.10.2011
 *      Author: markus
 */

#ifndef VECTORPRIMITIVES_H_
#define VECTORPRIMITIVES_H_

#ifdef _OPENMP
 #include <omp.h>
#endif

#ifdef HAVE_SSE2
 #include "cplx_sse2.h"
 #include "m128dc.h"
#endif

namespace QDLIB {

   /**
    * Exponentiate elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void ExpElements(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
       */
   inline void ExpElements(cVec *A, dVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      double *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void AddElementsEq(cVec *C, cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *b;
      dcomplex *c;


      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;

#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *b;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va, vb;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            va += vb;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *b;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc v;
 #ifdef _OPENMP
 #pragma omp parallel for schedule(static) default(shared) private(i,v)
 #endif
         for (i=0; i < size; i++){
            v = m128dc(b[i]) * d;
            v += m128dc(a[i]);
            v.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void AddElements(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *b;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va, vb, vc(c);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            va += vb * vc;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] += b[i] * c;
         }
#endif
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
    * parallelisation will be done here.
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
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
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
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va,vc;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vc)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vc = c[i];
            vc *= va;
            vc = vc.MulImag(b[i]);
            vc *= d;
            vc.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            c[i] *= a[i] * I * b[i] * d;
         }
#endif
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
    * parallelisation will be done here.
   */
   inline void MultElementsConjugate(cVec *C ,cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *c=NULL;
      dcomplex *a=NULL;
      dcomplex *b=NULL;

      dcomplex comp(0,0);

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
       */
   inline void MultElementsConjugate(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      dcomplex comp(0,0);

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
       */
   inline void MultElementsComplex(cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      double *b=NULL;

      dcomplex comp(0,0);

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            va = va.MulImag(b[i]) * d;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
       */
   inline void MultElementsComplex(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      dcomplex comp(0,0);

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
//#ifdef HAVE_SSE2
//         m128dc va, vb;
//         m128dd vd(d);
//#ifdef _OPENMP
//#pragma omp parallel for schedule(static) default(shared) private(i, va, vb)
//#endif
//         for (i=0; i < size; i++){
//            va = a[i];
//            vb = b[i];
//            va.MulImag(vd);
//            va *= vb;
//            va.Store(a[i]);
//         }
//#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] = (a[i] * I) * b[i] * d;
         }
//#endif
      }
   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void MultElementsCopy(cVec *C, cVec *A, cVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;
      dcomplex *c=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc vc, va, vb;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i, va, vc, vb)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            vc = va * vb;
            vc.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            c[i] = a[i] * b[i];
         }
#endif
      }

   }

      /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
       */
   inline void MultElementsCopy(cVec *C, cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;
      dcomplex *c=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
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
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *b;


      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va, vb;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++)
         {
            va = a[i];
            vb = b[i];
            va *= vb;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, cVec *B, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *b;


      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va,vb;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            va *= vb * c;
            va.Store(a[i]);
         }

#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] *= b[i] * c;
         }
#endif
      }
   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *b;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *C, cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a __attribute__((aligned(16)));
      dcomplex *c __attribute__((aligned(16)));
      double *b __attribute__((aligned(16)));

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,a,b,c)
#endif
      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va)
#endif
         for (i=0; i < size; i++){
            va = m128dc(a[i]) * b[i];
            va *= d;
            va.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i] * b[i] * d;
         }
#endif
      }
   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
       */
   inline void MultElements(cVec *C, cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *c;
      double *b;


      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,a,b,c)
#endif
      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;

#ifdef HAVE_SSE2
         m128dc va;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va)
#endif
         for (i=0; i < size; i++)
         {
            va = a[i];
            va *= b[i];
            va.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, dVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *b;


      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va, vb, vc(c);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            va *= vb * vc;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] *= b[i] * c;
         }
#endif
      }
   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
    */
   inline void MultElements(dVec *A, double c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      double *a;
      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         lint i;
#ifdef HAVE_SSE2
         __m128d vc = _mm_set1_pd(c);
         if (size % 2 == 0)
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
            for (i=0; i < size; i+=2){
               __m128d va = _mm_load_pd(&(a[i]));
               va = _mm_mul_pd(va, vc);
               _mm_store_pd( &(a[i]), va );
            }
         else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
            for (i=0; i < size; i++)
               a[i] *= c;
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] *= c;
         }
#endif
      }
   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElements(cVec *A, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va, vc(c);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va)
#endif
         for (i=0; i < size; i++)
         {
            va = a[i];
            va *= vc;
            va.Store(a[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] *= c;
         }
#endif
      }
   }
   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsCopy(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va,vb;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++){
            vb = b[i];
            va = vb * d;
            va.Store(a[i]);
         }

#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               a[i] = d * b[i];
            }
         }
#endif
      }

   }

   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, dcomplex c)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc va,vb,vc(c);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,va,vb)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vb = b[i];
            va += vb * vc;
            va.Store(a[i]);
         }

#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            a[i] += c * b[i];

         }
#endif
      }

   }

   /**
    * Multiply + Add vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               a[i] += d * b[i];
            }
         }
      }

   }

   /**
    * Multiply + Add vectors by elements.
    *
    * A += B * C
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, dVec *C)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;
      double *c=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,a,b,c)
#endif
      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               a[i] += b[i] * c[i];
            }
         }
      }

   }


   /**
    * Multiply + Add vectors by elements.
    *
    * A += B * C * d
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsAdd(cVec *A, cVec *B, dVec *C, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a=NULL;
      dcomplex *b=NULL;
      double *c=NULL;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,a,b,c)
#endif
      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               a[i] += b[i] * c[i] * d;
            }
         }
      }

   }



   /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
    */
   inline void MultElementsCopy(cVec *C, cVec *A, dcomplex d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         c = C->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc vc,va,vd(d);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,vc,va)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vc = va * vd;
            vc.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i] * d;
         }
#endif
      }
   }

      /**
    * Multiply vectors by elements.
    *
    * You strongly to encouraged to use this, since all optimizations and
    * parallelisation will be done here.
       */
   inline void MultElementsCopy(cVec *C, cVec *A, dVec *B)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *b;
      dcomplex *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
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
    * parallelisation will be done here.
          */
   inline void MultElementsCopy(cVec *C, cVec *A, dVec *B, double d)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *b;
      dcomplex *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         b = B->begin(s);
         c = C->begin(s);
         lint i;
#ifdef HAVE_SSE2
         m128dc vc, va;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i,vc,va)
#endif
         for (i=0; i < size; i++){
            va = a[i];
            vc = va * b[i] * d;
            vc.Store(c[i]);
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i] * b[i] * d;
         }
#endif
      }
   }


   /**
    *  Return the Imaginary part. (in place)
    */
   inline void Im(cVec *A)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] = a[i].imag();
         }
      }
   }

   /**
    *  Return the Imaginary part.
    */
   inline void Im(cVec *C, cVec *A)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i].imag();
         }
      }
   }


   /**
    *  Return the real part. (in place)
    */
   inline void Re(cVec *A)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            a[i] = a[i].real();
         }
      }
   }

   /**
    *  Return the real part.
    */
   inline void Re(cVec *C, cVec *A)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      dcomplex *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = a[i].real();
         }
      }
   }


   /**
    *  Return the abs values.
    */
   inline void Abs(dVec *C, cVec *A)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      dcomplex *a;
      double *c;

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         a = A->begin(s);
         c = C->begin(s);
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++)
         {
            c[i] = cabs(a[i]);
         }
      }
   }

}

#endif /* VECTORPRIMITIVES_H_ */
