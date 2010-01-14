#ifndef QDLIBOGRIDNABLA_H
#define QDLIBOGRIDNABLA_H

#include "ODSpace.h"
#include <GridSystem.h>

namespace QDLIB {

   /**
    * Nabla Operator.
    * 
    * \f$ a \sum_i \frac{\partial}{\partial x_i} \f$
    * 
    * prarams:
    * \li fac Prefactor
    * 
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OGridNabla : public ODSpaceCplx, public GridSystem
   {
      private:
         string _name;
         double _fac;  /* Prefactor */
         dVec *_kspace; /* Imaginary part of the k-space */
      protected:
         void _InitDspaceReal();
      public:
         OGridNabla();
      
         ~OGridNabla();
         
         /** Set prefactor a */
         void PreFactor(double factor) { _fac = factor; }
         /** Get prefactor a */
         double PreFactor() { return _fac; }
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
         virtual void Init(WaveFunction *Psi);
	 
         virtual const string& Name();
         
         virtual void UpdateTime() {}  /* Operator is not time dependent */
	 
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
         virtual double Expec(WaveFunction *Psi);
	 
         virtual double Emax();
	 
         virtual double Emin() { return 0; }
	 
         virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual WaveFunction* Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* Copy(Operator* O);
	 
         virtual Operator* operator*(Operator* O);
         
         virtual Operator* Scale(const double d);

         /*Interface implementation, ODSpace  */ 
         virtual void InitDspace();
   };

}

#endif
