/*
 * OMatDipole.cpp
 *
 *  Created on: 07.02.2010
 *      Author: markus
 */

#include "OMatDipole.h"
#include "math/math_functions.h"

namespace QDLIB
{

   OMatDipole::OMatDipole() :
      OHermitianMatrix(), _name("OMatDipole"), _init(false)
   {
      _isTimedependent = true;
   }

   void OMatDipole::SetLaser(Laser & laser)
   {
      _laser = laser;
   }

   Laser* OMatDipole::GetLaser()
   {
      return &_laser;
   }

   Operator* OMatDipole::NewInstance()
   {
      OMatDipole *r = new OMatDipole();
      return r;
   }

   void OMatDipole::Clock(QDClock * cl)
   {
      clock = cl;
      _laser.Clock(cl);
   }

   void OMatDipole::Init(ParamContainer &params)
   {
      bool leave_empty;

      params.GetValue("empty", leave_empty, false);

      /* Protect from double initalization */
      if (!_init && !leave_empty) {
         /* Read the laser field */
         string name;
         Laser::FileLaser file = _laser.File();

         if (!params.isPresent("laser"))
            throw(EParamProblem("No laser file name given"));
         params.GetValue("laser", name);
         file.Suffix(BINARY_O_SUFFIX);
         file.Name(name);
         file >> &_laser;
      }
      _init = true;

      /* Let parents do initialisation */
      OHermitianMatrix::Init(params);

   }

   double OMatDipole::Emax()
   {
      return OHermitianMatrix::Emax() * VecMax((dVec&) _laser);
   }

   double OMatDipole::Emin()
   {
      return OHermitianMatrix::Emin() * VecMin((dVec&) _laser);
   }

   WaveFunction * OMatDipole::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      OHermitianMatrix::Apply(destPsi, sourcePsi);
      *destPsi *= _laser.Get();
      
      return destPsi;
   }

   WaveFunction * OMatDipole::Apply(WaveFunction *Psi)
   {
      OHermitianMatrix::Apply(Psi);
      *Psi *= _laser.Get();
      return Psi;
   }

   Operator * OMatDipole::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator* OMatDipole::Copy(Operator* O)
   {
      OMatDipole* o = dynamic_cast<OMatDipole*> (O);

      if (o == NULL)
         throw(EIncompatible("Error in assignment", Name(), O->Name()));

      _laser = o->_laser;
      clock = o->clock;

      /* Copy parents */
      OHermitianMatrix::Copy(O);

      return this;
   }

   void OMatDipole::InitExponential(cVec *exp, dcomplex c)
   {
      if (_dspace == NULL)
         InitDspace();

      ExpElements(exp, _dspace, c * _laser.Get());
   }
}
