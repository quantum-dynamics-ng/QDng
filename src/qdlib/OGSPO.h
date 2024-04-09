// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGSPO.h
 *
 *  Created on: 06.02.2010
 *      Author: markus
 */

#ifndef GSPO_H_
#define GSPO_H_

#include "qdlib/OPropagator.h"
#include "qdlib/ODSpace.h"
#include "qdlib/OSum.h"

namespace QDLIB
{

   /**
    * General SPO Propagator.
    *
    * The Operators must be d-space operators and are specified in a sequence:
    * A, B, C,... corresponding to
    * \f$ e^{A+B+C} = e^{\frac{1}{2}C} e^{\frac{1}{2}B} e^{A} e^{\frac{1}{2}B} e^{\frac{1}{2}C}\f$
    * If a transformation is needed for exponentiation, this step is inserted e.g.:
    * \f$ X_B~e^{\frac{1}{2}B}~X_B^\dagger~X_A~e^{A}~X_A^\dagger~
    * X_B~e^{\frac{1}{2}B}~X_B^\dagger\f$
    */
   class OGSPO: public QDLIB::OPropagator
   {
      private:
         string _name;
         
         int _spoLen;                  /* Number of split elements */
         ODSpace* _ops[MAX_OPS];       /* operator list */
         cVec _exp[MAX_OPS];           /* storage for exponentials */
         int _laststep;                /* Remember last time step for update */
         dcomplex _lastexp;            /* Last exponential factor used */

         void InitExp();
      public:
         OGSPO();

         ~OGSPO();

         virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);
         virtual const string& Name();
         virtual void UpdateTime();
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif /* GSPO_H_ */
