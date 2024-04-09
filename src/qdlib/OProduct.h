// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOPRODUCT_H
#define QDLIBOPRODUCT_H

#include "qdlib/OList.h"

namespace QDLIB {

   /**
    * Product of Operators.
    * 
    * The last Operator in the container is most right one, e.g. the first to apply.
    * 
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OProduct : public OList
   {
      private:
         string _name;
      public:
         OProduct();
      
         ~OProduct();
         
         /* Interface implementation */
         virtual Operator* NewInstance();
               
         virtual const string& Name();
                     
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
      
         virtual dcomplex Emax();
      
         virtual dcomplex Emin();
      
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
      
         virtual void Apply(WaveFunction *Psi);
         
         virtual void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi);
         
         virtual void Apply(Operator* destOp, Operator* sourceOp);
         
         virtual void Apply(Operator* Op);
   };

}

#endif
