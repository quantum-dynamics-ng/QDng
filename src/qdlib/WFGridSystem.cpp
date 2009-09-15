

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

      bool nofft;
      params.GetValue("NoFFT", nofft, false);
      /* Initialize FFT */
      if (_spacebuffer == NULL && !nofft) _spacebuffer = new cVec(cVec::size());
      if (fft == NULL && !nofft){
	 fft = new FFT(*((GridSystem*) this), *((cVec*) this), *_spacebuffer);
      }
   }

   
   /**
    * Copy own content.
    * 
    * Should be used by derived classes to transfers the information correctly.
    */
   void WFGridSystem::operator =(WFGridSystem *G)
   {
      IsKspace(G->IsKspace());
      *((GridSystem*) this) = *((GridSystem*) G);
      *((cVec*) this) = *((cVec*) G);
   }

   
} /* namespace QDLIB */


