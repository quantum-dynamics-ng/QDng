
#include <iostream>
#include "GridSystem.h"
#include "tools/Exception.h"

#define GRID_EPS 1e-10

#include <math.h>

namespace QDLIB {

   void GridSystemHeaderPm::InitFromParamContainer(ParamContainer& pm)
   {
      Clear();

      /* Get all the params from the ParamContainer */
      int dims;
      pm.GetValue( "dims", dims);

      set_dims(dims);
      if ( dims <= 0 ) throw ( EParamProblem("Zero number of dimension defined") );

      int i = 0;
      int n = 0;
      double d = 0;
      char c[256];
      sprintf(c, "%d", i);
      string s = string("N") + string(c);

      while (pm.isPresent(s) && i < dims) {
         dim_description_t* dimd = add_dim();

         pm.GetValue(string("N") + string(c), n);

         if (n <= 0)
            throw(EParamProblem("Zero elements grid defined"));

         dimd->set_size(n);

         pm.GetValue(string("xmin") + string(c), d);
         dimd->set_xmin(d);

         pm.GetValue(string("xmax") + string(c), d);
         dimd->set_xmax(d);

         if ((dimd->xmax() - dimd->xmin()) <= 0)
            throw(EParamProblem("Zero length grid defined"));

         i++;
         sprintf(c, "%d", i);
      }
   }

   GridSystem::GridSystem()
   {
      grid_sys.set_dims(0);
      for (int i=0; i < MAX_DIMS; i++){
	 _dx[i] = 0;
      }
   }

   GridSystem::GridSystem(int dims)
   {
      for (int i=0; i < dims; i++)
         grid_sys.add_dim();

      grid_sys.set_dims(dims);
   }
   
   /**
    * Number of dimensions.
    */
   int GridSystem::Dim() const
   {
      return grid_sys.dims();
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

      grid_sys.mutable_dim()->Clear();
      for (int i=0; i < dims; i++)
         grid_sys.add_dim();

      grid_sys.set_dims(dims);
   }
         
   const int* GridSystem::DimSizes()
   {
      for (size_t i=0; i < grid_sys.dims(); i++)
       dimsizes_[i] = grid_sys.dim(i).size();

      return dimsizes_;
   }
   
   /**
    * Sizes of dimensions.
    * 
    * \param dim Number of dimension (take care of MAX_DIMS!)
    * \return Number of points in this direction
    */
   int GridSystem::DimSize(int dim) const
   {
      return grid_sys.dim(dim).size();
   }
   
   /**
    * Set the size of a dimension/coordinate.
    * 
    * \param dim Number of dimension
    * \param size Number of points in this direction
    */
   void GridSystem::DimSize(int dim, uint32_t size)
   {
      grid_sys.mutable_dim(dim)->set_size(size);
   }
   
   /**
    * Absolute number of grid points.
    */
   int GridSystem::Size() const
   {
      if (grid_sys.dims() == 0) return 0;

      int size=1;
      for (size_t i=0; i < grid_sys.dims(); i++)
	 size *= grid_sys.dim(i).size();

      return size;
   }

   /**
    * Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   double GridSystem::Xmin(int dim) const
   {
      return grid_sys.dim(dim).xmin();
   }
   
   /**
    * Set Xmin value.
    * \param dim Number of dimension
    * \return xmin value in this coordinate
    */
   void GridSystem::Xmin(int dim, double xmin)
   {
      grid_sys.mutable_dim(dim)->set_xmin(xmin);
      _dx[dim] = (grid_sys.dim(dim).xmax() - xmin)/(double(grid_sys.dim(dim).size()-1)); /* Update dx value */
   }
   
   /**
    * Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   double GridSystem::Xmax(int dim) const
   {
      return grid_sys.dim(dim).xmax();
   }
   
   /**
    * Set Xmax value.
    * \param dim Number of dimension
    * \return xmax value in this coordinate
    */
   void GridSystem::Xmax(int dim, double xmax)
   {
      grid_sys.mutable_dim(dim)->set_xmax(xmax);
      _dx[dim] = (xmax - grid_sys.dim(dim).xmin())/(double(grid_sys.dim(dim).size()-1));    /* Update dx value */
   }

   /**
    * Distance between grid points.
    * \param dim Number of dimension
    * \return dx value in this coordinate
    */
   double GridSystem::Dx (int dim) const
   {
      return ( grid_sys.dim(dim).xmax() - grid_sys.dim(dim).xmin())/(double(grid_sys.dim(dim).size()-1));
      //return _dx[dim];
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
      grid_sys = G.grid_sys;
      
      for (int i=0; i < MAX_DIMS; i++)
      {
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
      
      if ( grid_sys.dims() != G.grid_sys.dims() ) return false;

      for (size_t i=0; i < grid_sys.dims(); i++)
      {
         if ( grid_sys.dim(i).size() != G.grid_sys.dim(i).size() ) equal = false;
         if ( fabs(grid_sys.dim(i).xmin() - G.grid_sys.dim(i).xmin()) > GRID_EPS) equal = false;
         if ( fabs(grid_sys.dim(i).xmax() - G.grid_sys.dim(i).xmax()) > GRID_EPS) equal = false;
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
      for (size_t dim=0; dim < grid_sys.dims(); dim++){
         _lothers[dim] = 1;
         _nothers[dim] = 1;

         /* Determine how many values in the lower dims are  present */
         if (dim>0)
         {
            for(size_t i=0; i < dim; i++)
               _lothers[dim] *= grid_sys.dim(i).size();
         }

         /* Determine how many times one x_i value is present */
         for(size_t i=0; i < grid_sys.dims(); i++)
         {
            if (i != dim) _nothers[dim] *= grid_sys.dim(i).size();
         }
      }
   }

   /**
    * Number of points in dims other than dim1 and dim2.
    *
    **/
   int GridSystem::NumOthers(size_t dim1, size_t dim2) const
   {
      int nothers = 1;

      for(size_t i=0; i < grid_sys.dims(); i++)
         if (i != dim1 && i != dim2) nothers *= grid_sys.dim(i).size();

      return nothers;
   }

   int GridSystem::LowOthers(size_t dim1, size_t dim2) const
   {
      int lothers = Size();

      for(size_t i=0; i < grid_sys.dims(); i++){
         int ltmp = 1;
         if (i != dim1 || i != dim2) {
            for(size_t j=0; j < i ; j++)
               ltmp *= grid_sys.dim(j).size();
         }
         if (ltmp < lothers) lothers = ltmp;
      }

      return lothers;
   }
}


