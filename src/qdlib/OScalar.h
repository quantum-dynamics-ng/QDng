#ifndef QDLIBOSCALAR_H
#define QDLIBOSCALAR_H

#include <Operator.h>

namespace QDLIB {

   /**
    * Simple scalar value.
    * 
    * options 
    * \li val  The value
    *   @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
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
         virtual double Emax() { return _value; }
	 virtual double Emin() { return 0; }
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
	 virtual Operator* Copy(Operator* O);
	 virtual Operator* operator*(Operator* O);
         virtual Operator* Offset(const double d) {return this;}
	 virtual Operator* Scale(const double d) { _value *= d; return this;}

   };

}

#endif
