#ifndef QDLIBOGRIDNABLASQ_H
#define QDLIBOGRIDNABLASQ_H

#include "ODSpace.h"
#include "Kspace.h"
#include "GridSystem.h"
#include "TransformFFT.h"

namespace QDLIB
{

   /**
    * Nabla square Operator for arbitrary dimensional WaveFunction on a Grid.
    *
    * \f$ \sum_i -\frac{1}{2m}\frac{\partial^2}{\partial x_i} \f$
    * The grid parameters are taken from the wave function after
    * first application of the operator.
    *
    * <p>
    * Takes the parameters:
    * - dims
    * - massn  Reduced mass for for coordinate \f$ x_n \f$. A missing
    * mass for a dimension means not to apply it for this coordinate.
    *	@author Markus Kowalewski
    */
   class OGridNablaSq: public ODSpace, public GridSystem
   {
      private:
         string _name;
         double _mass[MAX_DIMS];
         TransformFFT _FFT;

      public:
         OGridNablaSq();

         ~OGridNablaSq();
         /* Interface implementation, Operator */

         virtual Operator* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void Init(WaveFunction *Psi);

         virtual const string& Name();

         virtual void UpdateTime();

         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);

         virtual double Expec(WaveFunction *Psi);

	 virtual dcomplex Emax();

	 virtual dcomplex Emin();

         virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual WaveFunction* Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         virtual Operator* operator*(Operator* O);

         virtual bool Valid(WaveFunction *Psi);

         virtual Transform* Transformation()
         {
            return &_FFT;
         }

         /*Interface implementation, ODSpace  */
         virtual void InitDspace();
         virtual void InitExponential (cVec *exp, dcomplex c);

   };

}

#endif
