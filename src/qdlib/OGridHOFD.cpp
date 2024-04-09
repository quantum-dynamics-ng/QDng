// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * OGridHOFD.cpp
 *
 *  Created on: 08.05.2012
 *      Author: Markus Kowalewski
 */

#include "tools/helpers.h"
#include "OGridHOFD.h"
#include "WFGridSystem.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridHOFD)


   OGridHOFD::OGridHOFD() : _name("OGridHOFD"), _deriv(1), _order(2), _dim(-1), _pfac(1.), _hofd(NULL),
                            _mass(MAX_DIMS), _buf(NULL)
   {
      _mass = 1.;
   }

   OGridHOFD::~OGridHOFD()
   {
      if (_hofd != NULL) delete _hofd;
   }

   void OGridHOFD::Init(ParamContainer& params)
   {
      _params = params;

      _params.GetValue("deriv", _deriv);

      _params.GetValue("order", _order);

      bool pbc;
      _params.GetValue("pbc", pbc, false);

      if ( _params.isPresent("dim") ){
         _params.GetValue("dim", _dim);
      }

      if (_hofd != NULL) delete _hofd;

      _hofd = new cHOFD(_deriv, _order);
      _hofd->SetPBC(pbc);
      _hofd->SetFactor(_pfac);
   }

   void OGridHOFD::Init(WaveFunction* Psi)
   {
      WFGridSystem* psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      *((GridSystem*) this) = *((GridSystem*) psi);

      if (_dim >= GridSystem::Dim() || _dim < -1)
         throw(EParamProblem("Invalid number of active dimension", _dim));

      _hofd->SetGrid(*((GridSystem*) this));

      /* Check mass definitons */
      if ( _params.isPresent("mass") ) {
         vector<double> vec;
         _params.GetArray("mass", vec);
         ConvertArray(vec, _mass);
         dVec factor(_mass.size());

         if (vec.size() < 1)
            throw(EParamProblem("HOFD operator needs at least one mass defintion"));

         /* Check if mass definitions make sense */
         bool eff = false;  /* Indicate at least 1 non-zero dim */

         for (int i = 0; i < GridSystem::Dim(); i++) {
            if (_mass[i] == 0)
               throw(EParamProblem("Zero mass defined"));
            if (_mass[i] > -1) eff = true;

            factor = -1./ (2 * _mass[i]);
         }

         _hofd->SetFactor(factor);
         if (!eff)
            throw(EParamProblem("HOFD Operator is empty (no masses defined)"));
      } else
         _hofd->SetFactor(_mass); /* inti ones */
   }

   dcomplex OGridHOFD::Emax()
   {
      if (_deriv == 1)
         return dcomplex(0, _pfac * M_PI/GridSystem::Dx(0) );
      else {
         double T = 0;
         for (int i = 0; i < GridSystem::Dim(); i++)
            if (_mass[i] > 0)
               T += 1 / (_mass[i] * GridSystem::Dx(i) * GridSystem::Dx(i));

         T *= (M_PI * M_PI / 2);
         return dcomplex(T);
      }
   }

   dcomplex OGridHOFD::Emin()
   {
      if (_deriv == 1)
         return dcomplex(0, -_pfac * M_PI/GridSystem::Dx(0) );
      else
         return dcomplex(0);
   }

   void OGridHOFD::Apply(WaveFunction* destPsi, WaveFunction* sourcePsi)
   {
      if (_dim == -1){
         _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, 0);
         for (int dim=1; dim < Dim(); dim++){
            _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, dim, true);
         }
      } else
         _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, _dim);
   }

   Operator* OGridHOFD::operator =(Operator* O)
   {
      return Copy(O);
   }

   Operator* OGridHOFD::Copy(Operator* O)
   {
      OGridHOFD* o = dynamic_cast<OGridHOFD*>(O);
      if (o == NULL)
         throw (EIncompatible("Not of type OGridHOFD", O->Name()));

      _deriv = o->_deriv;
      _order = o->_order;
      _dim = o->_dim;

      OGridSystem::Copy(O);

      return this;
   }

} /* namespace QDLIB */
