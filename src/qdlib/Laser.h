#ifndef LASER_H
#define LASER_H

#include "math/typedefs.h"
#include "tools/ParamContainer.h"
#include "tools/QDClock.h"
#include "fft/fft.h"
#include "tools/FileSingle.h"

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
	 
	 /* Return the parameter container */
	 ParamContainer& Params() { return _params; };
	 
	 string& Name();
	 
         dVec* PowerSpectrum();
	 
	 cVec* Spectrum();
	 
	 void ToTimeDomain();
	 
	 void Clock(QDClock *clock);
	 
	 QDClock* Clock();
	 
	 double Get();
         
         void Set(double value);
	 
	 Laser& operator=(const Laser &laser);
	 
	 double PulseEnergy();
         
         double Dt() {return _dt;}
         
         void Dt(double dt) {_dt = dt; _params.SetValue("dt",dt) ;}
         
         int Nt() {return dVec::size();}
         
         void Nt(int size)  {dVec::newsize(size); _params.SetValue("Nt",size) ;}
   };

} /* namespace QDLIB */
      
#endif /* #ifndef LASER_H */
