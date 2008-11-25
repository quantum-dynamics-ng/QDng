#include "WFGridSystem.h"
#include "sys/Exception.h"
#include "fft/fft.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false), fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
   }
   
   
   /**
    * \return true if wave function is in Kspace representation.
    */
   bool WFGridSystem::isKspace()
   {
      return _isKspace;
   }
   
   /**
    * Check if FFT is initialized.
    */
   void WFGridSystem::_check_kspace()
   {
      if (GridSystem::Dim() == 0 || GridSystem::Dim() > MAX_DIMS)
         throw( EParamProblem("Dims not initialized or to large") );
      
      /* Initialize FFT */
      if (fft == NULL){
	 fft = new FFT(*((GridSystem*) this), *((cVec*) this));
      }
   }
   
   /**
    * Transform the wavefunction into momentum space.
    */
   void WFGridSystem::ToKspace()
   {
      if (_isKspace) return;
      
      _check_kspace();
      fft->forward();
      _isKspace = true;
   }
   
   /**
    * Transform the wavefunction back into real space.
    */
   void WFGridSystem::ToXspace()
   {
      if (!_isKspace) return;
      
      _check_kspace();
      fft->backward();
      _isKspace = false;
      
   }
   
   /**
    * Copy own content.
    * 
    * Should be used by derived classes to transfers the information correctly.
    */
   void WFGridSystem::operator =(WFGridSystem *G)
   {
      _isKspace = G->_isKspace;
      *((GridSystem*) this) = *((GridSystem*) G);
      *((cVec*) this) = *((cVec*) G);
   }

   
} /* namespace QDLIB */




