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
         int _lothers[MAX_DIMS];    /* Stripe size of dim */
         int _nothers[MAX_DIMS];    /* Replica points for each dim */
         int _active;               /* activated dimension */
         int _numactive;

         void _BuildInfo();
      public:
         GridSystem();
         GridSystem(int dims) : _ndims(dims) {}

         ~GridSystem();

         int Dim() const;

         void Dim(int dims);

         int DimSize(int dim) const;

         const int* DimSizes();

         int Size() const;

         void DimSize(int dim, int size);

         double Xmin(int dim) const;

         void Xmin(int dim, double xmin);

         double Xmax(int dim) const;

         void Xmax(int dim, double xmax);

         void SetFromArrays(vector<int> N, vector<double> xmin, vector<double> xmax);

         double Dx(int dim) const;

         void operator=(GridSystem &G);

         bool operator==(GridSystem &G) const;

         bool operator!=(GridSystem &G) const;

         /**
          *  Activate a dimension for index mapping.
          */
         void ActiveDim(int dim) { _active = dim;  _numactive = _dims[dim]; }

         /**
          * Number of replica points for one index in activated dimension.
          *
          */
         int NumOthers() const { return _nothers[_active]; }
         int NumOthers(int dim) const { return _nothers[dim]; }
         int NumOthers(int dim1, int dim2) const;

         int LowOthers() const { return _lothers[_active]; }
         int LowOthers(int dim) const { return _lothers[dim]; }
         int LowOthers(int dim1, int dim2) const;

         /**
          * Number of points in active dimension.
          */
         int NumActive() const { return _numactive; }

         /**
          * Get index in array by giving the index i of active dimension and the collapsed replica index.
          */
         int Index(int i, int replica) const
         {
           return (replica/_lothers[_active]) * _numactive * _lothers[_active] + replica%_lothers[_active]  + i * _lothers[_active];
         }

         /**
          * Return base index for given replica point.
          *
          * The index of the point is then given by: IndexBase + i * LowOthers()
          */
         int IndexBase(int replica) const
         {
            return (replica/_lothers[_active]) * _numactive * _lothers[_active] + replica%_lothers[_active];
         }

   };

}

#endif
