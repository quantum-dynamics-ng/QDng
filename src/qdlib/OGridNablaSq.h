#ifndef QDLIBOGRIDNABLASQ_H
#define QDLIBOGRIDNABLASQ_H

#include "OKSpace.h"
#include "GridSystem.h"

namespace QDLIB {

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
   class OGridNablaSq : public OKSpace, public GridSystem
   {
      private:
	 string _name;
	 double _mass[MAX_DIMS];
      protected:
	 dVec* InitKspace1D(const double mass, const double length, const int Nx);
      public:
	 OGridNablaSq();
      
	 ~OGridNablaSq();
	 /* Interface implementation, Operator */ 
	
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual const string& Name();
         
	 virtual void UpdateTime();
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);

	 /*Interface implementation, OKSpace  */ 
	 virtual void InitKspace();
   };

}

#endif
