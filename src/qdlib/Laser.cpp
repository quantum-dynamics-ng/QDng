#include "Laser.h"
#include "tools/Exception.h"
#include "tools/ZCopyStream.h"

namespace QDLIB {
   
   Laser::Laser() : _name("Laser"), _clock(NULL), _scale(1), _fft(NULL), _spectrum(NULL)
   {}

   Laser::~ Laser()
   {
      if (_fft != NULL) delete _fft;
      if (_spectrum != NULL) delete _spectrum;
      
   }

   Laser::FileLaser& Laser::File( )
   {
      return _file;
   }

   
   /**
    * Initialize the laser.
    *
    * Import parameters are:
    * 
    * \li dt
    * \li Nt 
    */
   void Laser::Init(ParamContainer &params)
   {
      _params = params;
      
      int Nt;
      
      if (!_params.isPresent("dt"))
	 throw ( EParamProblem("The laser is missing dt") );
      _params.GetValue("dt", _dt);
      
      if (!_params.isPresent("Nt"))
	 throw ( EParamProblem("The laser is missing the number of time steps") );
      _params.GetValue("Nt", Nt);
      
      dVec::newsize(Nt);
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
      Spectrum();
      
      dVec *power = new dVec(_clock->Steps()/2+1);
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
      if (_spectrum == NULL){
         downsize(_clock->Steps());
         _spectrum = new cVec(_clock->Steps()/2+1);
      }
      if (_fft == NULL) _fft = new FFT( *this, *_spectrum);
      
      _fft->forward();
      
      return _spectrum;
   }

   /**
    * Transform spectral representation back to time domain.
    */
   void Laser::ToTimeDomain()
   {
      if (_spectrum != NULL && _fft != NULL)
	 _fft->backward();
   }

   /**
    * Set the clock of laser.
    */
   void Laser::Clock(QDClock * clock)
   {
      if (clock==NULL) return;
      _clock = clock;
      if (clock->Dt() != _dt || clock->Steps() > dVec::size())
	 throw ( EParamProblem("Clock values doesn't fit the laser field") );
   }

   /**
    * Ask for the clock of the laser.
    */
   QDClock * Laser::Clock()
   {
      return _clock;
   }

   /**
    * Get laser value at actual timestep
    */
   double Laser::Get()
   {
      return (*this)[_clock->TimeStep()] * _scale;
   }
   
   /**
    * Set laser value at actual timestep
    */
   void Laser::Set(double value)
   {
      (*this)[_clock->TimeStep()] = value;
   }
   
   /**
    * Get the corresponding vector potential A(t).
    *
    * \f$ A(t) = \int_0^t E(t) dt \f$.
    */
   double Laser::GetVectorPot()
   {
      double sum=0;

      for (int t=0; t < _clock->TimeStep(); t++){
         sum += (*this)[t];
      }

      return sum * _dt;
   }

   Laser& Laser::operator =(const Laser &laser)
   {
      *((dVec*) this) = (dVec) laser;
      _clock = laser._clock;
       _params = laser._params;
      _dt = laser._dt;
      if (_spectrum != NULL){
         delete _spectrum;
         _spectrum = NULL;
      }
      if (_fft != NULL){
         delete _fft;
         _fft=NULL;
      }
      return *this;
   }

   /**
    * \return Energy of the Laserfield
    */
   double Laser::PulseEnergy()
   {
      double d = 0;

      for(int i=0; i < size(); i++){
	 d += (*this)[i] * (*this)[i];
      }
      d *= _scale;
      return ( 137.0359996  / (4 * M_PI) * d * _dt );
      /*       c0 in au          eps_0 in au             */
   }

  /**
   * Set the timestep size.
   */
   void Laser::Dt(double dt)
   {
      _dt = dt;
      _params.SetValue("dt",dt) ;
   }  

   /**
    * Set the number of points.
    * 
    * Resize is destructive.
    */
   void Laser::Nt(int size)
   {
      dVec::newsize(size);
      _params.SetValue("Nt",size) ;
   }
   
   void Laser::Serialize (::google::protobuf::io::ZeroCopyOutputStream& os)
   {
      // Keep format simple
      uint64_t size = dVec::size();

      if (size == 0) return;

      WriteToZeroCopyStream(os, reinterpret_cast<char*>(&size), sizeof(size));
      WriteToZeroCopyStream(os, reinterpret_cast<char*>(&_dt), sizeof(_dt));

      WriteToZeroCopyStream(os, reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

   void Laser::DeSerialize (::google::protobuf::io::ZeroCopyInputStream& is)
   {
      uint64_t size;

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(&size), sizeof(size));

      if (size == 0)
         throw(EParamProblem("Laser field has zero size"));

      double dt;
      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(&dt), sizeof(dt));

      if (dt <= 0)
         throw(EParamProblem("Laser field got a non-sense dt: ", dt));

      _dt = dt;

      newsize(size);

      ReadFromZeroCopyStream(is, reinterpret_cast<char*>(begin(0)), sizeBytes());
   }
   
}







