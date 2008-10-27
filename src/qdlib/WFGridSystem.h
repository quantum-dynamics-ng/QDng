#ifndef WFGRIDSYSTEM_H
#define WFGRIDSYSTEM_H

#include "WaveFunction.h"
#include "fft/fft.h"

#define MAX_DIMS 8      /* Defines the maximum number of dimensions in a grid */

namespace QDLIB {
   
   /**
    * Abstract wave function class for grid systems.
    */
   class WFGridSystem : public WaveFunction
   {
      private:
         int _ndims;
         int _dims[MAX_DIMS];
         
         bool _isKspace;
         cVec *_spacebuffer;  /* buffer for FFT*/
         
         void _check_kspace();
      protected:
            FFT *fft;

      public:
         WFGridSystem() :
            _ndims(0),
            _isKspace(false), _spacebuffer(NULL), fft(NULL)
	 {
	    for (int i=0; i < MAX_DIMS; i++){
	       _dims[i] = 0;
	    }
	 }
            
         ~WFGridSystem();
         
         int Dim();
         
         void Dim(int dims);
         
         int* DimSizes();
         
         bool isKspace();
         
         void ToKspace();
         
         void ToXspace();
         


         
   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
