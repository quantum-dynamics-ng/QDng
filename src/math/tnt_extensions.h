#ifndef MYTYPES_H
#define MYTYPES_H

#include "math.h"
#include "dcomplex.h"
#include "types.h"

#include "tnt.h"
// 
using namespace TNT;
namespace QDLIB {

  
  
  /** Multiply scalar with matrix. */
  template <class T>
  inline Matrix<T> operator*(const T &A, const Matrix<T>  &B)
  {
    Matrix<T> C(B.num_rows(), B.num_cols());
    
    for (int i=0; i < B.num_rows(); i++){
      for (int j=0; j < B.num_cols(); j++){
	C[i][j] = B[i][j] * A;
      }
    }
    return C;
    
  }
  
  
  template <class T>
  inline Matrix<T> operator*(const double &A, const Matrix<T>  &B)
  {
    Matrix<T> C(B.num_rows(), B.num_cols());
    
    for (int i=0; i < B.num_rows(); i++){
      for (int j=0; j < B.num_cols(); j++){
	C[i][j] = B[i][j] * A;
      }
    }
    return C;
    
  }
  
  typedef Matrix<double> dMat;
  typedef Matrix<dcomplex> cMat;
  
  typedef Vector<double> dVec;
  typedef Vector<dcomplex> cVec;
  
  
  /** Multiply Matrix with Imaginary unit. */
  template <class T>
  inline cMat operator*(const Imaginary im, const Matrix<T>  &B)
  {
    cMat C(B.num_rows(), B.num_cols());
    
    for (int i=0; i < B.num_rows(); i++){
      for (int j=0; j < B.num_cols(); j++){
	C[i][j] = I*B[i][j];
      }
    }
    return C;
    
  }
  
  
  /*
   * General matrix extensions
   */
   
  /**
   * Matrix multiplication of different types.
   * This is meant to be used for multiplications of Matrix<double> *  Matrix<dcomplex>
   */
  template <class T, class C>
  inline Matrix<C> operator*(const Matrix<T> &A, const Matrix<C> &B)
  {
    Matrix<T> tmp(A.num_rows(), B.num_cols());
    C sum;
    
    if (A.num_rows() == B.num_cols()){
      for (int i=0; i < A.num_rows(); i++){
	for (int j=0; i < B.num_cols(); j++){
	  sum = 0;
	  for (int k=0; k < B.num_cols(); k++){
	    sum += A[i][k] * B[k][j];
	  }
	  tmp[i][j] = sum;
	}
      }
    }
      
  }
  
  /** Trace of Matrix. */
  template <class T>
  inline T Trace(const Matrix<T> &A)
  {
    T d;
    if (A.num_rows() != A.num_cols()) return 0.0;
    
    d = 0.0;
    
    for (int i=0; i <  A.num_rows(); i++){
      d += A[i][i];
    }
    return d;
  }
  
  /** Trace of real part Matrix. */
  double TraceReal(const cMat &A)
  {
    double d;
    if (A.num_rows() != A.num_cols()) return 0.0;
    
    d = 0.0;
    
    for (int i=0; i <  A.num_rows(); i++){
      d += A[i][i]._real;
    }
    return d;
  }

} /* namespace QDLIB */

#endif
