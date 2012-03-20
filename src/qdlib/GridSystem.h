#ifndef QDLIBGRIDSYSTEM_H
#define QDLIBGRIDSYSTEM_H

#include "math/typedefs.h"

namespace QDLIB
{

   /**
    * Simple container class for the usual grid system parameters.
    * 
    * The fastest varing dimension in the array has the lowest number.
    *	@author Markus Kowalewski
    */
   class GridSystem
   {
      private:
         /* Grid parameters */
         int _ndims;
         int _dims[MAX_DIMS];
         double _xmin[MAX_DIMS];
         double _xmax[MAX_DIMS];
         double _dx[MAX_DIMS];
      public:
         GridSystem();
         GridSystem(int dims) : _ndims(dims) {}

         ~GridSystem();

         int
         Dim() const;

         void
         Dim(int dims);

         int
         DimSize(int dim) const;

         const int*
         DimSizes();

         int
         Size() const;

         void
         DimSize(int dim, int size);

         double
         Xmin(int dim) const;

         void
         Xmin(int dim, double xmin);

         double
         Xmax(int dim) const;

         void
         Xmax(int dim, double xmax);

         double
         Dx(int dim) const;

         void
         operator=(GridSystem &G);

         bool
         operator==(GridSystem &G);

         bool
         operator!=(GridSystem &G);
   };

}

#endif
