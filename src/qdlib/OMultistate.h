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
    * \todo do implementation
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
	 
	 virtual void Init(WaveFunction *Psi);
	 
	 virtual const string& Name();
	                
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual void UpdateTime();
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
// 	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
// 	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);

	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

	 virtual Operator* Offset(const double d);

	 virtual Operator* operator-=(const double d);

	 virtual Operator* Scale(const double d);
   };

}

#endif
