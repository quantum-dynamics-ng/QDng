#ifndef OGRIDPOTENTIAL_H
#define OGRIDPOTENTIAL_H

#include "OGridsystem.h"

namespace QDLIB
{
   /**
    * Class which represents an time independent Potential in arb. dimensions.
    * 
    * Takes the parameters:
    * -dims
    * -n#
    * -min#
    * -max#
    */
   class OGridPotential : public OGridSystem
   {
      private:
	 string _name;
	 int _ndims;
	 int _dims[MAX_DIMS];
	 double _xmin[MAX_DIMS];
	 double _xmax[MAX_DIMS]; 
      public:
	 OGridPotential();
   
	 ~OGridPotential();
	 
	 int Dim();
	 void Dims(int ndims);
	 int* DimSizes();
	 
	 
	 /* Interface implementation */
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
   
   };
   
}

#endif
