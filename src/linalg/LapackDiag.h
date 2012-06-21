#ifndef LPACKDIAG_H
#define LPACKDIAG_H

#include "math/typedefs.h"
#include "config.h"

#define DSYEV_F77 F77_FUNC (dsyev, DSYEV)
#define ZHEEV_F77 F77_FUNC (zheev, ZHEEV)
#define DSTEV_F77 F77_FUNC (dstev, DSTEV)
#define ZHSEQR_F77 F77_FUNC (zhseqr, ZHSEQR)
#define ZHSEIN_F77 F77_FUNC (zhsein, ZHSEIN)

#define LAPACK_MAX_WSLIST 4

using namespace QDLIB;

namespace LAPACK {
      

   extern int FullDiagHermitian(dMat *mat, dVec *evals);
   
   extern int FullDiagHermitian(cMat *mat, dVec *evals);
   
   extern int TriDiagHermitian(dVec *diag, dVec *subdiag, dMat *evec, int length);
   
   extern int DiagHessenberg(cMat *mat, cVec* evals, cMat* evecsL, cMat* evecsR);

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


      /**
       * Purpose
      *     =======
      *
      *     ZHSEQR computes the eigenvalues of a Hessenberg matrix H
      *     and, optionally, the matrices T and Z from the Schur decomposition
      *     H = Z T Z**H, where T is an upper triangular matrix (the
      *     Schur form), and Z is the unitary matrix of Schur vectors.
      *
      *     Optionally Z may be postmultiplied into an input unitary
      *     matrix Q so that this routine can give the Schur factorization
      *     of a matrix A which has been reduced to the Hessenberg form H
      *     by the unitary matrix Q:  A = Q*H*Q**H = (QZ)*H*(QZ)**H.
      *
      *     Arguments
      *     =========
      *
      *     JOB   (input) CHARACTER*1
      *           = 'E':  compute eigenvalues only;
      *           = 'S':  compute eigenvalues and the Schur form T.
      *
      *     COMPZ (input) CHARACTER*1
      *           = 'N':  no Schur vectors are computed;
      *           = 'I':  Z is initialized to the unit matrix and the matrix Z
      *                   of Schur vectors of H is returned;
      *           = 'V':  Z must contain an unitary matrix Q on entry, and
      *                   the product Q*Z is returned.
      *
      *     N     (input) INTEGER
      *           The order of the matrix H.  N .GE. 0.
      *
      *     ILO   (input) INTEGER
      *     IHI   (input) INTEGER
      *           It is assumed that H is already upper triangular in rows
      *           and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally
      *           set by a previous call to ZGEBAL, and then passed to ZGEHRD
      *           when the matrix output by ZGEBAL is reduced to Hessenberg
      *           form. Otherwise ILO and IHI should be set to 1 and N
      *           respectively.  If N.GT.0, then 1.LE.ILO.LE.IHI.LE.N.
      *           If N = 0, then ILO = 1 and IHI = 0.
      *
      *     H     (input/output) COMPLEX*16 array, dimension (LDH,N)
      *           On entry, the upper Hessenberg matrix H.
      *           On exit, if INFO = 0 and JOB = 'S', H contains the upper
      *           triangular matrix T from the Schur decomposition (the
      *           Schur form). If INFO = 0 and JOB = 'E', the contents of
      *           H are unspecified on exit.  (The output value of H when
      *           INFO.GT.0 is given under the description of INFO below.)
      *
      *           Unlike earlier versions of ZHSEQR, this subroutine may
      *           explicitly H(i,j) = 0 for i.GT.j and j = 1, 2, ... ILO-1
      *           or j = IHI+1, IHI+2, ... N.
      *
      *     LDH   (input) INTEGER
      *           The leading dimension of the array H. LDH .GE. max(1,N).
      *
      *     W        (output) COMPLEX*16 array, dimension (N)
      *           The computed eigenvalues. If JOB = 'S', the eigenvalues are
      *           stored in the same order as on the diagonal of the Schur
      *           form returned in H, with W(i) = H(i,i).
      *
      *     Z     (input/output) COMPLEX*16 array, dimension (LDZ,N)
      *           If COMPZ = 'N', Z is not referenced.
      *           If COMPZ = 'I', on entry Z need not be set and on exit,
      *           if INFO = 0, Z contains the unitary matrix Z of the Schur
      *           vectors of H.  If COMPZ = 'V', on entry Z must contain an
      *           N-by-N matrix Q, which is assumed to be equal to the unit
      *           matrix except for the submatrix Z(ILO:IHI,ILO:IHI). On exit,
      *           if INFO = 0, Z contains Q*Z.
      *           Normally Q is the unitary matrix generated by ZUNGHR
      *           after the call to ZGEHRD which formed the Hessenberg matrix
      *           H. (The output value of Z when INFO.GT.0 is given under
      *           the description of INFO below.)
      *
      *     LDZ   (input) INTEGER
      *           The leading dimension of the array Z.  if COMPZ = 'I' or
      *           COMPZ = 'V', then LDZ.GE.MAX(1,N).  Otherwize, LDZ.GE.1.
      *
      *     WORK  (workspace/output) COMPLEX*16 array, dimension (LWORK)
      *           On exit, if INFO = 0, WORK(1) returns an estimate of
      *           the optimal value for LWORK.
      *
      *     LWORK (input) INTEGER
      *           The dimension of the array WORK.  LWORK .GE. max(1,N)
      *           is sufficient and delivers very good and sometimes
      *           optimal performance.  However, LWORK as large as 11*N
      *           may be required for optimal performance.  A workspace
      *           query is recommended to determine the optimal workspace
      *           size.
      *
      *           If LWORK = -1, then ZHSEQR does a workspace query.
      *           In this case, ZHSEQR checks the input parameters and
      *           estimates the optimal workspace size for the given
      *           values of N, ILO and IHI.  The estimate is returned
      *           in WORK(1).  No error message related to LWORK is
      *           issued by XERBLA.  Neither H nor Z are accessed.
      *
      *
      *     INFO  (output) INTEGER
      *             =  0:  successful exit
      *           .LT. 0:  if INFO = -i, the i-th argument had an illegal
      *                    value
      *           .GT. 0:  if INFO = i, ZHSEQR failed to compute all of
      *                the eigenvalues.  Elements 1:ilo-1 and i+1:n of WR
      *                and WI contain those eigenvalues which have been
      *                successfully computed.  (Failures are rare.)
      *
      *                If INFO .GT. 0 and JOB = 'E', then on exit, the
      *                remaining unconverged eigenvalues are the eigen-
      *                values of the upper Hessenberg matrix rows and
      *                columns ILO through INFO of the final, output
      *                value of H.
      *
      *                If INFO .GT. 0 and JOB   = 'S', then on exit
      *
      *           (*)  (initial value of H)*U  = U*(final value of H)
      *
      *                where U is a unitary matrix.  The final
      *                value of  H is upper Hessenberg and triangular in
      *                rows and columns INFO+1 through IHI.
      *
      *                If INFO .GT. 0 and COMPZ = 'V', then on exit
      *
      *                  (final value of Z)  =  (initial value of Z)*U
      *
      *                where U is the unitary matrix in (*) (regard-
      *                less of the value of JOB.)
      *
      *                If INFO .GT. 0 and COMPZ = 'I', then on exit
      *                      (final value of Z)  = U
      *                where U is the unitary matrix in (*) (regard-
      *                less of the value of JOB.)
      *
      *                If INFO .GT. 0 and COMPZ = 'N', then Z is not
      *                accessed.
      */
      extern void ZHSEQR_F77(const char* JOB, const char* COMPZ, const int* N,
               const int* ILO, const int* IHI, dcomplex* H, const int* LDH, dcomplex* W,
               dcomplex* Z, const int* LDZ, dcomplex* WORK, const int* LWORK, int* INFO);


