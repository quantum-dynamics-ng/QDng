#include "OGridDipole.h"
#include "sys/Exception.h"

namespace QDLIB {
   
   
   OGridDipole::OGridDipole() : _name("OGridDipole") {}
   
   void QDLIB::OGridDipole::Laser(Laser & laser)
   {
      _laser = laser;
   }

   Laser & QDLIB::OGridDipole::Laser()
   {
      return _laser;
   }

   void OGridDipole::Init(ParamContainer &params)
   {
      /* Let parents do initialisation */
      OGridPotential::Init(params);
      
      /* Read the laser field */
      string name;
      Laser::FileLaser file = laser.File();
      
      if (_params.isPresent("laser"))
	 throw (EParamProblem("No laser file name given"));
      _params.GetValue("laser", name);
      file.Name(name);
      file >> laser;
   }
   
   void QDLIB::OGridDipole::UpdateTime()
   {
      laser.Clock(clock);
   }

   
   double QDLIB::OGridDipole::Emax()
   {
      return VecMax( (dVec) *this ) * VecMax( (dVec) laser );
   }
   
   double QDLIB::OGridDipole::Emin()
   {
      return 0;
   }
   
   WaveFunction * QDLIB::OGridDipole::operator *(WaveFunction * Psi)
   {
      WaveFunction *psi = Psi->NewInstance();
      
      MultElements((cVec*) psi, (dVec*) this, laser.Get());
      
      return psi;
   }
   
   WaveFunction * QDLIB::OGridDipole::operator *=(WaveFunction * Psi)
   {
      MultElements((cVec*) Psi, (dVec*) this, laser.Get());
      
      return psi;
   }
   
   Operator * OGridDipole::operator =(Operator * O)
   {
      OGridDipole* o=dynamic_cast<OGridDipole*>(O);
      if (o == NULL)
	 throw (EIncompatible("Error in assignment", Name(), O->Name() ));
      
      /* Copy parents */
      *((OGridPotential*) this) = *((OGridPotential*) o);
      laser = o->_laser;
   }

}




