#ifndef BESSEL_H
#define BESSEL_H

#include "math/typedefs.h"

namespace QDLIB {


   extern int BesselJ0 (int n, double arg, dVec &coeffs, int &zeroes);
   extern int BesselI0 (int n, double arg, dVec &coeffs, int &zeroes);
      
   /* Slatec functions */
   extern "C" { 
      void zbesj_(double *re, double *im, double *alpha, int *kode, int *n,
		  double *cRe, double *cIm, int *nz, int *ierr);
      void zbesi_(double *re, double *im, double *alpha, int *kode, int *n,
		  double *cRe, double *cIm, int *nz, int *ierr);
   }

}

#endif
