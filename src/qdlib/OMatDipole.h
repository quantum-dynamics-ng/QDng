#ifndef OMATDIPOLE_H_
#define OMATDIPOLE_H_

#include "OHermitianMatrix.h"
#include "Laser.h"

namespace QDLIB
{

   /**
    * Dipole interaction operator in matrix representation.
    *
    * params:
    * \li laser file name of laser
    */
   class OMatDipole: public QDLIB::OHermitianMatrix
   {
      private:
         string _name;
         Laser _laser;
         bool _init;
      public:
         OMatDipole();

         void SetLaser(Laser &laser);
         Laser* GetLaser();

         virtual string& Name(){ return _name; }

         virtual void Clock(QDClock *cl);

         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

	 virtual dcomplex Emax();

	 virtual dcomplex Emin();

         virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual WaveFunction* Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         /* DSpace features */
         virtual void InitExponential (cVec *exp, dcomplex c);
   };

}

#endif /* OMATDIPOLE_H_ */
