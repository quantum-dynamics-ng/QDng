#ifndef QDLIBOCHEBY_H
#define QDLIBOCHEBY_H

#include "OPropagator.h"

namespace QDLIB
{

   /**
    * Chebychev Propagator for WaveFunctions.
    * 
    * Init parameters:
    * \li order: desired order of recursion
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OCheby : public OPropagator
   {
      private:
	 string _name;
	 Operator _hamilton;   /* The Hamiltonian */
	 int _order;           /* Recursion depth */
	 cVec _coeff;         /* Coefficients including the Bessel functions at k'th order */
	 double Rdelta;        /* Energy range of the hamiltonian */
	 double Gmin;          /* Energy offset of the hamiltonian */
      public:
         OCheby();
         ~OCheby();
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	                
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
