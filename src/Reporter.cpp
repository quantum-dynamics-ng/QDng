#include "Reporter.h"

namespace QDLIB
{

   Reporter::Reporter() :
	 _norm(true), _energy(false), _proj0(false), _spectrum(false),
         _psi0(NULL), _specname(), _rfile(), _specbuf(NULL), _step(0)
   {}


   Reporter::~Reporter()
   {}
}

/**
 * Set the wavefunction a time zero.
 * 
 * This is only needed if proj0 or spectrum is turned on.
 * 
 */
void QDLIB::Reporter::PsiInitial( WaveFunction * Psi )
{
   _psi0 = Psi->NewInstance();
   *_psi0 = Psi;
}

void QDLIB::Reporter::Analyze( WaveFunction * Psi )
{
   QDClock *clock = QDGlobalClock->Instance();
   dcomplex proj;
   
   /* Write header */
   if (_step == 0){
      _specbuf.newsize(clock->Steps());
      cout << "Time Step\tTime[au]";
      if (_norm) cout << "\tNorm";
      if (_proj) cout << "\t<Psi0|PsiT>";
      if (_energy) cout << "\tEnergy[au]";
      cout << endl;
   }
   
   /* Write time */
   cout << clock->TimeStep() << "\t" << clock->Time();
   
   /* Norm */
   if (_norm){
      cout << "\t" << *Psi * Psi;
   } 
   
   /* projection and spectrum */
   if (_proj){
      proj = *_psi0 * Psi;
      cout << "\t" << proj;
      
      if (_spectrum)
      {
	 _specbuf[_step] = proj;
      }
   }
   
   /* Energy */
   if (_energy)
   {
      cout << "\t" << _H.Expec(Psi);
   }
   
   cout << endl;
}

void QDLIB::Reporter::Spectrum( const string & name )
{
   _specname = name;
   _spectrum = true;
   _proj0 = true;
}

