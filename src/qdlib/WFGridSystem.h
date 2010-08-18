#ifndef WFGRIDSYSTEM_H
#define WFGRIDSYSTEM_H

#include "qdlib/WaveFunction.h"
#include "qdlib/GridSystem.h"
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
         
         void CheckFFT();
      public:
         WFGridSystem();
 
         ~WFGridSystem();
	 
	 virtual void Init(ParamContainer &params);
	 	 
	 void operator=(WFGridSystem *G);

         virtual void Reduce(double tolerance);
         
         virtual void Restore();
         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
