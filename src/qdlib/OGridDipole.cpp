// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OGridDipole.h"
#include "tools/Exception.h"

#include "math/math_functions.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridDipole)

   OGridDipole::OGridDipole() : OGridPotential(), _name("OGridDipole"), _init(false)
   {
      _isTimedependent = true;
   }

   void OGridDipole::Init(ParamContainer &params)
   {
      /* Protect from double initalization */
      if (!_init){
         OLaser::Init(params);
      }
      _init = true;

      /* Let parents do initialisation */
      OGridPotential::Init(params);

   }

   void OGridDipole::UpdateTime()
   {
   }


   dcomplex OGridDipole::Emax()
   {
      return dcomplex(VecMax( *((dVec*) this) ) * VecMax( (dVec&) *GetLaser() ));
   }

   dcomplex OGridDipole::Emin()
   {
      return dcomplex(0);
   }

   void OGridDipole::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, (-1) * GetLaser()->Get());
   }

   void OGridDipole::ApplyAdd(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MultElementsAdd((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, (-1) * GetLaser()->Get());
   }


   void OGridDipole::Apply(WaveFunction *Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this, (-1) * GetLaser()->Get());
   }

   Operator * OGridDipole::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator* OGridDipole::Copy(Operator* O)
   {
      OGridDipole* o=dynamic_cast<OGridDipole*>(O);
      if (o == NULL)
	 throw (EIncompatible("Error in assignment", Name(), O->Name() ));

      SetLaser(*o->GetLaser());
      clock = o->clock;

      /* Copy parents */
      OGridPotential::Copy(O);

      return this;
   }

   void OGridDipole::InitExponential (cVec *exp, dcomplex c)
   {
      if (_dspace == NULL) InitDspace();
      ExpElements(exp, _dspace, -1 * c * GetLaser()->Get());
   }

}




