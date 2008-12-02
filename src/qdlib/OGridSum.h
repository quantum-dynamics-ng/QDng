#ifndef QDLIBOGRIDSUM_H
#define QDLIBOGRIDSUM_H

#include "OGridsystem.h"

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
	 
	 virtual const string& Name();
	 
	 virtual void UpdateTime();
	 
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 
	 virtual double Expec(WaveFunction *Psi);
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);
	 
// 	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const double d);
	 virtual WaveFunction* Apply(WaveFunction *Psi, const dcomplex d);
	 
         virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* operator*(Operator* O);
	 
	 virtual Operator* Offset(const double d);

	 virtual Operator* Scale(const double d);
   };

}

#endif
