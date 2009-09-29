

#include "WFGridSystem.h"
#include "tools/Exception.h"
#include "fft/fft.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() : WaveFunction(),
	 _isKspace(false), _spacebuffer(NULL), fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
      if (_spacebuffer != NULL) delete _spacebuffer;
   }
   
   void WFGridSystem::Init(ParamContainer &params)
   {
      if (GridSystem::Dim() == 0 || GridSystem::Dim() > MAX_DIMS)
	 throw( EParamProblem("Dims not initialized or to large") );

      if (_spacebuffer == NULL) _spacebuffer = new cVec(GridSystem::Size());
      
      /* Initialize FFT */
      if (fft == NULL){
	 fft = new FFT(*((GridSystem*) this), *((cVec*) this), *_spacebuffer);
      }
   }
   
   
   /**
    * \return true if wave function is in Kspace representation.
    */
   bool WFGridSystem::isKspace()
   {
      return _isKspace;
   }
   
   /**
    * Tell the WF that it is in k-Space representation.
    */
   void WFGridSystem::isKspace( bool is )
   {
      if (_isKspace != is){
	 cVec::swap(*_spacebuffer);
      }
      _isKspace = is;
   }
      
   /**
    * Transform the wavefunction into momentum space.
    */
   void WFGridSystem::ToKspace()
   {
      fft->forward();
      cVec::swap(*_spacebuffer);    /* The fft ouput is in _spacebuffer => exchange it to data space of WF class  */
      _isKspace = true;
      
   }
   
   /**
    * Transform the wavefunction back into real space.
    */
   void WFGridSystem::ToXspace()
   {
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


