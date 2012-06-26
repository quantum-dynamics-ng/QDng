#include "LapackDiag.h"
#include <stdlib.h>

#include "tools/Memory.h"

using namespace QDLIB;

namespace LAPACK {
   
   /**
    * Diagonalize a real symmetric matrix.
    * 
    * The Upper triangular matrix is used.
    * 
    * \param mat     The matrix to diagonalize. On exit it contains the eigenvectors.
    * \param evals   Contains the eigenvalues 
    */
   int FullDiagHermitian(dMat *mat, dVec *evals)
   {
      int ws_size;
      int size;
      int info=0;
      
      if (mat->rows() != mat->cols()) return -1;
      if (evals->strides() != 1) return -2;
      
      size = mat->rows();
      ws_size = size * 3;
      
      evals->newsize(size);
      
      char jobz = 'V';
      char uplo = 'U';
      
      double *ws;
      Memory::Instance().Align((void**) &ws, ws_size*sizeof(double));
      DSYEV_F77(&jobz, &uplo, &size, mat->begin(),  &size,
		 evals->begin(0) , ws, &ws_size, &info);
      
      Memory::Instance().Free( (void*) ws );

      return info;
   }
   
   int FullDiagHermitian(cMat *mat, dVec *evals)
   {
      int ws_size;
      int size;
      int info;
      
      if (mat->rows() != mat->cols()) return -1;
      if (evals->strides() != 1) return -2;
      
      size = mat->rows();
      ws_size = size * 3;
      
      evals->newsize(size);
      
      char jobz = 'V';
      char uplo = 'U';
      
      double *ws;
      Memory::Instance().Align( (void**) &ws, ws_size*sizeof(dcomplex));

      dcomplex *wsC;
      Memory::Instance().Align((void**) &wsC, ws_size*sizeof(double));

      ZHEEV_F77(&jobz, &uplo, &size, mat->begin(),  &size,
	     evals->begin(0) , wsC , &ws_size, ws, &info);
      
      Memory::Instance().Free( (void*) ws );
      Memory::Instance().Free( (void*) wsC );

      return info;
   }
   
   int TriDiagHermitian(dVec *diag, dVec *subdiag, dMat *evec, const int length)
   {
      char jobz = 'V'; /* compute including eigenvectors */
      int n = length;
      int ws_size;
      int info;
      
      ws_size = 2*n-2;
      
      evec->newsize(n,n);
      
      double *ws;
      Memory::Instance().Align((void**) &ws, ws_size*sizeof(double));

      DSTEV_F77(&jobz, &n ,diag->begin(0), subdiag->begin(0), evec->begin(),  &n, ws, &info);
      
      Memory::Instance().Free( (void*) ws );

      return info;
   }
   
   int DiagGeneral(cMat *mat, cVec* evals, cMat* evecsL, cMat* evecsR)
   {
      if (mat->rows() != mat->cols()) return -1;
      if (evals->strides() != 1) return -2;

      Memory& mem = Memory::Instance();
      int n=mat->rows();
      int ilo = 1;
      int info;

      int wsize = n*n;
      dcomplex* ws;
      mem.Align( (void**) &ws, wsize * sizeof(dcomplex) );

      ZGEHRD_F77(&n, &ilo, &n, mat->begin(), &n, evals->begin(0), ws, &wsize, &info);

      mem.Free( ws );

      return DiagHessenberg(mat, evals, evecsL, evecsR);

   }

   int DiagHessenberg(cMat *mat, cVec* evals, cMat* evecsL, cMat* evecsR)
   {
      if (mat->rows() != mat->cols()) return -1;
      if (evals->strides() != 1) return -2;

      Memory& mem = Memory::Instance();
      char job='E';
      char compz = 'N';
      int n=mat->rows();
      int ilo = 1;
      int info;

      int wsize = n*n;
      dcomplex* ws;

      evals->newsize(n);
      evecsL->newsize(n,n);
      evecsR->newsize(n,n);
      cMat matbuf;

      matbuf = *mat;

      mem.Align( (void**) &ws, wsize * sizeof(dcomplex) );


      ZHSEQR_F77(&job, &compz, &n, &ilo, &n, mat->begin(), &n, evals->begin(0), ws , &n, ws, &wsize, &info);


      if (info > 0)
         cout << "\n*** ZHSEQR Warning: Eigenvalues failed: " << info << endl;
      if (info < 0 ){
         cout << "\n*** ZHSEQR Failed " << info << endl;
      }


      char side = 'B';
      char eigsrc = 'N';
      char initv = 'N';
      Vector<int> select(n);
      Vector<int> lfail(n);
      Vector<int> rfail(n);
      cVec ebuf = *evals;

      double* rws;

      select = 1;

      mem.Align( (void**) &rws, n * sizeof(double) );

      ZHSEIN_F77(&side, &eigsrc, &initv, select.begin(0), &n, matbuf.begin(0), &n, ebuf.begin(0),
               evecsL->begin(), &n, evecsR->begin(), &n, &n, &n, ws, rws, lfail.begin(0), rfail.begin(0), &info);

      mem.Free( ws );
      mem.Free( rws );

      if (info > 0)
         cout << "\n*** ZHSEIN Warning: Eigenvalues failed: " << info << endl;
      if (info < 0 ){
         cout << "\n*** ZHSEIN Failed " << info << endl;
      }

      /*
       * normalize eigenvectors
       */
      dcomplex sl, sr;
      for (int c=0; c < n; c++){
         sl=dcomplex(0);
         sr=dcomplex(0);
         for (int r=0; r < n; r++){
            sl += conj((*evecsL)(r,c)) * (*evecsL)(r,c);
            sr += conj((*evecsR)(r,c)) * (*evecsR)(r,c);
         }
         sl = 1./sqrt(sl.real());
         sr = 1./sqrt(sr.real());
         for (int r=0; r < n; r++){
            (*evecsL)(r,c) = sl.real() * (*evecsL)(r,c);
            (*evecsR)(r,c) = sr.real() * (*evecsR)(r,c);
         }
      }

      return info;
   }

   int InvertGeneral(cMat *mat)
   {
      int m = mat->rows();
      int n = mat->cols();
      int info;

      Vector<int> ipiv(m);


      ZGETRF_F77(&m, &n, mat->begin(), &m, ipiv.begin(0), &info);


      if (info < 0){
         cout << "*** GETRF Error: Invalid argument: " << info << endl;
         return info;
      }

      if (info > 0){
         cout << "*** GETRF Error: Singular element: " << info << endl;
         return info;
      }


      int wsize = 10*n;
      dcomplex* ws;
      Memory::Instance().Align( (void**) &ws, wsize * sizeof(dcomplex) );

      ZGETRI_F77(&n, mat->begin(0), &m, ipiv.begin(0), ws, &wsize, &info);

      if (info < 0){
         cout << "*** GETRF Error: Invalid argument: " << info << endl;
         return info;
      }

      if (info > 0){
         cout << "*** GETRF Error: Singular matrix element: " << info << endl;
         return info;
      }

      return info;

   }
}



