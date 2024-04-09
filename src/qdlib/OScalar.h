// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOSCALAR_H
#define QDLIBOSCALAR_H

#include "qdlib/Operator.h"

namespace QDLIB {

   /**
    * Simple scalar value.
    * 
    * options 
    * \li value  The value
    * \li im   Imaginary part
    *   @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    * 
    * \todo remove buffer, this not needed for scalar operator!?
    */
   class OScalar : public Operator
   {
      private:
         string _name;
         WaveFunction* _buf;
         dcomplex _value;
         
      public:
         OScalar();
         ~OScalar();
   
         /* Interface implementation */
	 virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
	 virtual void Init(WaveFunction *Psi);
	 virtual const string& Name();
         virtual void UpdateTime() {};
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
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
