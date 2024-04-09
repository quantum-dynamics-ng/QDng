// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGridStokesAlbrecht.h
 *
 *  Created on: 05.03.2012
 *      Author: mko
 */

#ifndef OGRIDSTOKESALBRECHT_H_
#define OGRIDSTOKESALBRECHT_H_

#include "qdlib/OList.h"

namespace QDLIB
{

   /**
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    *
    * Non-linear friction operator.
    *
    * \f$ \hat S = 6\pi\eta R \frac{1}{m} \left[
    *  c\left( \frac{1}{2}
    *  \left(\hat x \hat p + \hat p \hat x \right ) - \langle x \rangle \hat p \right)
    *  + (1-c) \langle p \rangle (x-\langle x \rangle) \right]\f$
    *
    *  parameters:
    *  \li mass reduced mass
    *  \li R    Particle radius
    *  \li eta  friction coeffiecient of the solvent
    *  \li c    weight factor of the non-linear potentials.
    */
   class OGridStokesAlbrecht : public OList
   {
      private:
         string _name;
         WaveFunction *_buf;
         WaveFunction *_psi;

//         dVec _sigmoid; /* Sigmoid shape mask */
//         dVec _sigmoidInt; /* Sigmoid shape mask */
//         double _x0;
//         double _slope;

         double _eta;

         double _xe; /* <x> */
         double _ve; /* <v>= dx/dt */
      public:
         OGridStokesAlbrecht();
         virtual  ~OGridStokesAlbrecht();

         virtual Operator* NewInstance();
         virtual const string& Name();
         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);
         virtual void UpdateTime(){}
         virtual dcomplex Emax();
         virtual dcomplex Emin() { return dcomplex(0); }
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif /* OGRIDSTOKESALBRECHT_H_ */
