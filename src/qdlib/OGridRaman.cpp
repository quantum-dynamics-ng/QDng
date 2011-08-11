#include "OGridRaman.h"
#include "tools/Exception.h"

#include "math/math_functions.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridRaman)

   OGridRaman::OGridRaman() : OGridPotential(), OLaser(2), _name("OGridRaman"), _init(false)
   {
      _isTimedependent = true;
   }

   void OGridRaman::Init(ParamContainer &params)
   {
      /* Protect from double initalization */
      if (!_init){
         OLaser::Init(params);
      }
      _init = true;

      /* Let parents do initialisation */
      OGridPotential::Init(params);

   }

   void OGridRaman::UpdateTime()
   {
   }


   dcomplex OGridRaman::Emax()
   {
      return dcomplex(0.5 * VecMax( *((dVec*) this) ) * VecMax( (dVec&) *GetLaser(0) ) * VecMax( (dVec&) *GetLaser(1) ));
   }

   dcomplex OGridRaman::Emin()
   {
      return dcomplex(0);
   }

   void OGridRaman::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, (-.5) * GetLaser(0)->Get() * GetLaser(1)->Get());
   }

   void OGridRaman::Apply(WaveFunction *Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this, (-.5) * GetLaser(0)->Get() * GetLaser(1)->Get());
   }

   Operator * OGridRaman::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator* OGridRaman::Copy(Operator* O)
   {
      OGridRaman* o=dynamic_cast<OGridRaman*>(O);
      if (o == NULL)
         throw (EIncompatible("Error in assignment", Name(), O->Name() ));

      SetLaser(0, *o->GetLaser(0));
      SetLaser(1, *o->GetLaser(1));
      clock = o->clock;

      /* Copy parents */
      OGridPotential::Copy(O);

      return this;
   }

   void OGridRaman::InitExponential (cVec *exp, dcomplex c)
   {
      if (_dspace == NULL) InitDspace();
      ExpElements(exp, _dspace, -1 * c * GetLaser(0)->Get() * GetLaser(1)->Get());
   }

}




