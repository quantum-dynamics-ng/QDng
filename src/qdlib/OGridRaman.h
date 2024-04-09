// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef OGridRaman_H
#define OGridRaman_H

#include "qdlib/OGridPotential.h"
#include "qdlib/OLaser.h"

namespace QDLIB
{

   /**
    * Class which represents a dipole interaction on the grid.
    *
    * Parameters:
    * \li laser   the file containing the laser field.
    * \li All other parameters are inherited from GridPotential
    */
   class OGridRaman: public OGridPotential, public OLaser
   {
      private:
         string _name;
         bool _init;
      public:

         OGridRaman();

         ~OGridRaman(){}
         
         virtual const string& Name();

         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void UpdateTime();

         virtual dcomplex Emax();

         virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         /* DSpace features */
         virtual void InitExponential (cVec *exp, dcomplex c);
         
         /* OLaser features */
         
         /**
          * Apply the pure polarizability without laser.
          */
         void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi)
         {
            OGridPotential::Apply(destPsi, sourcePsi);
         }
   };

} /* namespace QDLIB */

#endif /* #ifndef OGridRaman_H */
