#ifndef LAPACK_RS
#define LAPACK_RS

#include "dVec.h"
#include "dMat.h"

/**
 * LAPACK Interface class.
 * 
 * Provides a simplified API to selected LAPACK routines
 */
class lapack_rs_d {
  public:
    static int geev(dMatrix &in, dMatrix &eigenvectors, dVector &eigenvalues, dVector &eigenvalues_im);
    static int inverse(dMatrix &in);
};


#endif

