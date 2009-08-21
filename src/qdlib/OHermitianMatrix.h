#ifndef OHERMAT_H
#define OHERMAT_H

#include "ODSpace.h"

namespace QDLIB {
   
   /**
    * This simple class represents a real, hermitian Hamiltonian Matrix.
    * 
    * Use it for matrix representation of Hamiltonians.
    * 
    * \todo modify matrix classes to provide real, efficient hermitian matrices.
    */
   template <class T>
   class OHermitianMatrixT: public Matrix<T>, public ODSpaceT<T>
   {
      public:
         typedef FileSingle< OHermitianMatrixT<T> > FileMatrix;
      
      private:
	 dMat *_X;           /* Eigenvector Matrix */
	 bool _valid;        /* Diag representation valid */
         FileMatrix *_file;  /* FileReader for the operator */
         
      protected:
         string _name;
	 
      public:
	 /* Interface implementation, Operator */
	 virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
	 virtual void Init(WaveFunction *Psi);
	 virtual const string& Name();
         virtual void UpdateTime() {};
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	 virtual double Expec(WaveFunction *Psi);
	 virtual double Emax();
	 virtual double Emin();
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 virtual WaveFunction* Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
	 virtual Operator* Copy(Operator* O);
	 virtual Operator* operator*(Operator* O);
         virtual Operator* Offset(const double d);
	 virtual Operator* Scale(const double d);

         /* Interface implementation, ODSpace */
         virtual void InitDspace();
         
         
	 /* Specific methods */
	 OHerMat();
	 OHerMat(int size);
	 ~OHerMat();
	 
	 void Size(int size);
	 int Size();
	 
	 void Diag();
	 dVec* Eval();
	 dVec* Evec();
	 
	 OHerMat& operator=(OHerMat &O);
   };

}

#include "OHermitianMatrix.cpp"

#endif /* #ifndef OHERMAT_H */

