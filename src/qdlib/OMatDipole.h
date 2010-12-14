#ifndef OMATDIPOLE_H_
#define OMATDIPOLE_H_

#include "qdlib/OHermitianMatrix.h"
#include "qdlib/OLaser.h"

namespace QDLIB
{

   /**
    * Dipole interaction operator in matrix representation.
    *
    * params:
    * \li laser file name of laser
    */
   class OMatDipole: public virtual OHermitianMatrix, public OLaser
   {
      private:
         string _name;
         bool _init;
      public:
         OMatDipole();

         virtual const string& Name();

         virtual void Clock(QDClock *cl);

         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);
	 
	 virtual dcomplex Emax();

	 virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         /* DSpace features */
         virtual void InitExponential (cVec *exp, dcomplex c);
	 
	 /* Redifine Operator methods to avoid ambigous base class */
	 virtual void Init(WaveFunction *Psi) { OHermitianMatrix::Init(Psi); }
	 virtual void UpdateTime() { OHermitianMatrix::UpdateTime();}
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
	 {
	    return OHermitianMatrix::MatrixElement(PsiBra, PsiKet);
	 }
	 virtual double Expec(WaveFunction *Psi) { return OHermitianMatrix::Expec(Psi); }
	 bool Valid(WaveFunction *Psi) { return OHermitianMatrix::Valid(Psi); }
         
         /**
          * Apply the pure dipole matrix without laser.
          */
         void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi)
         {
            OHermitianMatrix::Apply(destPsi, sourcePsi);
         }
	 
   };

}

#endif /* OMATDIPOLE_H_ */
