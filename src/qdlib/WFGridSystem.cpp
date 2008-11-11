#include "WFGridSystem.h"
#include "sys/Exception.h"
#include "fft/fft.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false), _spacebuffer(NULL), fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
      if (_spacebuffer != NULL) delete _spacebuffer;
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

      if (_spacebuffer == NULL) _spacebuffer = new cVec(cVec::size());
      
      
      /* Initialize FFT */
      if (fft == NULL){
	 switch(GridSystem::Dim()){
            case 1:
               fft = new FFT(cVec::size(), cVec::begin(), _spacebuffer->begin() );
               break;
            case 2:
	       fft = new FFT(GridSystem::DimSizes(0), GridSystem::DimSizes(1) , cVec::begin(), _spacebuffer->begin() );
               break;
            case 3:
	       fft = new FFT(GridSystem::DimSizes(0), GridSystem::DimSizes(1), GridSystem::DimSizes(2),
			     cVec::begin(), _spacebuffer->begin() );
               break;
            default:
               fft = new FFT(GridSystem::Dim(), GridSystem::DimSizes(), cVec::begin(), _spacebuffer->begin() );
         }
         
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
      
      cVec::swap(*_spacebuffer);    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
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
      cVec::swap(*_spacebuffer);    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
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




