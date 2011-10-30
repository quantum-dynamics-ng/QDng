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
      
      _norm = 1./psi->Size();
      psi->fft().forward();
      psi->IsKspace(true);
   }
   
   /**
    * 
    * Transform the wavefunction into position space.
   */
   void TransformFFT::Backward(WaveFunction * Psi)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      
      if (!psi) throw( EIncompatible("FFT can't transform", Psi->Name()) );
     
      _norm = 1./psi->Size();
      psi->fft().backward();
      psi->IsKspace(false);
   }

   void TransformFFT::Forward(WaveFunction *Psi, int dim)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      psi->fft().forward(dim);
      psi->IsKspace(true);
   }

   void TransformFFT::Backward(WaveFunction *Psi, int dim)
   {
      WFGridSystem* psi=dynamic_cast<WFGridSystem*>(Psi);
      psi->fft().backward(dim);
      psi->IsKspace(false);
   }
}


