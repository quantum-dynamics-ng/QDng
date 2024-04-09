// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOGRIDNAC_H
#define QDLIBOGRIDNAC_H

#include "qdlib/OGridNabla.h"
#include "qdlib/OGridPotential.h"

namespace QDLIB {

   /**
    * Coupling operator for Non-Adiabatic coupling.
    * 
    * params:
    * \li dims  Number of grid dimensions
    * \li file  Numeric representation of the NAMCE
    * \li sign  pre-factor (chose 1 or -1)
    * \li mass  reduced mass
    * 
    * \todo implement for N-dims
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OGridNAC : public Operator, public GridSystem
   {
      private:
         string _name;
         OGridPotential _NACME[MAX_DIMS]; /* Holds the NAC Matrix elements*/
         OGridNabla _Nabla[MAX_DIMS];     /* Holds the derivatives */
         dVec* _pNACME[MAX_DIMS];         /* Content pointer to _NACME */
         double _sign;           /* Sign of the coupling */
         double _mass;            /* mass for 1D-NACs */
         WaveFunction *_buf;
      public:
         OGridNAC();
      
         ~OGridNAC();
         
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
         virtual void Init(WaveFunction *Psi);
	 
         virtual const string& Name();
	 
         virtual void UpdateTime(){}
         
         virtual dcomplex Emax();

         virtual dcomplex Emin();
         
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);
	 
         virtual Operator* Copy(Operator* O);
         
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif
