#ifndef OGRIDDIPOLE_H
#define OGRIDDIPOLE_H

#include "OGridSystem.h"
#include "Laser.h"

namespace QDLIB {
   
   /**
    * Class which represents a potential on the grid.
    * 
    * Parameters:
    *    - dipole_file [optional]
    *    - laser_file [optional]
    *    - 
    */
   class OGridDipole : public OGridSystem
      private:

      public:
         void Laser(Laser &laser);
	 Laser& Laser();
         
   };

} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDDIPOLE_H */
