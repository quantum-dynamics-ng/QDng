// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef OGRIDPOTENTIAL_H
#define OGRIDPOTENTIAL_H

#include "qdlib/OGridsystem.h"

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

	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

	 virtual void ApplyAdd(WaveFunction *destPsi, WaveFunction *sourcePsi);
	
	 virtual void Apply(WaveFunction *Psi);
	 
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	 
   };
   
}

#endif
