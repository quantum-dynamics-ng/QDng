#ifndef QDLIBOSCALAR_H
#define QDLIBOSCALAR_H

#include "qdlib/Operator.h"

namespace QDLIB {

   /**
    * Simple scalar value.
    * 
    * options 
    * \li val  The value
    *   @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    * 
    * \todo remove buffer, this not needed for scalar operator!?
    */
   class OScalar : public Operator
   {
      private:
         string _name;
         WaveFunction* _buf;
         double _value;
         
      public:
         OScalar();
         ~OScalar();
   
         /* Interface implementation */
	 virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
	 virtual void Init(WaveFunction *Psi);
	 virtual const string& Name() {return _name;}
         virtual void UpdateTime() {};
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 virtual double Expec(WaveFunction *Psi);
	 virtual dcomplex Emax() { return dcomplex(_value); }
	 virtual dcomplex Emin() { return dcomplex(0); }
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
	 virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);

   };

}

#endif
