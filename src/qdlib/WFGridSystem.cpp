

#include "WFGridSystem.h"
#include "tools/Exception.h"
#include "fft/fft.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false),  fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
   }
   
   void WFGridSystem::Init(ParamContainer &params)
   {
      if (GridSystem::Dim() == 0 || GridSystem::Dim() > MAX_DIMS)
	 throw( EParamProblem("Dims not initialized or to large") );

      bool nofft;
      params.GetValue("NoFFT", nofft, false);
      /* Initialize FFT */
      if (fft == NULL && !nofft){
         fft = new FFT(*((GridSystem*) this), *((cVec*) this), *GetSpaceBuffer());
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


