// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOFLUX_H
#define QDLIBOFLUX_H

#include "qdlib/OList.h"
#include "qdlib/WFGridSystem.h"


namespace QDLIB {

   /**
    * Flux Integration Operator.
    * 
    * The operator two subsequent Operators (in order!):
    * \li Kinetic energy operator
    * \li Gobbler which encloses the inner Volume.
    * \todo get ops by name
    * 
    *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OFlux : public OList
   {
      private:
         string _name;
         WaveFunction *_buf;
         WaveFunction *_one;
         int _indT;           /* index of the kinetic energy operator */
         int _indG;           /* index of the Gobbler */
      public:
         OFlux();
         ~OFlux();
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
                 
         virtual void Init(ParamContainer &params);
         
         virtual void Init(WaveFunction *Psi);
         
         virtual const string& Name();
         
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
        
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         
         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
         
         virtual Operator* Copy(Operator* O);
         
         /* dummies */
         virtual dcomplex Emin() {return dcomplex(0);}
         virtual dcomplex Emax() {return dcomplex(0);}
   
   };

}

#endif
