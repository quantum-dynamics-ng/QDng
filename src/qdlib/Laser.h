#ifndef LASER_H
#define LASER_H

#include "sys/typedefs.h"


namespace QDLIB {
   
   /**
    * Class which represents a laserfield.
    */
   class Laser : public dVec
      private:
	 double _dt;
         FFT *fft;
	 cVec *_spectrum;
      public:
         void fft();
	 
	 void Dt(double dt);
	 double Dt();
	 
	 
   };

} /* namespace QDLIB */
      
#endif /* #ifndef LASER_H */