#ifndef QDLIBOSOD_H
#define QDLIBOSOD_H

#include "OPropagator.h"

namespace QDLIB
{

   /**
    * SOD Propagator for general Problems.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OSOD : public OPropagator
   {
      private:
	 string _name;
	 WaveFunction *_psi_last;   /* WF from last step */
	 Operator *_ham;            /* The hamiltonian */
	 dcomplex _exp;             /* Exponential */
	 bool _isUpToDate;
	 
	 void _Init(WaveFunction *Psi);
      public:
         OSOD();
         ~OSOD();
	 
	 void AddHamiltonian (Operator *H);
	 
	 /* Interface implementation, Operator */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime() {}
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const double d);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const dcomplex d){return Psi;}
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 /* Interface implementation, Propagator */
	 virtual ParamContainer& TellNeeds();
	 
	 virtual void AddNeeds(string &Key, Operator *O);
	 
	 virtual void ReInit();
   };

}

#endif
