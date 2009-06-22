#include "Reporter.h"
#include "tools/QDGlobalClock.h"
#include "fft/fft.h"
#include "tools/Logger.h"

namespace QDLIB
{

   Reporter::Reporter() :
	 _wcycle(10), _norm(true), _energy(false), _proj0(false), _proj0Sq(false),
		 _spectrum(false), _multistate(false),
	         _psi0(NULL), _H(NULL), _specname(), _rfile(), _specbuf(), _step(0)
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
      WFMultistate *mpsi;
      
      /* Check for Multistate WF*/
      mpsi = dynamic_cast<WFMultistate*>(Psi);
      if (mpsi != NULL){
	 _psims0 = dynamic_cast<WFMultistate*>(Psi->NewInstance());
	 *_psims0 = Psi;
	 _psi0 = _psims0;
	 _multistate = true;
      } else {
	 _psi0 = Psi->NewInstance();
	 *_psi0 = Psi;
      }
      
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
      Logger& log = Logger::InstanceRef();
      WFMultistate *mspsi=NULL;
      
      dcomplex proj;
  
      /* Multistate support */
      if (_multistate)
	 mspsi = dynamic_cast<WFMultistate*>(Psi);

      /* Write header */
      if (_step == 0){
 	 _specbuf.newsize(clock->Steps()+1);
	 log.cout() << "Step\tTime[au]";
	 if (_norm) log.cout() << "\tNorm";
	 if (_multistate && _norm){
	    for (int i=0; i < mspsi->States(); i++)
	       if (_norm) log.cout() << "\t\tNorm_" << i;
	 }
	 if (_energy) log.cout() << "\t\tEnergy[au]";
	 if (_proj0 && !_proj0Sq) log.cout() << "\t<Psi0|PsiT>";
	 if (_proj0 && _proj0Sq) log.cout() << "\t|<Psi0|PsiT>|^2";
	 log.cout() << endl;
	 log.flush();
      }
      _step++;
      
      /* Write time */
      log.cout().precision(1);
      log.cout() << clock->TimeStep() << "\t" << fixed << clock->Time()<< "\t";
         
      /* Norm */
      if (_norm){
	 log.cout().precision(8);
	 log.cout() << "\t" << fixed << Psi->Norm();
	 if (_multistate){ /* Norm of single states */
	    for (int i=0; i< mspsi->States(); i++)
	       log.cout() << "\t" << fixed << mspsi->State(i)->Norm();
	 }
	 
      } 
   
      /* Energy */
      if (_energy)
      {
	 log.cout() << "\t" << _H->Expec(Psi);
      }

      /* projection and spectrum */
      if (_proj0){
	 proj = *_psi0 * Psi;
	 
	 if (_proj0Sq) /* Squared - Normal*/
	    log.cout() << "\t" << cabs(proj)*cabs(proj);
	 else
	    log.cout() << "\t" << proj;
	 
	 if (_spectrum)
	 {
	    if (_proj0Sq)
	       _specbuf[_step-1] = cabs(proj)*cabs(proj);
	    else
	       _specbuf[_step-1] = proj;
	 }
      }
      
      log.cout() << endl;
      log.flush();
   }

   /**
    * Finish the report at the end of a propagation.
    * 
    * The autocorrellation spectrum is done here.
    */
   void Reporter::Finalize( )
   {
      QDClock *clock = QDGlobalClock::Instance();
      Logger& log = Logger::InstanceRef();
      
      
      log.cout() << "\n\n" << clock->Steps() << " step done (" << clock->Steps() *  clock->Dt() << " au)\n\n";
      
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
	  log.cout() << "Autocorellation power spectrum written to file: " << _specname.c_str() << endl << endl;
	 }  catch (...)
	 {log.cout() << "!!! Can't write auto correlation spectrum\n\n";}
      }
      log.flush();
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


