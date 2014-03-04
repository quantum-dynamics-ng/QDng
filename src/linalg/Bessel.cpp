#include "Bessel.h"


namespace QDLIB {
   
   /**
    * Bessel function of first kind with initial order zero.
    * 
    * \f$ J_n^{\alpha}(x) with \alpha=0 \f$
    * 
    * \param n order of the bessel function
    * \param arg the x value
    * \param coeffs values of the sum evaluation
    * \param zeroes Number of coeficients set to zero due to underun
    * 
    * \return Slatec error code. Should be zero.
    */
   int BesselJ0 (int n, double arg, dVec &coeffs, int &zeroes)
   {
      double alpha =0;
      double argIm =0;
      dVec bIm;
      int kode=1;    /* don't scale the values */
      int ierr;
	 
      if (n==0) return -1;
	 
      coeffs.newsize(n);
      bIm.newsize(n);
   //       (ZR, ZI, FNU, KODE, N, CYR, CYI, NZ, IERR)
      ZBESJ_F77(&arg, &argIm, &alpha, &kode, &n, (double*) coeffs.begin(0), (double*) bIm.begin(0), &zeroes, &ierr);
	 
	 //if (nz > 0) cout << "\n\n*** Bessel underflow warning : " << nz << "\n";
      switch (ierr) {
	 case 0: break;
	 case 1:
	    cout << "\n\n*** Bessel error : Input error\n";
	    break;
	 case 2:
	    cout << "\n\n*** Bessel error : Overflow\n";
	    break;
	 case 3:
	    cout << "\n\n*** Bessel warning : Precision warning\n";
	    break;
	 case 4:
	    cout << "\n\n*** Bessel warning : Precision error\n";
	    break;
	 case 5:
	    cout << "\n\n*** Bessel warning : Algorithmic error\n";
	    break;
	       
      }
      return ierr;
   }
      
   
   int BesselI0( int n, double arg, dVec & coeffs, int & zeroes )
   {
      double alpha =0;
      double argIm =0;
      dVec bIm;
      int kode=2;    /* return values scaled by exp(-arg) */
      int ierr;
	 
      if (n==0) return -1;
	 
      coeffs.newsize(n);
      bIm.newsize(n);
   //       (ZR, ZI, FNU, KODE, N, CYR, CYI, NZ, IERR)
      ZBESI_F77(&arg, &argIm, &alpha, &kode, &n, (double*) coeffs.begin(0), (double*) bIm.begin(0), &zeroes, &ierr);
	 
	 //if (nz > 0) cout << "\n\n*** Bessel underflow warning : " << nz << "\n";
      switch (ierr) {
	 case 0: break;
	 case 1:
	    cout << "\n\n*** Bessel error : Input error\n";
	    break;
	 case 2:
	    cout << "\n\n*** Bessel error : Overflow\n";
	    break;
	 case 3:
	    cout << "\n\n*** Bessel warning : Precision warning\n";
	    break;
	 case 4:
	    cout << "\n\n*** Bessel warning : Precision error\n";
	    break;
	 case 5:
	    cout << "\n\n*** Bessel warning : Algorithmic error\n";
	    break;
	       
      }
      return ierr;
   }

} /* namespace QDLIB */
