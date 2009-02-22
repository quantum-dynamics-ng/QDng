#include "Reporter.h"
#include "sys/QDGlobalClock.h"
#include "fft/fft.h"

namespace QDLIB
{

   Reporter::Reporter() :
	 _wcycle(10), _norm(true), _energy(false), _proj0(false), _proj0Sq(false),
         _spectrum(false), _psi0(NULL), _H(NULL), _specname(), _rfile(), _specbuf(), _step(0)
   {}


   Reporter::~Reporter()
   {
      if (_psi0 != NULL) delete _psi0;
   }
   
   
   /**
    * Set the wavefunction a time zero.
    * 
    * This is only needed if proj0 or spectrum is turned on.
    * 
    * 
    * \param Psi The wave function a t=0 (copied & delete by reporter)
    */
   void Reporter::PsiInitial( WaveFunction * Psi )
   {
      _psi0 = Psi->NewInstance();
      *_psi0 = Psi;
   }

   /**
    * Set the hamiltonian needed for the energy calculation.
    */
   void Reporter::Hamilton(Operator *H)
   {
      _H = H;
   }
   
   
   /**
    * Run the analyzer.
    */
   void Reporter::Analyze( WaveFunction * Psi )
   {
      QDClock *clock = QDGlobalClock::Instance();
      dcomplex proj;
   
      /* Write header */
      if (_step == 0){
 	 _specbuf.newsize(clock->Steps()+1);
	 cout << "Step\tTime[au]";
	 if (_norm) cout << "\tNorm";
	 if (_proj0 && !_proj0Sq) cout << "\t\t<Psi0|PsiT>";
	 if (_proj0 && _proj0Sq) cout << "\t\t|<Psi0|PsiT>|^2";
	 if (_energy) cout << "\t\tEnergy[au]";
	 cout << endl;
      }
      _step++;
      
      /* Write time */
      cout.precision(1);
      cout << clock->TimeStep() << "\t" << fixed << clock->Time()<< "\t";
   
      /* Norm */
      if (_norm){
	 cout.precision(8);
	 cout << "\t" << fixed << Psi->Norm();
      } 
   
      /* projection and spectrum */
      if (_proj0){
	 proj = *_psi0 * Psi;
	 
	 if (_proj0Sq) /* Squared - Normal*/
	    cout << "\t" << cabs(proj)*cabs(proj);
	 else
	    cout << "\t" << proj;
	 
	 if (_spectrum)
	 {
	    if (_proj0Sq)
	       _specbuf[_step-1] = cabs(proj)*cabs(proj);
	    else
	       _specbuf[_step-1] = proj;
	 }
      }
   
      /* Energy */
      if (_energy)
      {
	 cout << "\t\t" << _H->Expec(Psi);
      }
   
      cout << endl;
   }

   /**
    * Finish the report at the end of a propagation.
    * 
    * The autocorrellation spectrum is done here.
    */
   void Reporter::Finalize( )
   {
      QDClock *clock = QDGlobalClock::Instance();
      
      cout << "\n\n" << clock->Steps() << " step done (" << clock->Steps() *  clock->Dt() << " au)\n";
      
      if (_spectrum){
         cVec spec(_specbuf.size());
	 FFT fftw(_specbuf, spec,true);
	 ofstream ofile;
	 
	 /* Autocorellation power spectrum */
	 try {
	  ofile.open(_specname.c_str());
	  if (!ofile.is_open()) throw;
	  
	  fftw.forward();
	  
	  int N = _specbuf.size();
	  double dw = 2*M_PI / (clock->Dt() * clock->Steps());
		   
	  for (lint i=0; i < N; i++){
	     /* Frequency axis in angualar frequency */
	     if (i < N/2)
	        ofile << i * dw;
	     else
		ofile << -(N-i) * dw;
	     
	     ofile << "\t"<< cabs(spec[i]) / (clock->Steps()/2) << endl;
	  }
	  ofile.close();
	  cout << "Autocorellation power spectrum written to file: " << _specname.c_str() << endl << endl;
	 }  catch (...)
	 {cout << "!!! Can't write auto correlation spectrum\n\n";}
	  
      }
   }

   
   /**
    * Set the name of the spectrum file.
    */
   void Reporter::Spectrum( const string & name )
   {
      _specname = name;
      _spectrum = true;
      _proj0 = true;
   }


}


