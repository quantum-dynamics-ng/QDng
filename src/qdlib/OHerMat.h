#ifndef OHERMAT_H
#define OHERMAT_H

#include "Operator.h"

namespace QDLIB {
   
   /**
    * This simple class represents a real, hermitian Hamiltonian Matrix.
    * 
    * Use it for matrix representation of Hamiltonians.
    * It is compaible with every WaveFunction but is
    * intended to use with WFLevel.
    * 
    * \see OHerOMat for the general case.
    * \todo modify matrix classes to provide real, efficient hermitian matrices.
    */
   class OHerMat: public dMat, public Operator
   {
      private:
	 string _name;
	 dMat *_X;       /* Eigenvector Matrix */
	 dVec *_E;       /* Eigenvalues */
	 
      public:
	 /* Interface implementation , Operator */
	 virtual Operator* NewInstance();
	 virtual void Init(ParamContainer &params);
	 virtual const string& Name();
	 virtual void UpdateTime();
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 virtual double Expec(WaveFunction *Psi);
	 virtual double Emax();
	 virtual double Emin();
	 
	 virtual WaveFunction* operator*(WaveFunction *Psi);	 
	 virtual WaveFunction* operator*=(WaveFunction *Psi);
	 
	 virtual Operator* operator=(Operator *O);
	 virtual Operator* operator*(Operator *O);
	 
	 virtual Operator* operator+=(const double d);
	 virtual Operator* operator-=(const double d);
	 virtual Operator* operator*=(const double d);

         
         
	 /* Specific methods */
	 OHerMat();
	 OHerMat(int size);
	 OHerMat(ParamContainer &params);
	 ~OHerMat();
	 
	 void Size(int size);
	 int Size();
	 
	 void Diag();
	 
	 OHerMat& operator=(OHerMat &O);
	 
         
   };

}


#endif /* #ifndef OHERMAT_H */

