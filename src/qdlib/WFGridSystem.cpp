#include "WFGridSystem.h"
#include "tools/Exception.h"
#include "fft/fft.h"
#include "math/math_functions.h"

namespace QDLIB {
         
   WFGridSystem::WFGridSystem() :
	 _isKspace(false),  fft(NULL)
   {}
	    
   WFGridSystem::~WFGridSystem()
   {
      if (fft != NULL) delete fft;
   }
   
   void QDLIB::WFGridSystem::CheckFFT()
   {
      if (fft == NULL){
         fft = new FFT(*((GridSystem*) this), *((cVec*) this), *GetSpaceBuffer());
      }
   }
   
   void WFGridSystem::Init(ParamContainer &params)
   {
      if (GridSystem::Dim() == 0 || GridSystem::Dim() > MAX_DIMS)
	 throw( EParamProblem("Dims not initialized or to large") );

      bool nofft;
      params.GetValue("NoFFT", nofft, false);
      /* Initialize FFT */
      if (!nofft) CheckFFT();
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

   void WFGridSystem::Reduce(double tolerance)
   {
      double norm;
      int size = cVec::size();
 
      CheckFFT();
      
      fft->forward();
      IsKspace(true);
      
      norm = VecMax(*this) * tolerance; /* This is the cut-off criteria */
      
      int i;
#ifdef _OPENMP
#pragma omp parallel for default(shared) private(i)
#endif
      for (i=0; i <  size; i++){
         /* cut down real & imag seperately */
         if ( abs((*this)[i].real()) < norm )
            (*this)[i]._real = 0;
         
         if ( abs((*this)[i].imag()) < norm )
            (*this)[i]._imag = 0;
      }
      
      *this *= 1./sqrt(double(size)); /* Normalize */
   }

   void WFGridSystem::Restore()
   {
      CheckFFT();
      
      fft->backward();
      IsKspace(false);
      
      *this *= 1./sqrt(double(cVec::size())); /* Normalize */
   }
   
} /* namespace QDLIB */



