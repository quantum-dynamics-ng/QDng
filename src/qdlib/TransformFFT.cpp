#include "TransformFFT.h"
#include "WFGridSystem.h"

namespace QDLIB
{

   TransformFFT::TransformFFT()
               : Transform(), _norm(1)
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
      
      if (!psi) throw( EIncompatible("FFT can't transform", Psi->Name()) );
     
      if (!psi->fft) throw( EParamProblem("FFT not initialized (maybe NoFFT is switched on?") );
      
      _norm = 1./psi->Size();
      psi->fft->forward();
      psi->IsKspace(true);
   }
   
   /**
    * 
    * Transform the wavefunction into position space.
   */
   void QDLIB::TransformFFT::Backward(WaveFunction * Psi)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      
      if (!psi) throw( EIncompatible("FFT can't transform", Psi->Name()) );
     
      _norm = 1./psi->Size();
      psi->fft->backward();
      psi->IsKspace(false);
   }

}


