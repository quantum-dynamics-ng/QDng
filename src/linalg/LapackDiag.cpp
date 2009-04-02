#include "LapackDiag.h"
#include <stdlib.h>

using namespace QDLIB;

namespace LAPACK {
   
   
   
   
   /**
    * Manage workspace memory with singleton.
    */
   class WsList {
      private:
	 static WsList *ref;
	 
	 double*  dbuf[LAPACK_MAX_WSLIST];
	 dcomplex* cbuf[LAPACK_MAX_WSLIST];
	 size_t sized[LAPACK_MAX_WSLIST];
	 size_t sizec[LAPACK_MAX_WSLIST];
	 
	 WsList()
	 {
	    for (int i=0; i < LAPACK_MAX_WSLIST; i++){
	       sized[i] = 0;
	       sizec[i] = 0;
	    }
	 }
	 WsList(WsList &cp){}
	 ~WsList(){}
      public:
	 static WsList* Instance()
	 {
	    if (ref == NULL) ref = new WsList();

	    return ref;
	 }
	 
	 double* Getdbuf(size_t size)
	 {
	    /* look for buff of same size or larger */
	    int i=0;
	    while (i < LAPACK_MAX_WSLIST){
	       if (sized[i] >= size) return dbuf[i];
	       i++;
	    }
	    /* resize smallest buffer */
	    i=0;
	    unsigned int min = 1;
	    unsigned int min_idx = 0;
	    while (i < LAPACK_MAX_WSLIST){
	       if (sized[i] < min) {
		  min_idx = i;
		  min = sized[i];
	       }
	    }
	    if (sized[min_idx] > 0) delete dbuf[min_idx];
	    i=posix_memalign((void**) &dbuf[min_idx], 2*sizeof(double), size*sizeof(double));
	    return dbuf[min_idx];
	 }
	 
	 dcomplex* Getcbuf(size_t size)
	 {
	    /* look for buff of same size or larger */
	    int i=0;
	    while (i < LAPACK_MAX_WSLIST){
	       if (sizec[i] >= size) return cbuf[i];
	       i++;
	    }
	    /* resize smallest buffer */
	    i=0;
	    unsigned int min = 1;
	    unsigned int min_idx = 0;
	    while (i < LAPACK_MAX_WSLIST){
	       if (sizec[i] < min) {
		  min_idx = i;
		  min = sizec[i];
	       }
	    }
	    if (sizec[min_idx] > 0) delete cbuf[min_idx];
	    i=posix_memalign((void**) &cbuf[min_idx], 2*sizeof(dcomplex), size*sizeof(dcomplex));
	    return cbuf[min_idx];
	 }
    };

    WsList* WsList::ref = 0;
    
//	 WsList_s<double> dWsList[LAPACK_MAX_WSLIST];
	 //WsList_s<dcomplex> cWsList[LAPACK_MAX_WSLIST];
   
   
   /**
    * Diagonalize a real symmetric matrix.
    * 
    * \param mat     The matrix to diagonalize. On exit it contains the eigenvectors.
    * \param evals   Contains the eigenvalues 
    */
   void FullDiagHermitian(dMat *mat, dVec *evals)
   {
      int ws_size;
      int size;
      int info;
      WsList *ws = WsList::Instance();
      
      if (mat->rows() != mat->cols()) return;
      if (evals->strides()) return;
      
      size = mat->rows();
      ws_size = size * 3;
      
      evals->newsize(size);
      
      char jobz = 'V';
      char uplo = 'U';
      
      DSYEV_F77(&jobz, &uplo, &size, mat->begin(),  &size,
		 evals->begin(0) , ws->Getdbuf(ws_size), &ws_size, &info);
   }
   
   void FullDiagHermitian(cMat *mat, dVec *evals)
   {
      int ws_size;
      int size;
      int info;
      WsList *ws = WsList::Instance();
      
      if (mat->rows() != mat->cols()) return;
      if (evals->strides()) return;
      
      size = mat->rows();
      ws_size = size * 3;
      
      evals->newsize(size);
      
      char jobz = 'V';
      char uplo = 'U';
      
      ZHEEV_F77(&jobz, &uplo, &size, mat->begin(),  &size,
	     evals->begin(0) , ws->Getcbuf(ws_size), &ws_size, ws->Getdbuf(ws_size), &info);
   }
   
   
   
}

