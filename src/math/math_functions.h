#include "typedefs.h"

namespace QDLIB {
   
   extern double VecMin(dVec &v);
   extern double VecMax(dVec &v);
   
   extern double Faculty (int arg);
   
   extern "C" { int dbesj_(double *x, double *alpha, int *n, double *coeffs, int *nz); }
   extern "C" { void zbesj_(double *re, double *im, double *alpha, int *kode, int *n,
      double *cRe, double *cIm, int *nz, int *ierr); }
   
   extern int BesselJ0 (int n, double arg, dVec &coeffs, int &zeroes);
   
   
   /**
    * Diagonalize a symmrtric 2x2 matrix
    * 
    * \param m1 Matrix element 11
    * \param m2 Matrix element 22
    * \param m1 Matrix element 12 = 21
    * \param ev1 Eigenvalue 1
    * \param ev2 Eigenvalue 2
    */
   inline void diag22symm(double m1, double m2, double m12, double &ev1, double &ev2)
   {
      ev1 = 0.5 * ( m1 + m2 + sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
      ev2 = 0.5 * ( m1 + m2 - sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
   }
   
}