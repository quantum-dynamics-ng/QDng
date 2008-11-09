#ifndef LASER_H
#define LASER_H

#include "sys/typedefs.h"
#include "sys/ParamContainer.h"
#include "QDClock.h"

namespace QDLIB {
   
   /**
    * Class which represents a laserfield.
    */
   class Laser : public dVec {
      private:
	 string _name;
	 QDClock *_clock;
         FFT *_fft;
	 cVec *_spectrum;
	 bool _remove_clock;
	 ParamContainer _params;
      public:
	 
	 Laser();
	 
	 ~Laser();
	 
	 void Init(ParamContainer &params);
	 
	 string& Name();
	 
         dVec* PowerSpectrum();
	 
	 cVec* Spectrum();
	 
	 void ToTimeDomain();
	 
	 void Clock(QDClock *clock);
	 
	 QDClock* Clock();
   };

} /* namespace QDLIB */
      
#endif /* #ifndef LASER_H */