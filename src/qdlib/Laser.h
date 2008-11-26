#ifndef LASER_H
#define LASER_H

#include "math/typedefs.h"
#include "sys/ParamContainer.h"
#include "sys/QDClock.h"
#include "fft/fft.h"
#include "FileSingle.h"

namespace QDLIB {
   
   /**
    * Class which represents a laserfield.
    */
   class Laser : public dVec {
      public:
	 typedef FileSingle<Laser> FileLaser;
      private:
	 
	 string _name;
	 QDClock *_clock;
	 double _dt;
         FFT *_fft;
	 cVec *_spectrum;
	 ParamContainer _params;
	 
	 FileLaser _file;
      public:
	 
	 Laser();
	 
	 ~Laser();
	 
	 FileLaser& File();
	 
	 void Init(ParamContainer &params);
	 
	 string& Name();
	 
         dVec* PowerSpectrum();
	 
	 cVec* Spectrum();
	 
	 void ToTimeDomain();
	 
	 void Clock(QDClock *clock);
	 
	 QDClock* Clock();
	 
	 double Get();
	 
	 Laser& operator=(const Laser &laser);
   };

} /* namespace QDLIB */
      
#endif /* #ifndef LASER_H */
