#ifndef QDLIBOCHEBY_H
#define QDLIBOCHEBY_H

#include "OPropagator.h"

#define BESSEL_DELTA 1e-40       /* Upper limit for convergence of the Bessel series convergence */
#define BESSEL_MAX_ORDER 1e3     /* Maximum recursion order to used (by automatic determination) */


namespace QDLIB
{

   /**
    * Chebychev Propagator for WaveFunctions.
    * 
    * Init parameters:
    * \li order: desired order of recursion, if left empty it's chosen automatically
    * 
    * \todo implement imaginary time propagation.
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OCheby : public OPropagator
   {
      private:
	 string _name;
	 Operator *_hamilton;   /* The Hamiltonian */
	 int _order;           /* Recursion depth */
	 cVec _coeff;          /* Coefficients including the Bessel functions at k'th order */
	 dcomplex _exp;        /* Scaled exponent:  e.g. -i*dt/Rdelta */
	 double Rdelta;        /* Energy range of the hamiltonian */
	 double Gmin;          /* Energy offset of the hamiltonian */
         WaveFunction *ket0, *ket1, *ket2;
      public:
         OCheby();
         ~OCheby();
	 
	 int Recursion();
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime(){ if (_hamilton != NULL) _hamilton->UpdateTime(); }
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 /* Interface implementation, Propagator */
	 virtual ParamContainer& TellNeeds();
	 
	 virtual void AddNeeds(string &Key, Operator *O);
	    
	 virtual void ReInit();

   };

}

#endif
