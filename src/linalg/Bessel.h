#ifndef BESSEL_H
#define BESSEL_H

#include "config.h"
#include "math/typedefs.h"

#define ZBESJ_F77 F77_FUNC(zbesj, ZBESJ)
#define ZBESI_F77 F77_FUNC(zbesi, ZBESI)

namespace QDLIB {


   extern int BesselJ0 (int n, double arg, dVec &coeffs, int &zeroes);
   extern int BesselI0 (int n, double arg, dVec &coeffs, int &zeroes);
      
   /* Slatec functions */
   extern "C" { 
      void ZBESJ_F77(double *re, double *im, double *alpha, int *kode, int *n,
		  double *cRe, double *cIm, int *nz, int *ierr);
      void ZBESI_F77(double *re, double *im, double *alpha, int *kode, int *n,
		  double *cRe, double *cIm, int *nz, int *ierr);
   }

}

#endif
