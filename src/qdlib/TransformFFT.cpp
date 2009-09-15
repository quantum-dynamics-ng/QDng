#include "TransformFFT.h"
#include "WFGridSystem.h"

namespace QDLIB
{

   TransformFFT::TransformFFT()
               : Transform()
   {
   }
   
   
   TransformFFT::~TransformFFT()
   {
   }
   
   /**
    * 
    * Transform the wavefunction into momentum space.
    */
   void TransformFFT::Forward(WaveFunction * Psi)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      
      if (!psi) throw( EIncompatible("FFT can' transform", Psi->Name()) );
      if (!psi->fft) throw( EParamProblem("FFT not initialized (maybe NoFFT is switched on?") );
      
      psi->fft->forward();
      psi->cVec::swap(*(psi->_spacebuffer));    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
      psi->IsKspace(true);
   }
   
   void QDLIB::TransformFFT::Backward(WaveFunction * Psi)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      
      if (!psi) throw( EIncompatible("FFT can' transform", Psi->Name()) );
      
      psi->fft->backward();
      psi->cVec::swap(*(psi->_spacebuffer));    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
      psi->IsKspace(false);
   }

}


