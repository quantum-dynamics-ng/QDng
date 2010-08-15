#ifndef QDLIBOGRIDNABLA_H
#define QDLIBOGRIDNABLA_H

#include "ODSpace.h"
#include "GridSystem.h"
#include "TransformFFT.h"

namespace QDLIB {

   /**
    * Nabla Operator.
    * 
    * \f$ \sum_i \frac{\partial}{\partial x_i} \f$
    * 
    * Can also be used as momentum operator:
    * 
    * \f$ \hat p = -i \sum_i \frac{\partial}{\partial x_i} \f$
    * 
    * prarams:
    * \li fac Prefactor
    * \li momentum Set operational mode to momentum 
    * \li dim Only setup the operator for specified dimension.
    * 
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OGridNabla : public ODSpaceCplx, public GridSystem
   {
      private:
         string _name;
         double _fac;    /* Prefactor */
         dVec *_kspace;  /* Imaginary part of the k-space */
         bool _momentum; /* Work in momentum mode */
         int _dim;       /* Only act on this dimension */
         TransformFFT _FFT;
         void _InitDim(dVecView &view, const int dim);
      protected:
         void _InitDspaceReal();
      public:
         OGridNabla();
         
         /** Set to momentum mode */
         OGridNabla(bool momentum);
         
         ~OGridNabla();
         
         /** Set prefactor a */
         void PreFactor(double factor) { _fac = factor; }
         /** Get prefactor a */
         double PreFactor() { return _fac; }
         
         /** Set momentum mode */
         void Momentum(bool on) { _momentum = on; }
         /** Get momentum mode */
         double Momentum() { return _momentum; }
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
         virtual void Init(WaveFunction *Psi);
	 
         virtual const string& Name();
         
         virtual void UpdateTime() {}  /* Operator is not time dependent */
	 
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
         virtual double Expec(WaveFunction *Psi);
	 
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin() { return dcomplex(0); }
	 
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* Copy(Operator* O);
	 
         virtual bool Valid(WaveFunction *Psi);
         
         /*Interface implementation, ODSpace  */ 
         virtual Transform* Transformation() { return &_FFT; }
         virtual void InitDspace();
         virtual void InitExponential (cVec *exp, dcomplex c);
   };

}

#endif
