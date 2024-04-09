// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGridPML.h
 *
 *  Created on: Jun 18, 2012
 *      Author: markus
 */

#ifndef OGRIDPML_H_
#define OGRIDPML_H_

#include "Operator.h"
#include "Diff.h"
#include "PML.h"

namespace QDLIB
{

   /**
    * Cartesian kinetic energy operator with perfect matched layers boundary.
    */
   class OGridPML : private GridSystem, public Operator, private Diff
   {
      private:
         string _name;
         PML _pml[MAX_DIMS];
         dVec _mass;

         WaveFunction* _buf1;
         WaveFunction* _buf2;
      public:
         OGridPML();
         virtual ~OGridPML();

         virtual Operator* NewInstance();
         virtual const string& Name();

         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);

         virtual dcomplex Emax();
         virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         virtual bool Valid(WaveFunction *Psi);
   };

} /* namespace QDLIB */
#endif /* OGRIDPML_H_ */
