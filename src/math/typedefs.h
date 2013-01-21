#ifndef TYPEDEFS_H
#define TYPEDEFS_H


#include "stdlib.h"

namespace QDLIB {
               
   /**
    * Counter/index integer
    * 
    * This should be used for all indices.
    * Helps us to avoid potential size problems
    * 
    */
   typedef int lint;
}


#include "dcomplex.h"
#include "Vector.h"
#include "VectorView.h"
#include "Matrix.h"

using namespace std;

/* Defines the maximum number of dimensions in a grid */
#define MAX_DIMS 4

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
//       for (s=0; s < strides; s++){
            dcomplex *a;
            for (int i=0; i < cols; i++){
               
            }
         }
      }
   }*/
   
   
}

#include "math/VectorPrimitives.h"


#endif /* TYPEDEFS_H */ 
