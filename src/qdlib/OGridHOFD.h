// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGridHOFD.h
 *
 *  Created on: 08.05.2012
 *      Author: markus
 */

#ifndef OGRIDHOFD_H_
#define OGRIDHOFD_H_

#include "qdlib/OGridsystem.h"
#include "qdlib/HOFD.h"

namespace QDLIB
{

   /**
    * Higher order finite differenciating scheme.
    *
    * First and second derivitives from 2. - 8. order
    * Parameters:
    *  \li deriv
    *  \li order
    *  \li dim   Dimension to act on
    *  \li mass  If given the prefactor is set to -1/(2m)
    *
    * \author Markus Kowalewksi
    */
   class OGridHOFD : public OGridSystem
   {
      private:
         string _name;
         int _deriv;     /* n-derivative */
         int _order;     /* accuracy */
         int _dim;       /* Dim. to act on. All if -1 */
         double _pfac;   /* contains a prefactor */
         cHOFD *_hofd;    /* diff. operator */
         dVec _mass;
         WaveFunction *_buf;
      public:
         OGridHOFD();
         virtual ~OGridHOFD();

         /* Interface implementation, Operator */
         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void Init(WaveFunction *Psi);

         virtual const string& Name();

         virtual void UpdateTime() {};

         virtual dcomplex Emax();

         virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

   };

} /* namespace QDLIB */
#endif /* OGRIDHOFD_H_ */
