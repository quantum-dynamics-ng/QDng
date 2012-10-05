
#include <iostream>
#include "GridSystem.h"
#include "tools/Exception.h"

#define GRID_EPS 1e-10

#include <math.h>

namespace QDLIB {

   GridSystem::GridSystem() :  _ndims(0)
   {
      for (int i=0; i < MAX_DIMS; i++){
	 _dims[i] = 0;
	 _xmin[i] = 0;
	 _xmax[i] = 0;
	 _dx[i] = 0;
      }
   }


   GridSystem::~GridSystem()
   {
   }
   
   /**
    * Number of dimensions.
    */
   int GridSystem::Dim() const
   {
      return _ndims;
   }
         
   /**
    * Number of dimensions.
    * 
    * Must be set by implementing class.
    */
   void GridSystem::Dim(int dims)
   {
      if (dims > MAX_DIMS)
         throw(EParamProblem("GridSystem: Number of Dimensions exceeds MAX_DIMS"));

      if (dims < 0)
         throw(EParamProblem("GridSystem: Number of Dimensions is negative"));

      _ndims = dims;
   }
         
   
   /**
    * Sizes of dimensions.
    * 
    * \param dim Number of dimension (take care of MAX_DIMS!)
    * \return Number of points in this direction
    */
   int GridSystem::DimSize(int dim) const
   {
      return _dims[dim];
   }
   
   /**
    * Sizes of dimensions.
    * 
    * \return pointer to array with sizes.
    */
   const int* GridSystem::DimSizes( )
   {
      return _dims;
   }
   
   /**
    * Set the size of a dimension/coordinate.
    * 
    * \param dim Number of dimension
    * \param size Number of points in this direction
    */
   void GridSystem::DimSize(int dim, int size)
   {
      _dims[dim] = size;
   }
   
   /**
    * Absolute number of grid points.
    */
   int GridSystem::Size() const
   {
      int size=1;

      if (_ndims == 0) return 0;

      for (int i=0; i < _ndims; i++)
	 size *= _dims[i];

      return size;
   }

   /**
    * Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   double GridSystem::Xmin(int dim) const
   {
      return _xmin[dim];
   }
   
   /**
    * Set Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   void GridSystem::Xmin(int dim, double xmin)
   {
      _xmin[dim] = xmin;
      _dx[dim] = (_xmax[dim] - _xmin[dim])/(double(_dims[dim]-1)); /* Update dx value */
   }
   
   /**
    * Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   double GridSystem::Xmax(int dim) const
   {
      return _xmax[dim];
   }
   
   /**
    * Set Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   void GridSystem::Xmax(int dim, double xmax)
   {
      _xmax[dim] = xmax;
      _dx[dim] = (xmax - _xmin[dim])/(double(_dims[dim]-1));    /* Update dx value */
   }

   /**
    * Distance between grid points.
    * \param dim Number of dimension
    * \return dx value in this coordinate
    */
   double GridSystem::Dx (int dim) const
   {
      return _dx[dim];
   }
   
   /**
    * Initialize all grid parameters from arrays.
    */
   void GridSystem::SetFromArrays(vector<int> N, vector<double> xmin, vector<double> xmax)
   {
      if (N.size() != xmin.size() || xmin.size() != xmax.size())
         throw (EParamProblem("GridSystem: All Arrays must have the same size"));

      Dim(N.size());

      for (unsigned int i=0; i < N.size(); i++){
         DimSize(i, N[i]);
         Xmin(i, xmin[i]);
         Xmax(i, xmax[i]);
      }
   }

   /**
    * Copy the parameter set.
    */
   void GridSystem::operator =(GridSystem &G)
   {
      _ndims = G._ndims;
      
      for (int i=0; i < MAX_DIMS; i++)
      {
       _dims[i] = G._dims[i];
       _xmin[i] = G._xmin[i];
       _xmax[i] = G._xmax[i];
       _dx[i] = G._dx[i];
      }

      _BuildInfo();
   }

   /**
    * Comparison of grid parameters.
    */
   bool GridSystem::operator ==( GridSystem & G ) const
   {
      bool equal = true;
      
      if ( _ndims != G._ndims ) return false;

      for (int i=0; i < _ndims; i++)
      {
         if ( _dims[i] != G._dims[i] ) equal = false;
         if ( fabs(_xmin[i] - G._xmin[i]) > GRID_EPS) equal = false;
         if ( fabs(_xmax[i] - G._xmax[i]) > GRID_EPS) equal = false;
      }
      return equal;
   }

   /**
    * Comparison of grid parameters.
    */
   bool GridSystem::operator !=( GridSystem & G ) const
   {
     if ( *this == G ) return false;
      else return true;
   }
   
   /**
    * Create Stripe informations.
    */
   void GridSystem::_BuildInfo()
   {
      for (int dim=0; dim < _ndims; dim++){
         _lothers[dim] = 1;
         _nothers[dim] = 1;

         /* Determine how many values in the lower dims are  present */
         if (dim>0)
         {
            for(int i=0; i < dim; i++)
               _lothers[dim] *= _dims[i];
         }

         /* Determine how many times one x_i value is present */
         for(int i=0; i < _ndims; i++)
         {
            if (i != dim) _nothers[dim] *= _dims[i];
         }
      }
   }

   /**
    * Number of points in dims other than dim1 and dim2.
    *
    **/
   int GridSystem::NumOthers(int dim1, int dim2) const
   {
      int nothers = 1;

      for(int i=0; i < _ndims; i++)
         if (i != dim1 && i != dim2) nothers *= _dims[i];

      return nothers;
   }

   int GridSystem::LowOthers(int dim1, int dim2) const
   {
      int lothers = Size();

      for(int i=0; i < _ndims; i++){
         int ltmp = 1;
         if (i != dim1 || i != dim2) {
            for(int j=0; j < i ; j++)
               ltmp *= _dims[j];
         }
         if (ltmp < lothers) lothers = ltmp;
      }

      return lothers;
   }
}


