#ifndef LPACKDIAG_H
#define LPACKDIAG_H

#include "math/typedefs.h"
#include "config.h"

#define DSYEV_F77 F77_FUNC (dsyev, DSYEV)
#define ZHEEV_F77 F77_FUNC (zheev, ZHEEV)
#define DSTEV_F77 F77_FUNC (dstev, DSTEV)

#define LAPACK_MAX_WSLIST 4

using namespace QDLIB;

namespace LAPACK {
      

   extern int FullDiagHermitian(dMat *mat, dVec *evals);
   
   extern int FullDiagHermitian(cMat *mat, dVec *evals);
   
   extern int TriDiagHermitian(dVec *diag, dVec *subdiag, dMat *evec, int length);
   
   /* Lapack routines */
   extern "C" {
      /**
       *  DSYEV computes all eigenvalues and, optionally, eigenvectors of a
       *  real symmetric matrix A.
       *
       *  Arguments
       *  =========
       *
       *  JOBZ    (input) CHARACTER*1
       *          = 'N':  Compute eigenvalues only;
       *          = 'V':  Compute eigenvalues and eigenvectors.
       *
       *  UPLO    (input) CHARACTER*1
       *          = 'U':  Upper triangle of A is stored;
       *          = 'L':  Lower triangle of A is stored.
       *
       *  N       (input) INTEGER
       *          The order of the matrix A.  N >= 0.
       *
       *  A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)
       *          On entry, the symmetric matrix A.  If UPLO = 'U', the
       *          leading N-by-N upper triangular part of A contains the
       *          upper triangular part of the matrix A.  If UPLO = 'L',
       *          the leading N-by-N lower triangular part of A contains
       *          the lower triangular part of the matrix A.
       *          On exit, if JOBZ = 'V', then if INFO = 0, A contains the
       *          orthonormal eigenvectors of the matrix A.
       *          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
       *          or the upper triangle (if UPLO='U') of A, including the
       *          diagonal, is destroyed.
       *
       *  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,N).
       *
       *  W       (output) DOUBLE PRECISION array, dimension (N)
       *          If INFO = 0, the eigenvalues in ascending order.
       *
       *  WORK    (workspace/output) DOUBLE PRECISION array, dimension (MAX(1,LWORK))
       *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
       *
       *  LWORK   (input) INTEGER
       *          The length of the array WORK.  LWORK >= max(1,3*N-1).
       *          For optimal efficiency, LWORK >= (NB+2)*N,
       *          where NB is the blocksize for DSYTRD returned by ILAENV.
       *
       *          If LWORK = -1, then a workspace query is assumed; the routine
       *          only calculates the optimal size of the WORK array, returns
       *          this value as the first entry of the WORK array, and no error
       *          message related to LWORK is issued by XERBLA.
       *
       *  INFO    (output) INTEGER
       *          = 0:  successful exit
       *          < 0:  if INFO = -i, the i-th argument had an illegal value
       *          > 0:  if INFO = i, the algorithm failed to converge; i
       *                off-diagonal elements of an intermediate tridiagonal
       *                form did not converge to zero.      
      */
      extern void DSYEV_F77(char* JOBZ, char* UPLO, int* N, double* A, int* LDA,
			    double* W , double* WORK, int* LWORK, int *INFO);
      

      /**
       *  ZHEEV computes all eigenvalues and, optionally, eigenvectors of a
       *  complex Hermitian matrix A.
       *
       *  Arguments
       *  =========
       *
       *  JOBZ    (input) CHARACTER*1
       *          = 'N':  Compute eigenvalues only;
       *          = 'V':  Compute eigenvalues and eigenvectors.
       *
       *  UPLO    (input) CHARACTER*1
       *          = 'U':  Upper triangle of A is stored;
       *          = 'L':  Lower triangle of A is stored.
       *
       *  N       (input) INTEGER
       *          The order of the matrix A.  N >= 0.
       *
       *  A       (input/output) COMPLEX*16 array, dimension (LDA, N)
       *          On entry, the Hermitian matrix A.  If UPLO = 'U', the
       *          leading N-by-N upper triangular part of A contains the
       *          upper triangular part of the matrix A.  If UPLO = 'L',
       *          the leading N-by-N lower triangular part of A contains
       *          the lower triangular part of the matrix A.
       *          On exit, if JOBZ = 'V', then if INFO = 0, A contains the
       *          orthonormal eigenvectors of the matrix A.
       *          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
       *          or the upper triangle (if UPLO='U') of A, including the
       *          diagonal, is destroyed.
       *
       *  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,N).
       *
       *  W       (output) DOUBLE PRECISION array, dimension (N)
       *          If INFO = 0, the eigenvalues in ascending order.
       *
       *  WORK    (workspace/output) COMPLEX*16 array, dimension (MAX(1,LWORK))
       *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
       *
       *  LWORK   (input) INTEGER
       *          The length of the array WORK.  LWORK >= max(1,2*N-1).
       *          For optimal efficiency, LWORK >= (NB+1)*N,
       *          where NB is the blocksize for ZHETRD returned by ILAENV.
       *
       *          If LWORK = -1, then a workspace query is assumed; the routine
       *          only calculates the optimal size of the WORK array, returns
       *          this value as the first entry of the WORK array, and no error
       *          message related to LWORK is issued by XERBLA.
       *
       *  RWORK   (workspace) DOUBLE PRECISION array, dimension (max(1, 3*N-2))
       *
       *  INFO    (output) INTEGER
       *          = 0:  successful exit
       *          < 0:  if INFO = -i, the i-th argument had an illegal value
       *          > 0:  if INFO = i, the algorithm failed to converge; i
       *                off-diagonal elements of an intermediate tridiagonal
       *                form did not converge to zero.
       * 
       */
      extern void ZHEEV_F77(char* JOBZ, char* UPLO, int* N, dcomplex* A,  int* LDA,
			    double* W , dcomplex* WORK, int* LWORK, double *RWORK ,int *INFO);
      
     /**
      SUBROUTINE DSTEV( JOBZ, N, D, E, Z, LDZ, WORK, INFO )
      *
      *  -- LAPACK driver routine (version 3.2) --
      *  -- LAPACK is a software package provided by Univ. of Tennessee,    --
      *  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..--
      *     November 2006
      *
      *     .. Scalar Arguments ..
      CHARACTER          JOBZ
      INTEGER            INFO, LDZ, N
      *     ..
      *     .. Array Arguments ..
      DOUBLE PRECISION   D( * ), E( * ), WORK( * ), Z( LDZ, * )
      *     ..
      *
      *  Purpose
      *  =======
      *
      *  DSTEV computes all eigenvalues and, optionally, eigenvectors of a
      *  real symmetric tridiagonal matrix A.
      *
      *  Arguments
      *  =========
      *
      *  JOBZ    (input) CHARACTER*1
      *          = 'N':  Compute eigenvalues only;
      *          = 'V':  Compute eigenvalues and eigenvectors.
      *
      *  N       (input) INTEGER
      *          The order of the matrix.  N >= 0.
      *
      *  D       (input/output) DOUBLE PRECISION array, dimension (N)
      *          On entry, the n diagonal elements of the tridiagonal matrix
      *          A.
      *          On exit, if INFO = 0, the eigenvalues in ascending order.
      *
      *  E       (input/output) DOUBLE PRECISION array, dimension (N-1)
      *          On entry, the (n-1) subdiagonal elements of the tridiagonal
      *          matrix A, stored in elements 1 to N-1 of E.
      *          On exit, the contents of E are destroyed.
      *
      *  Z       (output) DOUBLE PRECISION array, dimension (LDZ, N)
      *          If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal
      *          eigenvectors of the matrix A, with the i-th column of Z
      *          holding the eigenvector associated with D(i).
      *          If JOBZ = 'N', then Z is not referenced.
      *
      *  LDZ     (input) INTEGER
      *          The leading dimension of the array Z.  LDZ >= 1, and if
      *          JOBZ = 'V', LDZ >= max(1,N).
      *
      *  WORK    (workspace) DOUBLE PRECISION array, dimension (max(1,2*N-2))
      *          If JOBZ = 'N', WORK is not referenced.
      *
      *  INFO    (output) INTEGER
      *          = 0:  successful exit
      *          < 0:  if INFO = -i, the i-th argument had an illegal value
      *          > 0:  if INFO = i, the algorithm failed to converge; i
      *                off-diagonal elements of E did not converge to zero.
      *
      *  =====================================================================
      */
      extern void DSTEV_F77(char* JOBZ, int* N, double* D, double*E, double*Z, int* LDZ, double* WORK, int* INFO );

     
   } /* extern "C" */
   
}

#endif


