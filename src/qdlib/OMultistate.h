#ifndef QDLIBOMULTISTATE_H
#define QDLIBOMULTISTATE_H

#include "Operator.h"
#include "WFMultistate.h"

namespace QDLIB
{

   /**
    * Multistate operator.
    * 
    * Takes several operators as matrix elementsments and acts as a hermitian
    * operator with real matrix elements.
    * 
    * parameters:
    * \li nonhermitian   Explicitly define a non-hermitian operator 
    * 
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OMultistate : public Operator
   {
      private:
	 string _name;
	 
	 bool _hermitian;
	 int _nstates;
	 
	 Operator* _matrix[QD_MAX_STATES][QD_MAX_STATES];
	 
	 WFMultistate *_buf1, *_buf2;
	 
      public:
         OMultistate();

         ~OMultistate();
	 
	 void Hermitian(bool hermit) {_hermitian = hermit;}
	 
	 bool Hermitian() {return _hermitian;}
	 
	 void Add(Operator *O, int row, int col);
	 
	 int States() {return _nstates;}
	 
	 /* Overide Clock setter */
	 virtual void Clock (QDClock *cl);
	 
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
	 
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

	 virtual Operator* Offset(const double d);

// 	 virtual Operator* operator-=(const double d);

	 virtual Operator* Scale(const double d);
   };

}

#endif
