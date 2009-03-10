#include "OGridDipole.h"
#include "tools/Exception.h"

#include "math/math_functions.h"

namespace QDLIB {
   
   
   OGridDipole::OGridDipole() : _name("OGridDipole") {}
   
   void OGridDipole::SetLaser(Laser & laser)
   {
      _laser = laser;
   }

   Laser & OGridDipole::GetLaser()
   {
      return _laser;
   }

   void OGridDipole::Init(ParamContainer &params)
   {
      /* Let parents do initialisation */
      OGridPotential::Init(params);
      
      /* Read the laser field */
      string name;
      Laser::FileLaser file = _laser.File();
      
      if (_params.isPresent("laser"))
	 throw (EParamProblem("No laser file name given"));
      _params.GetValue("laser", name);
      file.Name(name);
      file >> &_laser;
   }
   
   void OGridDipole::UpdateTime()
   {
      _laser.Clock(clock);
   }

   
   double OGridDipole::Emax()
   {
      
      return VecMax( *((dVec*) this) ) * VecMax( (dVec&) _laser );
   }
   
   double OGridDipole::Emin()
   {
      return 0;
   }
   
   WaveFunction * OGridDipole::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      *destPsi = sourcePsi;
      
      MultElements((cVec*) destPsi, (dVec*) this, _laser.Get());
      
      return destPsi;
   }
   
   WaveFunction * OGridDipole::Apply(WaveFunction *Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this, _laser.Get());
      
      return Psi;
   }
   
   Operator * OGridDipole::operator =(Operator * O)
   {
      OGridDipole* o=dynamic_cast<OGridDipole*>(O);
      if (o == NULL)
	 throw (EIncompatible("Error in assignment", Name(), O->Name() ));
      
      /* Copy parents */
      *((OGridPotential*) this) = *((OGridPotential*) o);
      _laser = o->_laser;
      
      return this;
   }

}

