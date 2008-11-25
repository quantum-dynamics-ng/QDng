#ifndef QDLIBKSPACE_H
#define QDLIBKSPACE_H

#include "math/typedefs.h"

namespace QDLIB {
   
   /**
    * Gerneral K-space tools to setup a k-space representation.
    */
   class Kspace
   {
      public:
	 /** Return the delta k of the grid. */
	 static inline double Dk(const double length)
	 {
	    return (2*M_PI) / length;
	 }
	 
	 static dVec* Init1Dd2dx2(const double mass, const double length, const int Nx);
	 static dVec* Init1Dddx(const double length, const int Nx);
   };
}

#endif
