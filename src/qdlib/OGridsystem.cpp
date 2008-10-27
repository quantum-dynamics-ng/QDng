#include "OGridSystem.h"

namespace QDLIB {
   
   /**
    * Number of dimensions.
    */
   int OGridSystem::Dim()
   {
      return _ndims;
   }
   
   /**
    * Set the number of dimensions
    */
   void OGridSystem::Dim(int dims)
   {
      _ndims = dims;
   }
   
   /**
    * Pointer to array of dimension sizes.
    */
   int* OGridSystem::DimSizes()
   {
       return _dims;
   }
   
   
} /* namespace QDLIB */
