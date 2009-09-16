#ifndef WFGRIDSYSTEM_H
#define WFGRIDSYSTEM_H

#include "WaveFunction.h"
#include "GridSystem.h"
#include "fft/fft.h"

namespace QDLIB {
   
   /**
    * Abstract wave function class for grid systems.
    * 
    * 
    * 
    */
   class WFGridSystem : public WaveFunction, public GridSystem
   {
      private:
         bool _isKspace;
         FFT *fft;
         friend class TransformFFT;
      public:
         WFGridSystem();
 
         ~WFGridSystem();
	 
	 virtual void Init(ParamContainer &params);
	 	 
	 void operator=(WFGridSystem *G);

         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
