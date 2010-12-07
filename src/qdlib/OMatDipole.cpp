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
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OMatDipole)
	 
   OMatDipole::OMatDipole() :
      OHermitianMatrix(), _name("OMatDipole"), _init(false)
   {
   }

   void OMatDipole::Clock(QDClock * cl)
   {
      Operator::clock = cl;
      GetLaser()->Clock(cl);
   }

   void OMatDipole::Init(ParamContainer &params)
   {
      bool leave_empty;

      params.GetValue("empty", leave_empty, false);

      /* Protect from double initalization */
      if (!_init && !leave_empty) {
         /* Read the laser field */
         string name;
         Laser::FileLaser file = GetLaser()->File();

         if (!params.isPresent("laser"))
            throw(EParamProblem("No laser file name given"));
         params.GetValue("laser", name);
         file.Suffix(BINARY_O_SUFFIX);
         file.Name(name);
	 file >> GetLaser();
      }
      _init = true;

      /* Let parents do initialisation */
      OHermitianMatrix::Init(params);

   }

   dcomplex OMatDipole::Emax()
   {
      return dcomplex(OHermitianMatrix::Emax() * VecMax((dVec&) *GetLaser()));
   }

   dcomplex OMatDipole::Emin()
   {
      return dcomplex(OHermitianMatrix::Emin() * VecMin((dVec&) *GetLaser()));
   }

   void OMatDipole::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      OHermitianMatrix::Apply(destPsi, sourcePsi);
      *destPsi *= -1*GetLaser()->Get();
   }

   void OMatDipole::Apply(WaveFunction *Psi)
   {
      OHermitianMatrix::Apply(Psi);
      *Psi *= -1*GetLaser()->Get();
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

      SetLaser(*o->GetLaser());
      Operator::clock = o->clock;

      /* Copy parents */
      OHermitianMatrix::Copy(O);

      return this;
   }

   void OMatDipole::InitExponential(cVec *exp, dcomplex c)
   {
      if (_dspace == NULL)
         InitDspace();

      ExpElements(exp, _dspace, -1 * c * GetLaser()->Get());
   }
}
