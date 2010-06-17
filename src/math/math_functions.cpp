#include "math.h"
#include "math_functions.h"


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
    * Returns the maximum abs value in the vector.
    */
   double VecMax(cVec & v)
   {
      double d;
      d = cabs(v[0]);
      for (int i=1; i < v.size(); i++){
         if (cabs(v[i]) >  d) d = cabs(v[i]);
      }
      return d;
   }
}



