// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef WFLEVEL_H
#define WFLEVEL_H

#include "qdlib/WaveFunction.h"

namespace QDLIB {
   
   /**
    * Wavefunction class for a simple level system.
    */
   class WFLevel: public WaveFunction
   {
      private:
         string _name;

      public:
         /* Interface implementation */
         WFLevel();

         WFLevel(size_t size);

         ~WFLevel();

         WaveFunction* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual const string& Name();

         WaveFunction* operator=(WaveFunction *Psi);

         dcomplex operator*(WaveFunction *Psi);

         virtual WaveFunction* Reduce(double tolerance);

         virtual void Restore(WaveFunction* Psi);

         /* Own methods */
         void Size(int size);

         WFLevel& operator=(WFLevel &Psi);

         virtual void Serialize (std::ostream& os);

         virtual void DeSerialize (std::istream& is);

   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */
