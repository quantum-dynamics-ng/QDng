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
   
   int DiagHessenberg(cMat *mat, cVec* evals, cMat* evecs)
   {
      if (mat->rows() != mat->cols()) return -1;
      if (evals->strides() != 1) return -2;

      Memory& mem = Memory::Instance();
      char job='E';
      char compz = 'N';
      int n=mat->rows();
      int ilo = 1;
      int info;

      int wsize = 11*n;
      dcomplex* ws;

      mem.Align( (void**) &ws, wsize );

      ZHSEQR_F77(&job, &compz, &n, &ilo, &n, mat->begin(), &n, evals->begin(0), ws , &n, ws, &wsize, &info);

      if (info > 0)
         cout << "\n*** ZHSEQR Warning: Eigenvalues failed: " << info << endl;
      if (info < 0 ){
         cout << "\n*** ZHSEQR Failed " << info << endl;
      }

      cout << "\nevals" <<*evals;
//      char side = 'R';
//      char eigsrc = 'Q';
//      char initv = 'N';
//      Vector<int> select(n);
//
//      select = 1;
//
//      ZHSEIN_F77(&side, &eigsrc, &initv, select.begin(0), &n, mat->begin(0));

      return info;
   }
}



