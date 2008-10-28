#ifndef OGRIDSYSTEM_H
#define OGRIDSYSTEM_H

#include "Operator.h"

#define MAX_DIMS 8

namespace QDLIB {
   
   /**
    * Abstract class to represent real number operators on a grid.
    */
   class OGridSystem : public Operator, public dVec
   {
      private:
         int _ndims;
         int _dims[MAX_DIMS];
      public:
         int Dim();
         
         void Dim(int dims);
         
         int* DimSizes();
   };

} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDSYSTEM_H */
