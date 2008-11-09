#ifndef OGRIDSYSTEM_H
#define OGRIDSYSTEM_H

#include "Operator.h"
#include "GridSystem.h"


namespace QDLIB {
   
   /**
    * Abstract class to represent real number operators on a grid.
    */
   class OGridSystem : public Operator, public dVec, public GridSystem
   {
      private:
      public:
   };

} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDSYSTEM_H */
