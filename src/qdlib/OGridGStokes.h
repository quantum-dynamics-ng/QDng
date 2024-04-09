// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGridGStokes.h
 *
 *  Created on: 28.02.2012
 *      Author: mko
 */

#ifndef OGRIDGSTOKES_H_
#define OGRIDGSTOKES_H_

#include "qdlib/Operator.h"
#include "qdlib/OGridNabla.h"
#include "qdlib/GridSystem.h"
#include "qdlib/OGridGMat.h"
#include "qdlib/OList.h"

namespace QDLIB
{

   /**
    * Non-linear momentum modification with Stokes friction model.
    *
    *
    */
   class OGridGStokes : public OList, public GridSystem
   {
      private:
         string _name;
         OGridGMat* _G;             /* G-Matrix operator of the system */

         double _etaR0;             /* -6 * pi * eta * R0 */
         double _x0[MAX_DIMS];      /* x0's for sigmoid function */
         double _slope[MAX_DIMS];   /* slopes for sigmoid function */

         dVec _s;                   /* shape mask */
         dVec _sInt[MAX_DIMS];      /* integrated shape mask */

         OGridPotential* _GnnG[MAX_DIMS][MAX_DIMS];
         dVec _p;                   /* momentum expec. */
         dVec _Deltap;              /* momentum expec. */
         dVec _v;                   /* velocities */
         dVec _exponent;             /* Storage for exponent of e^ikq */

      public:
         OGridGStokes();
         virtual ~OGridGStokes();

         /* Interface implementation */
         virtual Operator* NewInstance();
         virtual const string& Name();

         virtual void Init(ParamContainer &params);

         virtual void Init(WaveFunction *Psi);

         virtual void UpdateTime(){} /* Nothing to do here */

         virtual dcomplex Emax(){ return dcomplex(0); }

         virtual dcomplex Emin(){ return dcomplex(0); }

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif /* OGRIDGSTOKES_H_ */
