#include "Laser.h"
#include "sys/Exception.h"

namespace QDLIB {
   
   Laser::Laser() : _name("Laser"), _clock(NULL), _fft(NULL), _spectrum(NULL), _remove_clock(false)
   {}

   Laser::~ Laser()
   {
      if (_fft != NULL) delete _fft;
      if (_spectrum != NULL) delete _spectrum;
      if (_remove_clock) delete _clock;
   }

   /**
    * Initialize the laser.
    *
    * Import parameters are:
    * 
    * - dt
    * - Nt 
    * 
    * If no clock is set then Laser will create it's own clock which will
    * also be removed in the Destructor.
    */
   void Laser::Init(ParamContainer & params)
   {
      _params = params;
      
      double dt;
      int Nt;
      _params.GetValue("dt", dt);
      _params.GetValue("Nt", Nt);
      
      if (_clock != NULL && ( dt != _clock->Dt() || Nt < _clock->Steps() ) )
	 throw ( EParamProblem("Parameter values are different for laser and own clock!") );
      if (_clock == NULL) {
	 _clock = new QDClock(Nt, dt);
	 _remove_clock = true;
      }
   }

   /**
    * Say my name.
    */
   string & Laser::Name()
   {
      return _name;
   }
   

   /**
    * Calculate the power spectrum of the laser.
    * \return The power spectrum (the pointer will not be maintained by laser anymore)
    */
   dVec * Laser::PowerSpectrum()
   {
      if (_spectrum == NULL) _spectrum = new cVec(_clock->Steps());
      if (_fft == NULL) _fft = new FFT(_clock->Steps(), dVec::begin(), _spectrum->begin());
      
      _fft->forward();
      
      dVec *power = new dVec(_clock->Steps());
      for (int i=0; i < dVec::size(); i++){
	 (*power)[i] = cabs((*_spectrum)[i]);
      }
      return power;
   }

   /**
    * Calculate the spectrum.
    * 
    * You can can act one the spectrum and use ToTimeDomain to make changes active.
    */
   cVec * Laser::Spectrum()
   {
      if (_spectrum == NULL) _spectrum = new cVec(_clock->Steps());
      if (_fft == NULL) _fft = new FFT(_clock->Steps(), dVec::begin(), _spectrum->begin());
      
      _fft->forward();
      
      return _spectrum;
   }

   /**
    * Transform spectral representation back to time domain.
    */
   void Laser::ToTimeDomain()
   {
      if (_spectrum == NULL && _fft != NULL)
	 _fft->backward();
   }

   /**
    * Set the clock of laser.
    */
   void Laser::Clock(QDClock * clock)
   {
      _clock = clock;
   }

   /**
    * Ask for the clock of the laser.
    */
   QDClock * Laser::Clock()
   {
      return _clock;
   }

   
   
}

