
#include <iostream>
#include "GridSystem.h"

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
   int GridSystem::Dim()
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
      _ndims = dims;
   }
         
   
   /**
    * Sizes of dimensions.
    * 
    * \param dim Number of dimension (take care of MAX_DIMS!)
    * \return Number of points in this direction
    */
   int GridSystem::DimSizes(const int dim)
   {
      return _dims[dim];
   }
   
   /**
    * Sizes of dimensions.
    * 
    * \return pointer to array with sizes.
    */
   int* GridSystem::DimSizes( )
   {
      return _dims;
   }
   
   /**
    * Set the size of a dimension/coordinate.
    * 
    * \param dim Number of dimension
    * \param size Number of points in this direction
    */
   void GridSystem::DimSizes(const int dim, const int size)
   {
      _dims[dim] = size;
   }
   
   /**
    * Absolute number of grid points.
    */
   int GridSystem::Size() const
   {
      int size=1;
      
      for (int i=0; i < _ndims; i++)
	 size *= _dims[i];
      
      return size;
   }
   
   /**
    * Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   double GridSystem::Xmin(const int dim)
   {
      return _xmin[dim];
   }
   
   /**
    * Set Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   void GridSystem::Xmin(const int dim, const double xmin)
   {
      _xmin[dim] = xmin;
      _dx[dim] = (_xmax[dim] - _xmin[dim])/(double(_dims[dim]-1)); /* Update dx value */
   }
   
   /**
    * Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   double GridSystem::Xmax(const int dim)
   {
      return _xmax[dim];
   }
   
   /**
    * Set Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   void GridSystem::Xmax(const int dim, const double xmax)
   {
      _xmax[dim] = xmax;
      _dx[dim] = (xmax - _xmin[dim])/(double(_dims[dim]-1));    /* Update dx value */
   }

   /**
    * Distance between grid points.
    * \param dim Number of dimension
    * \return dx value in this coordinate
    */
   double GridSystem::Dx (const int dim)
   {
      return _dx[dim];
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
   }

   /**
    * Comparison of grid parameters.
    */
   bool GridSystem::operator ==( GridSystem & G )
   {
      bool equal = true;
      
      for (int i=0; i < MAX_DIMS; i++)
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
   bool GridSystem::operator !=( GridSystem & G )
   {
     if ( *this == G ) return false;
      else return true;
   }
   
}


