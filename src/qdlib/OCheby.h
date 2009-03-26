#ifndef QDLIBOCHEBY_H
#define QDLIBOCHEBY_H

#include "OPropagator.h"

#define BESSEL_DELTA 1e-30       /* Lower limit for convergence of the Bessel series convergence (default) */
#define BESSEL_MAX_ORDER 1e6     /* Maximum recursion order to used (by automatic determination) */


namespace QDLIB
{

   /**
    * Chebychev Propagator for WaveFunctions.
    * 
    * Init parameters:
    * \li order   desired order of recursion, if left empty it's chosen automatically.
    * \li scaling The scaling of the Hamiltonian. Can only be used in combination with order.
    * \li prec    The lower limit precission of the bessel coeffcients (mutual exclusive with order)
    * 
    * \todo implement imaginary time propagation.
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OCheby : public OPropagator
   {
      private:
	 string _name;
	 ParamContainer _needs;
	 
	 Operator *_hamilton;   /* The Hamiltonian */
	 int _order;           /* Recursion depth */
	 cVec _coeff;          /* Coefficients including the Bessel functions at k'th order */
	 dcomplex _exp;        /* Scaled exponent:  e.g. -i*dt/Rdelta */
	 double Rdelta;        /* Energy range of the hamiltonian */
	 double Gmin;          /* Energy offset of the hamiltonian */
         WaveFunction *ket0, *ket1, *ket2, *buf;
	 
      public:
         OCheby();
         ~OCheby();
	 
	 int Recursion();
	 
	 void Hamiltonian(Operator *H);
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){ if (_hamilton != NULL) _hamilton->UpdateTime(); }
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 //virtual WaveFunction* operator*(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 //virtual WaveFunction* operator*=(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);

	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 /* Interface implementation, Propagator */
	 virtual ParamContainer& TellNeeds();
	 
	 virtual void AddNeeds(string &Key, Operator *O);
	 

   };

}

#endif
