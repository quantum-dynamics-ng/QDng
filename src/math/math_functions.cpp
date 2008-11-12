#include "math.h"
#include "typedefs.h"

namespace QDLIB {
   
   /**
    * Returns the minimum value in the vector.
    */
   double VecMin(dVec &v)
   {
      double d;
      d = v[0];
      for (int i=1; i < v.size(); i++){
	 if (v[i] <  d) d = v[i];
      }
      return d;
   }
   
   /**
    * Returns the maximum value in the vector.
    */
   double VecMax(dVec &v)
   {
      double d;
      d = v[0];
      for (int i=1; i < v.size(); i++){
	 if (v[i] >  d) d = v[i];
      }
      return d;
   }
   
   /**
    * Faculty function.
    */
   double Faculty (int arg)
   {
      double r;
      
      if (arg==0) return 1;
      
      r=1.;
      for (int i=1; i <= arg; i++){
         r *= double(i);
      }
	 
      return r;
   }
   
   /**
    * Bessel function of first kind with initial order zero.
    * 
    * \f$ J_n^{\alpha}(x) \text{with} \alpha=0 \f$
    * 
    * \param n order of the bessel function
    * \param arg the x value
    * \param coeffs values of the sum evaluation
    * 
    * \return Number of underflows to zero
    */
   int BesselJ0 (int n, double arg, dVec &coeffs)
   {
      double alpha =0;
      int nz;
      
      coeffs.newsize(n);
      dbesj_(&arg, &alpha, &n, coeffs.begin(), &nz);
      
      return nz;
   }
}
