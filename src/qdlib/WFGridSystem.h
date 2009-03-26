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
         cVec *_spacebuffer;  /* buffer for FFT*/
         
      protected:
         FFT *fft;

      public:
         WFGridSystem();
 
         ~WFGridSystem();
	 
	 virtual void Init(ParamContainer &params);
	 
         bool isKspace();
	 
	 void isKspace(bool is);
         
         void ToKspace();
         
         void ToXspace();
	 
	 void operator=(WFGridSystem *G);

         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
