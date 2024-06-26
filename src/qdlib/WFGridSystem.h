// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

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
         FFT *_fft;

         void CheckFFT();
      public:
         FFT& fft();
         WFGridSystem();
         WFGridSystem(const GridSystem& grid);


         ~WFGridSystem();

         virtual void Init(ParamContainer &params);

         void operator=(WFGridSystem *G);

         virtual WaveFunction* Reduce(double tolerance);

         virtual void Restore(WaveFunction* Psi);

         virtual void Reaquire();

         /**
          * Integrates over the WaveFunction.
          */
         virtual dcomplex Integral() = 0;

         virtual void Serialize (std::ostream& os);

         virtual void DeSerialize (std::istream& is);

   }; /* class WFGridSystem */
   
} /* namespace QDLIB */

#endif /* #ifndef WFGRIDSYSTEM_H */
