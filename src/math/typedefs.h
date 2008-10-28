#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#include "dcomplex.h"
#include "tnt/tnt.h"


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

   /** TNT Vector of doubles */
   typedef TNT::Vector<double> dVec;
   
   /** TNT Vector of dcomplex */
   typedef TNT::Vector<dcomplex> cVec;

   /** TNT Matrix of doubles */
   typedef TNT::Matrix<double> dMat;
   
   /** TNT Matrix of dcomplex */
   typedef TNT::Matrix<dcomplex> cMat;

   

}



#endif /* TYPEDEFS_H */ 
