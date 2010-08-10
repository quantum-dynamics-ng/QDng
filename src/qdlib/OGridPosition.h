#ifndef QDLIBOGRIDPOSITION_H
#define QDLIBOGRIDPOSITION_H

#include "OGridsystem.h"

namespace QDLIB {

   /**
    * Position Operator
    * 
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    * 
    * prams:
    * \li dim  Use only for dimension i
    */
   class OGridPosition : public OGridSystem
   {
      private:
         string _name;
         int _dim;
         void _InitDim(dVecView &view, const int dim);
      public:
         OGridPosition();
      
         ~OGridPosition();
   
	 virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
	 virtual void Init(WaveFunction *Psi);
	 virtual const string& Name() {return _name;}
         virtual void UpdateTime(){}
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 virtual double Expec(WaveFunction *Psi);
	 virtual dcomplex Emax();
	 virtual dcomplex Emin();
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
	 virtual Operator* Copy(Operator* O);
	 virtual Operator* operator*(Operator* O);
   };

}

#endif
