#ifndef ODMULTISTATE_H_
#define ODMULTISTATE_H_

#include "OMultistate.h"
#include "ODSpace.h"
#include "TransformMSD.h"

namespace QDLIB
{

   /**
    * Simple Extension to OMultistates for Diagonalization of Operators.
    *
    * This class only makes sense if every Matrix Element is already diagonal
    * (e.g. Grids in position representation)
    */
   class ODMultistate : public OMultistate, public ODSpace
   {
      private:
         string _name;
         dMat **_X;        /* Eigenvector Matrices (one States x States Matrix per gridpoint */
         TransformMSD _XT; /* Transformation object */
         Transform **_T;   /* Storage for external transformation objects */
         int _state_size;  /* Size of a single state */
      public:
         ODMultistate();
         virtual ~ODMultistate();

         virtual Operator* NewInstance();
         
         virtual void Init(ParamContainer &params) { OMultistate::Init(params); }
         virtual void Init(WaveFunction *Psi);
	 virtual const string& Name() { return _name; }
         virtual void UpdateTime() {OMultistate::UpdateTime();}
         
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
         {
            return OMultistate::MatrixElement(PsiBra, PsiKet);
         }
 	 virtual double Expec(WaveFunction *Psi) { return OMultistate::Expec(Psi); }
 	 virtual double Emax() { return OMultistate::Emax(); }
         virtual double Emin() { return OMultistate::Emin(); }
 	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
         {
            return OMultistate::Apply(destPsi, sourcePsi);
         }
         
         virtual WaveFunction* Apply(WaveFunction *Psi) { return OMultistate::Apply(Psi); }
         virtual Operator* operator=(Operator* O) { return Copy(O); }
 	 virtual Operator* Copy(Operator* O);
 	 virtual Operator* operator*(Operator* O);

         bool Valid(WaveFunction *Psi) { return OMultistate::Valid(Psi); }
         
         virtual Operator* Scale(const double d)
         {
            /* Avoid disambigious virtual overload */
            OMultistate::Scale(d);
            return NULL;
         }

         void InitDspace();
         
         Transform* Transformation() { return &_XT; }
         
   };

}

#endif /* ODMULTISTATE_H_ */
