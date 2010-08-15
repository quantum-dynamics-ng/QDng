#ifndef OHERMAT_H
#define OHERMAT_H

#include "ODSpace.h"
#include "tools/FileSingle.h"
#include "TransformMat.h"

namespace QDLIB
{

   /**
    * This simple class represents a real, hermitian Hamiltonian Matrix.
    *
    * Use it for matrix representation of Hamiltonians.
    *
    * Parameters:
    * \li size
    * \li qdiag      Diagonalize immidately.s
    * \li setzero    Shift diagonal to zero
    * \li file       Input from file.
    *
    * \todo modify matrix classes to provide real, efficient hermitian matrices.
    */
   class OHermitianMatrix: public dMat, public ODSpaceT<double>
   {
      public:
         typedef FileSingle<OHermitianMatrix> FileMatrix;

      private:
         dMat *_X; /* Eigenvector Matrix */
         TransformMat _XT; /* Transformer object */
         bool _valid; /* Diag representation valid */
         FileMatrix *_file; /* FileReader for the operator */
         bool _init; /* indicator for recursion in Init */

      protected:
         string _name;

      public:
         OHermitianMatrix();
         ~OHermitianMatrix();

         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
         virtual void Init(ParamContainer &params);
         virtual void Init(WaveFunction *Psi);
         virtual const string& Name();
         virtual void UpdateTime()
         {
         }
         ;
         virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
         virtual double Expec(WaveFunction *Psi);
	 virtual dcomplex Emax();
	 virtual dcomplex Emin();
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
         virtual void Apply(WaveFunction *Psi);
         virtual Operator* operator=(Operator* O);
         virtual Operator* Copy(Operator* O);
         virtual void Apply(Operator *destOp, Operator *sourceOp);
         bool Valid(WaveFunction *Psi);

         /* Interface implementation, ODSpace */
         virtual void InitDspace();
         virtual Transform* Transformation()
         {
            return &_XT;
         }

         /* Specific methods */
         void Size(int size);
         int Size();

         void Diag();

         FileMatrix* File()
         {
            if (_file == NULL)
               _file = new FileMatrix();
            return _file;
         }

         OHermitianMatrix& operator=(OHermitianMatrix &O);
   };

}

#endif /* #ifndef OHERMAT_H */

