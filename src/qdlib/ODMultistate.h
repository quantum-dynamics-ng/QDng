// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef ODMULTISTATE_H_
#define ODMULTISTATE_H_

#include "OMultistate.h"
#include "ODSpace.h"
#include "TransformMSD.h"

namespace QDLIB
{

   /**
    * Simple Extension to OMultistates for Diagonalization of Operators.
    *
    * This class only makes sense if every off-diagonal Matrix Element is already diagonal
    * (e.g. Grids in position representation)
    * 
    * K-Space operators like \f$ \nabla^2 \f$ are handled via an external transformation
    * for consistensy here (only for diagonals).
    */
   class ODMultistate : public OMultistate, public ODSpace
   {
      private:
         string _name;
         dMat **_X;        /* Eigenvector Matrices (one States x States Matrix per gridpoint */
         TransformMSD _XT; /* Transformation object */
         Transform **_T;   /* Storage for external transformation objects */
         int _state_size;  /* Size of a single state */
         bool _IsFullDiag; /* Indicate if transformation is really needed */
      public:
         ODMultistate();
         virtual ~ODMultistate();

         virtual Operator* NewInstance();
         
         virtual void Init(ParamContainer &params) { OMultistate::Init(params); }
         virtual void Init(WaveFunction *Psi);
	 virtual const string& Name() { return _name; }
         virtual void UpdateTime() {OMultistate::UpdateTime();}
         
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
         {
            return OMultistate::MatrixElement(PsiBra, PsiKet);
         }
 	 virtual double Expec(WaveFunction *Psi) { return OMultistate::Expec(Psi); }
	 virtual dcomplex Emax() { return OMultistate::Emax(); }
	 virtual dcomplex Emin() { return OMultistate::Emin(); }
 	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
         {
            OMultistate::Apply(destPsi, sourcePsi);
         }
         
         virtual void Apply(WaveFunction *Psi) { OMultistate::Apply(Psi); }
         virtual Operator* operator=(Operator* O) { return Copy(O); }
 	 virtual Operator* Copy(Operator* O);
 	 
         bool Valid(WaveFunction *Psi) { return OMultistate::Valid(Psi); }
         
         void InitDspace();
         virtual void InitExponential (cVec *exp, dcomplex c);
         
         Transform* Transformation() { if (_IsFullDiag) return NULL; else return &_XT; }
         
   };

}

#endif /* ODMULTISTATE_H_ */
