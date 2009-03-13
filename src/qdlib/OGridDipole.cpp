#include "OGridDipole.h"
#include "tools/Exception.h"

#include "math/math_functions.h"

namespace QDLIB {
   
   
   OGridDipole::OGridDipole() : _name("OGridDipole"), _init(false) {}
   
   void QDLIB::OGridDipole::Clock( QDClock * cl )
   {
      clock = cl;
      _laser.Clock(cl);
   }
   
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
      cout << params << endl;
      /* Protect from double initalization */
      if (!_init){
	 /* Read the laser field */
	 string name;
	 Laser::FileLaser file = _laser.File();
	 
	 if (!params.isPresent("laser"))
	    throw (EParamProblem("No laser file name given"));
	 params.GetValue("laser", name);
	 cout << "Laserfile: " <<name<<endl;
	 file.Suffix(BINARY_O_SUFFIX);
	 file.Name(name);
	 file >> &_laser;
      }
      _init = true;
     
      /* Let parents do initialisation */
      OGridPotential::Init(params);

   }
   
   void OGridDipole::UpdateTime()
   {
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
      MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, _laser.Get());
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



