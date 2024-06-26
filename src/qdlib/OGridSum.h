// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBOGRIDSUM_H
#define QDLIBOGRIDSUM_H

#include "qdlib/OGridsystem.h"

#define MAX_OPS 16

namespace QDLIB {

   /**
    * Represents a sum of OGridSystem system operators.
    * 
    * Similar to OSum but much more efficient when used with grids.
    * Uses an internal dVec (from OGridSystem) for summing the Operators.
    * 
    * This is also needed e.g. for the SPO to build the exponentials.
    *
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OGridSum : public OGridSystem
   {
      private:
	 string _name;
	 int _size;
	 OGridSystem* _O[MAX_OPS];
	 bool _isUpTodate;
	 
	 void _Update();
      public:
	 OGridSum();
      
	 virtual ~OGridSum();
         
	 OGridSystem* Get( int i );
	 
	 int Size();
         
	 void Add(OGridSystem *O); 
	 
   	 /* Interface implementation, Operator */
	 virtual Operator* NewInstance();
	 	 
         virtual void Init(ParamContainer &params);
	 
	 virtual void Init( WaveFunction * Psi );
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime();
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual dcomplex Emax();
	 
	 virtual dcomplex Emin();
	 
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
	 virtual void Apply(WaveFunction *Psi);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
   };

}

#endif
