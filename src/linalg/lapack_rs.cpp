#include "lapack_rs.h"
#include "lapack_double.h"

/**
 * Diagonale a non symetric Matrix.
 * 
 * Complex Eigenvalues are Possible. The Imaginary parts are stored in 
 * 
 * \param in the input matrix (content will be destroyed)
 * \param eigenvectors Matrix containing the eigenvectors
 * \param eigenvalues Vector containing the eigenvalues of the matrix
 * \param eigenvalues_im Vector containing imaginary parts of the eigenvalues
 * 
 * \return LAPACK error codes. Zero = no error
 */
int lapack_rs_d::geev(dMatrix &in, dMatrix &eigenvectors, dVector &eigenvalues, dVector &eigenvalues_im)
{
  int check;

  if ( in.num_rows()!= in.num_cols() || (eigenvectors.num_rows() != eigenvectors.num_cols()) ||
        eigenvalues.size() < in.num_cols() || eigenvalues.size() < 1 )
    return -1;
  
  int lwork = in.num_rows()*4;
  double work[in.size()*4];
  double RVec[in.size()];

  int k=in.num_rows();
  char ljob='V';
  char rjob='N';

  dMatrix trans(eigenvectors);
  dgeev_(&ljob, &rjob, &k, in.begin(), &k, eigenvalues.begin(), eigenvalues_im.begin(), trans.begin(),
        &k, RVec, &k, work, &lwork, &check);
  
  eigenvectors = TNT::transpose(trans);
    
  return check;
}

/**
 * Calculate the inverse Matrix.
 * 
 * \param in the input matrix (the inverse after operation)
 * 
 * \return LAPACK error codes. Zero = no error
 */
int lapack_rs_d::inverse(dMatrix &in)
{
  int check;

  int lwork = in.num_rows()*4;
  double work[in.size()*4];
  
  int k=in.num_rows();

  int pivot_indeces[in.num_rows()];

  dgetrf_(&k, &k, in.begin(), &k, pivot_indeces, &check);

  if (check == 0)
    dgetri_(&k, in.begin() , &k, pivot_indeces, work, &lwork, &check);
  
  return check;
}