      /*
      *  Purpose
      *  =======
      *
      *  ZHSEIN uses inverse iteration to find specified right and/or left
      *  eigenvectors of a complex upper Hessenberg matrix H.
      *
      *  The right eigenvector x and the left eigenvector y of the matrix H
      *  corresponding to an eigenvalue w are defined by:
      *
      *               H * x = w * x,     y**h * H = w * y**h
      *
      *  where y**h denotes the conjugate transpose of the vector y.
      *
      *  Arguments
      *  =========
      *
      *  SIDE    (input) CHARACTER*1
      *          = 'R': compute right eigenvectors only;
      *          = 'L': compute left eigenvectors only;
      *          = 'B': compute both right and left eigenvectors.
      *
      *  EIGSRC  (input) CHARACTER*1
      *          Specifies the source of eigenvalues supplied in W:
      *          = 'Q': the eigenvalues were found using ZHSEQR; thus, if
      *                 H has zero subdiagonal elements, and so is
      *                 block-triangular, then the j-th eigenvalue can be
      *                 assumed to be an eigenvalue of the block containing
      *                 the j-th row/column.  This property allows ZHSEIN to
      *                 perform inverse iteration on just one diagonal block.
      *          = 'N': no assumptions are made on the correspondence
      *                 between eigenvalues and diagonal blocks.  In this
      *                 case, ZHSEIN must always perform inverse iteration
      *                 using the whole matrix H.
      *
      *  INITV   (input) CHARACTER*1
      *          = 'N': no initial vectors are supplied;
      *          = 'U': user-supplied initial vectors are stored in the arrays
      *                 VL and/or VR.
      *
      *  SELECT  (input) LOGICAL array, dimension (N)
      *          Specifies the eigenvectors to be computed. To select the
      *          eigenvector corresponding to the eigenvalue W(j),
      *          SELECT(j) must be set to .TRUE..
      *
      *  N       (input) INTEGER
      *          The order of the matrix H.  N >= 0.
      *
      *  H       (input) COMPLEX*16 array, dimension (LDH,N)
      *          The upper Hessenberg matrix H.
      *
      *  LDH     (input) INTEGER
      *          The leading dimension of the array H.  LDH >= max(1,N).
      *
      *  W       (input/output) COMPLEX*16 array, dimension (N)
      *          On entry, the eigenvalues of H.
      *          On exit, the real parts of W may have been altered since
      *          close eigenvalues are perturbed slightly in searching for
      *          independent eigenvectors.
      *
      *  VL      (input/output) COMPLEX*16 array, dimension (LDVL,MM)
      *          On entry, if INITV = 'U' and SIDE = 'L' or 'B', VL must
      *          contain starting vectors for the inverse iteration for the
      *          left eigenvectors; the starting vector for each eigenvector
      *          must be in the same column in which the eigenvector will be
      *          stored.
      *          On exit, if SIDE = 'L' or 'B', the left eigenvectors
      *          specified by SELECT will be stored consecutively in the
      *          columns of VL, in the same order as their eigenvalues.
      *          If SIDE = 'R', VL is not referenced.
      *
      *  LDVL    (input) INTEGER
      *          The leading dimension of the array VL.
      *          LDVL >= max(1,N) if SIDE = 'L' or 'B'; LDVL >= 1 otherwise.
      *
      *  VR      (input/output) COMPLEX*16 array, dimension (LDVR,MM)
      *          On entry, if INITV = 'U' and SIDE = 'R' or 'B', VR must
      *          contain starting vectors for the inverse iteration for the
      *          right eigenvectors; the starting vector for each eigenvector
      *          must be in the same column in which the eigenvector will be
      *          stored.
      *          On exit, if SIDE = 'R' or 'B', the right eigenvectors
      *          specified by SELECT will be stored consecutively in the
      *          columns of VR, in the same order as their eigenvalues.
      *          If SIDE = 'L', VR is not referenced.
      *
      *  LDVR    (input) INTEGER
      *          The leading dimension of the array VR.
      *          LDVR >= max(1,N) if SIDE = 'R' or 'B'; LDVR >= 1 otherwise.
      *
      *  MM      (input) INTEGER
      *          The number of columns in the arrays VL and/or VR. MM >= M.
      *
      *  M       (output) INTEGER
      *          The number of columns in the arrays VL and/or VR required to
      *          store the eigenvectors (= the number of .TRUE. elements in
      *          SELECT).
      *
      *  WORK    (workspace) COMPLEX*16 array, dimension (N*N)
      *
      *  RWORK   (workspace) DOUBLE PRECISION array, dimension (N)
      *
      *  IFAILL  (output) INTEGER array, dimension (MM)
      *          If SIDE = 'L' or 'B', IFAILL(i) = j > 0 if the left
      *          eigenvector in the i-th column of VL (corresponding to the
      *          eigenvalue w(j)) failed to converge; IFAILL(i) = 0 if the
      *          eigenvector converged satisfactorily.
      *          If SIDE = 'R', IFAILL is not referenced.
      *
      *  IFAILR  (output) INTEGER array, dimension (MM)
      *          If SIDE = 'R' or 'B', IFAILR(i) = j > 0 if the right
      *          eigenvector in the i-th column of VR (corresponding to the
      *          eigenvalue w(j)) failed to converge; IFAILR(i) = 0 if the
      *          eigenvector converged satisfactorily.
      *          If SIDE = 'L', IFAILR is not referenced.
      *
      *  INFO    (output) INTEGER
      *          = 0:  successful exit
      *          < 0:  if INFO = -i, the i-th argument had an illegal value
      *          > 0:  if INFO = i, i is the number of eigenvectors which
      *                failed to converge; see IFAILL and IFAILR for further
      *                details.
      */
     extern void ZHSEIN_F77(const char* SIDE, const char* EIGSRC, const char* INITV, const int* SELECT,
              const int* N, const dcomplex* H, const int* LDH, dcomplex* W, dcomplex* VL,
              const int* LDVL, dcomplex* VR, const int* LDVR, const int* MM, int* M,
              dcomplex* WORK, double* RWORK, int* IFAILL, int* IFAILR, int* INFO);

   } /* extern "C" */
   
}

#endif


