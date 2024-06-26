// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * ORK4.cpp
 *
 *  Created on: 09.10.2010
 *      Author: markus
 */

#include "ORK4.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(ORK4)

   ORK4::ORK4() : _name("ORK4")
   {
      for (int i=0; i < 5; i++)
         _buf[i] = NULL;
   }

   ORK4::~ORK4()
   {
      for (int i=0; i < 5; i++)
         DELETE_WF(_buf[i]);
   }

   void ORK4::Init(WaveFunction *Psi)
   {
      if (Psi == NULL)
          throw(EParamProblem("RK4: Invalid WaveFunction"));

      if (_buf[0] != NULL) return; // Avoid init twice
      
      H = Get("hamiltonian");

      for (int i=0; i < 5; i++){
         _buf[i] = Psi->NewInstance();
      }

      OPropagator::Init(Psi);
      H->Init(Psi);
   }

   void ORK4::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      *sourcePsi = destPsi;
      Apply(sourcePsi);
   }


   void ORK4::Apply(WaveFunction *Psi)
   {
      for (int i=0; i < 5; i++)
         _buf[i]->Reaquire();

      H->Apply(_buf[1], Psi); /* k1 = f(t, y0) */
      H->RecalcInternals(false);  /* Non-linear operator should not recalulate internal WF specfic values until end of recursion */
      *(_buf[1]) *= Exponent()/clock->Dt();

      /* k2 = f(t+1/2*dt, y0 + 1/2 * dt * k1) */
      *(_buf[2]) = _buf[1];
      *(_buf[2]) *= 0.5*clock->Dt();
      *(_buf[2]) += Psi;
      H->Apply(_buf[2]);
      *(_buf[2]) *= Exponent()/clock->Dt();


      /* k3 = f(t + 1/2 * dt, y0 + 1/2 * dt * k2) */
      *(_buf[3]) = _buf[2];
      *(_buf[3]) *= 0.5*clock->Dt();
      *(_buf[3]) += Psi;
      H->Apply(_buf[3]);
      *(_buf[3]) *= Exponent()/clock->Dt();

      /* k4 = f(t + dt, y0 + dt * k3) */
      *(_buf[4]) = _buf[3];
      *(_buf[4]) *= clock->Dt();
      *(_buf[4]) += Psi;
      H->Apply(_buf[4]);
      *(_buf[4]) *= Exponent()/clock->Dt();

      /* y(t+dt) = y0 + 1/6 * dt * ( k1 + 2*k2 + 2*k3 + k4) */
      *(_buf[2]) *= 2;
      *(_buf[3]) *= 2;

      *(_buf[1]) += _buf[2];
      *(_buf[1]) += _buf[3];
      *(_buf[1]) += _buf[4];

      *(_buf[1]) *= clock->Dt()/6;

      *Psi += _buf[1];

      for (int i=0; i < 5; i++)
         _buf[i]->Retire();

      H->RecalcInternals(true); /* turn it on again */
   }

   Operator* ORK4::operator=(Operator* O)
   {
      return Copy(O);
   }

   Operator* ORK4::Copy(Operator* O)
   {
      ORK4 *o;

      o = dynamic_cast<ORK4*>(O);
      if (o == NULL)
         throw (EIncompatible("ORK4: Can't copy operator"), O->Name());


      OPropagator::Copy(O);

      for (int i=0; i < 5; i++){
         DELETE_WF(_buf[i]);
         _buf[i] = o->_buf[0]->NewInstance();
      }

      return this;
   }

   bool ORK4::Valid(WaveFunction *Psi)
   {
      return H->Valid(Psi);
   }

}
