#ifndef QDLIBGRIDSYSTEM_H
#define QDLIBGRIDSYSTEM_H

/* Defines the maximum number of dimensions in a grid */
#define MAX_DIMS 4 

namespace QDLIB {

   /**
    * Simple container class for the usual grid system parameters.
    * 
    * The fastest varing dimension in the array has the lowest number.
    *	@author Markus Kowalewski
    */
   class GridSystem{
      private:
	 /* Grid parameters */
	 int _ndims;
	 int _dims[MAX_DIMS];
	 double _xmin[MAX_DIMS];
	 double _xmax[MAX_DIMS];
	 double _dx[MAX_DIMS];
      public:
	 GridSystem();
      
	 ~GridSystem();
	 
	 int Dim();
	       
	 void Dim(int dims);
	       
	 int DimSizes(const int dim);
	       
	 int* DimSizes();
	 
	 int Size();
	 
	 void DimSizes(const int dim, const int size);
	       
	 double Xmin(const int dim);
	       
	 void Xmin(const int dim, const double xmin);
	       
	 double Xmax(const int dim);
	       
	 void Xmax(const int dim, const double xmax);
      
	 double Dx (const int dim);
   
	 void operator=(GridSystem &G);
	 
	 bool operator==(GridSystem &G);
	 
	 bool operator!=(GridSystem &G);
   };

}

#endif
