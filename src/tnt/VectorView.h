#include "tnt_vector.h"

namespace TNT 
{
   /**
    * Multidimensional view on a vector.
    * 
    * This class is used to represent a vector as multidimensional Grid.
    * A projection on all points of one dimension is made to act on
    * a single coordinate in the array.
    * 
    * The order in the dimension numbering is x,y,z,...
    * that the fastest varing index has the lowest number.
    */
   template <class T>
   class VectorView
   {
      private:
	 T *_v;         /* Pointer to vec data */
	 T ****_view;   /* Array for pointer to projected data */
	 int _ndims;    /* Number of dimensions */
	 int *_dims;    /* Array with number of points in each dim */
	 int _adim;     /* Active dimensions */
	 int _nothers;  /* number of points for each x_i value in active dim */
      public:
	 VectorView(Vector<T> &vec, int ndims, int *dims);
	 ~VectorView();
	 
	 void ActiveDim(int dim);
	 
	 Vector<T>* Sum();
	 
	 void operator=(const Vector<T> &vec);
	 void operator*=(const Vector<T> &vec);
	 void operator+=(const Vector<T> &vec);
	 
   };
   
   /**
    * Initialize the view with all information about the vector.
    * 
    * \param vec The vector to look at
    * \param ndims number of dimensions
    * \param dims  array with number of points in each direction
    */
   template <class T>
	 VectorView<T>::VectorView(Vector<T> &vec, int ndims, int *dims) : _adim(0)
   {
      _v = vec.begin();
      _ndims = ndims;
      _dims = dims;
      
      _view = new T***[ndims];
      for(int i=0; i < ndims; i++){
	 _view[i] = NULL;
      }
   }
   
   template <class T>
	 VectorView<T>::~VectorView()
   {
      for(int i=0; i < _ndims; i++){
	 if (_view[i] != NULL) {
	    for (int j=0; j < _dims[i]; j++){ /* Loop over points in dim */
	       delete _view[i][j];
	    }
	    delete _view[i];
	 }
      }
      delete _view;
      
   }
   
   /**
    * Activate the dimension to work on.
    * 
    * This needs to be done bevor using the other methods of the class.
    */
   template <class T>
	 void VectorView<T>::ActiveDim(int dim)
   {
      
      _adim = dim;
      
      if (_view[dim] == NULL){
	 _nothers = 1;
	 int lothers = 1;
	 
	 /* Determine how many values in the lower dims are  present */
	 if (dim>0) 
	 {
	    for(int i=0; i < dim; i++)
	       lothers *= _dims[i];
	 }
	 
	 /* Determine how many times one x_i value is present */
	 for(int i=0; i < _ndims; i++)
	 {
	    if (i != dim) _nothers *= _dims[i];
	 }
	 
	 /* Reserve space for active dimension*/
	 _view[dim] = new T**[_dims[dim]];
	 /* Set the pointer array */
	 int me;
	 me = _dims[dim];
	 for (int i=0; i < _dims[dim]; i++){ /* Loop over points in dim */
	    _view[dim][i] = new T*[_nothers];
	    for (int j=0; j < _nothers; j++) /* Loop over other dim */
	    {
	       _view[dim][i][j] = _v+( (j/lothers) * me * lothers + j%lothers  + i * lothers);
	    }
	 }
      }
   }
   
   /**
    * Sum over the active dimension.
    * 
    * \return A vector which has one dim less.
    * \todo Test method.
    */
   template <class T>
   inline Vector<T>* VectorView<T>::Sum()
   {
      Vector<T> *r;
      r = new Vector<T>(_nothers);
      *r = 0;
      
      for (int i=0; i < _dims[_adim]; i++){ /* Loop over points in dim */
	 for (int j=0; j < _nothers; j++) /* Loop over other dim */
	 {
	    (*r)[j] += *(_view[_adim][i][j]);
	 }
      }
   }
   
   /**
    * Copy a vector to the active dimension.
    */
   template <class T>
	 inline void VectorView<T>::operator=(const Vector<T> &vec)
   {
      for (int i=0; i < _dims[_adim]; i++){ /* Loop over points in dim */
	 for (int j=0; j < _nothers; j++) /* Loop over other dim */
	 {
	    *(_view[_adim][i][j]) = vec[i];
	 }
      }
   }

   
   /**
    * Multiply a vector with the active dimension.
    */
   template <class T>
	 inline void VectorView<T>::operator*=(const Vector<T> &vec)
   {
      for (int i=0; i < _dims[_adim]; i++){ /* Loop over points in dim */
	 for (int j=0; j < _nothers; j++) /* Loop over other dim */
	 {
	    *(_view[_adim][i][j]) *= vec[i];
	 }
      }
   }
   
   /**
    * Add a vector to the active dimension.
    */
   template <class T>
	 inline void VectorView<T>::operator+=(const Vector<T> &vec)
   {
      for (int i=0; i < _dims[_adim]; i++){ /* Loop over points in dim */
	 for (int j=0; j < _nothers; j++) /* Loop over other dim */
	 {
	    *(_view[_adim][i][j]) += vec[i];
	 }
      }
   }
   
}
