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
         cVec *_spacebuffer;  /* buffer for FFT*/
         
      protected:
         FFT *fft;

      public:
         WFGridSystem();
 
         ~WFGridSystem();
	 
	 virtual void Init(ParamContainer &params);
	 
         virtual void ToKspace(Operator* O);
         
         virtual void ToXspace(Operator* O);
	 
	 void operator=(WFGridSystem *G);

         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
