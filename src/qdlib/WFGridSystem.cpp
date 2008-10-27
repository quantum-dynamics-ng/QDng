#include "WFGridSystem.h"
#include "sys/Exception.h"
#include "fft/fft.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
      _ndims(0),
      _isKspace(false), _spacebuffer(NULL), fft(NULL)
   {
      for (int i=0; i < MAX_DIMS; i++){
	 _dims[i] = 0;
      }
   }
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
      if (_spacebuffer != NULL) delete _spacebuffer;
   }
   
   /**
    * Number of dimensions.
    */
   int WFGridSystem::Dim()
   {
      return _ndims;
   }
         
   /**
    * Number of dimensions.
    * 
    * Must be set by implementing class.
    */
   void WFGridSystem::Dim(int dims)
   {
      _ndims = dims;
   }
         
   /**
    * Sizes of dimensions.
    * \return Pointer to array of dim sizes (take care of MAX_DIMS!).
    */
   int* WFGridSystem::DimSizes()
   {
      return _dims;
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
      if (_ndims == 0 || _ndims > MAX_DIMS)
         throw( EParamProblem("Dims not initialized or to large") );

      if (_spacebuffer != NULL) _spacebuffer = new cVec(cVec::size());
      
      /* Initialize FFT */
      if (fft == NULL){
         switch(_ndims){
            case 1:
               fft = new FFT(cVec::size(), cVec::begin(), _spacebuffer->begin() );
               break;
            case 2:
               fft = new FFT(_dims[0], _dims[1] ,cVec::begin(), _spacebuffer->begin() );
               break;
            case 3:
               fft = new FFT(_dims[0], _dims[1], _dims[2], cVec::begin(), _spacebuffer->begin() );
               break;
            default:
               fft = new FFT(_ndims, _dims, cVec::begin(), _spacebuffer->begin() );
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
   
   

   
} /* namespace QDLIB */ 
