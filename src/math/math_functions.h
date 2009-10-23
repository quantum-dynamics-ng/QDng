#include "typedefs.h"

namespace QDLIB {
   
   extern double VecMin(dVec &v);
   extern double VecMax(dVec &v);
   
   extern double Faculty (int arg);
   
   //extern "C" { int dbesj_(double *x, double *alpha, int *n, double *coeffs, int *nz); }
      
   
   /**
    * Diagonalize a symmetric 2x2 matrix
    * 
    * \param m1 Matrix element 11
    * \param m2 Matrix element 22
    * \param m1 Matrix element 12 = 21
    * \param eval Eigenvalues
    */
   inline void diag22symm(double m1, double m2, double m12, double *eval)
   {
      /* Eigenvalues */
      eval[0] = 0.5 * ( m1 + m2 + sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
      eval[1] = 0.5 * ( m1 + m2 - sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
   }
   
   /**
    * Diagonalize a symmetric 2x2 matrix
    * 
    * \param m1 Matrix element 11
    * \param m2 Matrix element 22
    * \param m1 Matrix element 12 = 21
    * \param eval Eigenvalues
    * \param evec Eigenvectors
    */
   inline void diag22symm(double m1, double m2, double m12, double *eval, double **evec)
   {
      diag22symm(m1, m2, m12, eval);

      /* Eigenvectors */
      evec[0][0] = m12 / 0.5 / ( m2 - m1 + sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
      evec[1][0] = m12 / 0.5 / ( m2 - m1 - sqrt(m1*m1 + m2*m2 - 2*m1*m2 + 4*m12*m12) );
      
      /* Normalize eigenvectors */
      double norm;
      norm=evec[0][0]*evec[0][0] + 1;
      evec[0][0] /= sqrt(norm);
      evec[0][1] = 1/ sqrt(norm);
      
      norm=evec[1][0]*evec[1][0] + 1;
      evec[1][0] /= sqrt(norm);
      evec[1][1] = 1/ sqrt(norm);
   }
}
