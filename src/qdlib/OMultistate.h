#ifndef QDLIBOMULTISTATE_H
#define QDLIBOMULTISTATE_H

#include "Operator.h"


namespace QDLIB
{

   /**
    * Multistate operator.
    * 
    * Takes several operators as matrix elementsments and acts as a hermitian
    * operator with real matrix elements.
    * 
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OMultistate : public Operator, public Matrix<Operator*>
   {
      private:
	 string _name;
      public:
         OMultistate();

         ~OMultistate();
	 
	 /* Interface implementation */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
	                
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual void UpdateTime();
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
// 	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const double d);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const dcomplex d);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

	 virtual Operator* Offset(const double d);

	 virtual Operator* operator-=(const double d);

	 virtual Operator* Scale(const double d);
   };

}

#endif
