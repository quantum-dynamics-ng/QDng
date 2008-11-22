#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/**
 * Counter/index integer
 * 
 * This should be used for all indices.
 * Helps us to avoid potential size problems
 * 
 */
namespace QDLIB {
   typedef  int lint;
}

#include "Vector.h"
#include "dcomplex.h"
#include "tnt/tnt.h"
#include "VectorView.h"


namespace TNT {
   /**
    * Enable multiplication of type double with arbitrary Types of Vector.
    */
   template <class T>
   Vector<T> operator*(const double &a, const Vector<T> &A)
   {
      Subscript N = A.dim();
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
       Subscript N = A.dim();
       assert(N == B.dim());
   
       Subscript i;
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
      Subscript N = B.dim();
      Vector<dcomplex> r(N);
   
      for (int i=0; i<N; i++){
	 r[i] = 0;
	 for (int j=0; j<N; j++){
            r[i] += A[i][j] * B[j];
	 }
      }
   
      return r;
   }
}



namespace QDLIB {

   /** Variant of TNT Vector of doubles */
   typedef QDLIB::Vector<double> dVec;
   
   /** Variant of TNT Vector of dcomplex */
   typedef QDLIB::Vector<dcomplex> cVec;

   /** TNT Matrix of doubles */
   typedef TNT::Matrix<double> dMat;
   
   /** TNT Matrix of dcomplex */
   typedef TNT::Matrix<dcomplex> cMat;

   /** Multidimensional view on dVec */
   typedef QDLIB::VectorView<double> dVecView;
   
   /** Multidimensional view on dVec */
   typedef QDLIB::VectorView<dcomplex> cVecView;
   
     
   extern double VecMin(dVec &v);
   extern double VecMax(dVec &v);
   
   extern double Faculty (int arg);
   
   extern "C" { int dbesj_(double *x, double *alpha, int *n, double *coeffs, int *nz); }
   extern "C" { void zbesj_(double *re, double *im, double *alpha, int *kode, int *n,
      double *cRe, double *cIm, int *nz, int *ierr); }
   
   extern int BesselJ0 (int n, double arg, dVec &coeffs, int &zeroes);
   
   /**
    * Multiply vectors by elements.
    * 
    * You strongly to encouraged to use this, since all optimizations and
    * parallelistation will be done here.
    */
   inline void MultElements(cVec *A, cVec *B)
   {
      int size = A->size();
      
      for (int i=0; i < size; i++)
      {
	(*A)[i] *= (*B)[i];
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
      int size = A->size();
      
      for (int i=0; i < size; i++)
      {
	 (*A)[i] *= (*B)[i] * c;
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
      int size = A->size();
      
      for (int i=0; i < size; i++)
      {
	 (*A)[i] *= (*B)[i] * c;
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
	 for (lint i=0; i < size; i++)
	 {
	    a[i] *= c;
	 }
      }
   }
   
}



#endif /* TYPEDEFS_H */ 
