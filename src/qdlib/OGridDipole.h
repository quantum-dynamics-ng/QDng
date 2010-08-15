#ifndef OGRIDDIPOLE_H
#define OGRIDDIPOLE_H

#include "OGridPotential.h"
#include "Laser.h"

namespace QDLIB
{

   /**
    * Class which represents a dipole interaction on the grid.
    *
    * Parameters:
    * \li laser   the file containing the laser field.
    * \li All other parameters are inherited from GridPotential
    */
   class OGridDipole: public OGridPotential
   {
      private:
         string _name;
         Laser _laser;
         bool _init;
      public:
         void SetLaser(Laser &laser);
         Laser* GetLaser();

         OGridDipole();

         ~OGridDipole(){}

         virtual string& Name(){ return _name; }

         virtual void Clock(QDClock *cl);

         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void UpdateTime();

         virtual dcomplex Emax();

	 virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         /* DSpace features */
         virtual void InitExponential (cVec *exp, dcomplex c);
   };

} /* namespace QDLIB */

#endif /* #ifndef OGRIDDIPOLE_H */
