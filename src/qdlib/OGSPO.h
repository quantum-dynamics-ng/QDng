/*
 * OGSPO.h
 *
 *  Created on: 06.02.2010
 *      Author: markus
 */

#ifndef GSPO_H_
#define GSPO_H_

#include "OPropagator.h"
#include "ODSpace.h"
#include "OSum.h"

#define GSPO_MAX_LEN 4

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
         ParamContainer _needs;
         
         OSum* _H;
         
         int _spoLen;                  /* Number of split elements */
         ODSpace* _ops[GSPO_MAX_LEN];  /* operator list */
         cVec _exp[GSPO_MAX_LEN];     /* storage for exponentials */
         int _laststep;                 /* Remember last time step for update */

         void InitExp();
      public:
         OGSPO();

         ~OGSPO();

         virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);
         virtual const string& Name();
         virtual void UpdateTime();
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
         virtual double Expec(WaveFunction *Psi);
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);

         /* Interface implementation, Propagator */
         virtual ParamContainer& TellNeeds()
         {
            return _needs;
         }

         virtual void AddNeeds(string &Key, Operator *O);
   };

}

#endif /* GSPO_H_ */
