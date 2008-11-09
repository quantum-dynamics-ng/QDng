#ifndef WFGRIDSYSTEM_H
#define WFGRIDSYSTEM_H

#include "WaveFunction.h"
#include "GridSystem.h"
#include "fft/fft.h"

#define MAX_DIMS 8      /* Defines the maximum number of dimensions in a grid */

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
         
         void _check_kspace();
      protected:
         FFT *fft;

      public:
         WFGridSystem() ;
 
            
         ~WFGridSystem();
         
         bool isKspace();
         
         void ToKspace();
         
         void ToXspace();
         
	 void operator=(WFGridSystem *G);

         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
