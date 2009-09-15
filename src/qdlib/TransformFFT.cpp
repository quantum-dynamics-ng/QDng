#include "TransformFFT.h"

namespace QDLIB
{

   TransformFFT::TransformFFT()
               : Transform(), fft(NULL)
   {
   }
   
   
   TransformFFT::~TransformFFT()
   {
   }
   
   /**
    * 
    * Transform the wavefunction into momentum space.
    */
   void QDLIB::TransformFFT::Forward(Wavefunction * Psi)
   {
      Wavefunction* psi=dynamic_cast<WFGridSystem*>(Psi);
      
      if (!psi) throw( EIncompatible("FFT can' transform") Psi->Name());
      
      psi->fft->forward();
      psi->cVec::swap(psi->*_spacebuffer);    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
      psi->IsKspace(true);
   }
   
   void QDLIB::TransformFFT::Inverse(Wavefunction * Psi)
   {
      psi->fft->backward();
      psi->cVec::swap(psi->*_spacebuffer);    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
      psi->IsKspace(false);
   }

}


