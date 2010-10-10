/*
 * ORK4.h
 *
 *  Created on: 09.10.2010
 *      Author: markus
 */

#ifndef ORK4_H_
#define ORK4_H_

#include "OPropagator.h"

namespace QDLIB
{

   /**
    * Runge-Kutta fourth order propgator.
    */
   class ORK4: public QDLIB::OPropagator
   {
      private:
         string _name;
         ParamContainer _needs;
         WaveFunction* _buf[5];
      public:
         ORK4();
         virtual ~ORK4();

         virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params) { _params = params; }
         virtual void Init(WaveFunction *Psi);
         virtual const string& Name();
         virtual void UpdateTime() {if (H != NULL) H->UpdateTime();}
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
         virtual double Expec(WaveFunction *Psi);
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);

         /* Interface implementation, Propagator */
         virtual ParamContainer& TellNeeds();

         virtual void AddNeeds(string &Key, Operator *O);
   };

}

#endif /* ORK4_H_ */