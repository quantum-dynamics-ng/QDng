#ifndef OGRIDPOTENTIAL_H
#define OGRIDPOTENTIAL_H

#include "OGridsystem.h"

namespace QDLIB
{
   /**
    * Class which represents an time independent Potential in arbitrary dimensions.
    * 
    * Takes the parameters:
    * -dims    Number of dims
    * -n#      Number of points in dim #
    * -min#    xmin
    * -max#    xmax
    * -file    Filename to load potential from (in this case, the other parameters can be empty)
    */
   class OGridPotential : public OGridSystem
   {
      private:
	 string _name;
         bool _init;    /* protect recursive init from deadlock */
      public:
	 OGridPotential();
   
	 ~OGridPotential();
	 
	 
	 /* Interface implementation */
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
   
   };
   
}

#endif
