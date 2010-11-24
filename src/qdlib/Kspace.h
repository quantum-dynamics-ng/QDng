#ifndef QDLIBKSPACE_H
#define QDLIBKSPACE_H

#include "math/typedefs.h"
#include "GridSystem.h"

namespace QDLIB {
   
   /**
    * Gerneral K-space tools to setup a k-space representation.
    */
   class Kspace
   {
      public:
	 /** Return the delta k of the grid. */
         static inline double Dk(const GridSystem &Grid, const int dim)
	 {
            return (2*M_PI) / ( Grid.Xmax(dim) - Grid.Xmin(dim) + Grid.Dx(dim) );
	 }
	 
         static dVec* Init1Dd2dx2(const double mass, const GridSystem &Grid, const int dim);
         static dVec* Init1Dddx(const GridSystem &Grid, const int dim, const double factor = 1);
   };
}

#endif
