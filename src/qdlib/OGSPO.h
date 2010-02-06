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

#define GSPO_MAX_LEN 4

namespace QDLIB
{

   /**
    * General SPO Propagator.
    */
   class OGSPO: public QDLIB::OPropagator
   {
      private:
         string _name;
         ParamContainer _needs;
         int _spoLen;                  /* Number of split elements */
         ODSpace *_ops[GSPO_MAX_LEN];  /* operator list */
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
         virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual WaveFunction* Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual Operator* operator*(Operator* O);
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
