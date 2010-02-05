#include "MatrixTest.h"
#include "defs.h"

// using namespace QDLIB;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(MatrixTest);

MatrixTest::MatrixTest()
 : CppUnit::TestFixture()
{
}


MatrixTest::~MatrixTest()
{
}

void MatrixTest::API_Test()
{
   Matrix<int> M, R;
   Vector<int> *V = NULL;
   
   /* Check init & Access */
   M.newsize(3,4);
   CPPUNIT_ASSERT(M.rows() == 3);
   CPPUNIT_ASSERT(M.cols() == 4);
   
   CPPUNIT_ASSERT(M.sizeBytes() == 3*4*sizeof(int));
   CPPUNIT_ASSERT(M.strides() == 1);
   
   /* Single Elements */
   for (int i=0; i < 4; i++){
      for (int j=0; j < 3; j++){
         M(j,i) = i * M.rows() + j;
         CPPUNIT_ASSERT( M(j,i) ==  i * M.rows() + j) ;
      }
   }
   
   /* Check internal storage */
   int *mat;
   for (int i=0; i < M.cols(); i++){
      mat = M.begin(i);
      V = M.coloumn(i);
      for (int j=0; j < M.rows(); j++){
         CPPUNIT_ASSERT(mat[j] == i * M.rows() + j);
         CPPUNIT_ASSERT((*V)[j] == i * M.rows() + j);
      }
   }
   
   mat = M.begin();
   for (int i = 0; i < M.rows()*M.cols(); i++){
      CPPUNIT_ASSERT(mat[i] == i);
   }

   
   
         
   /* Set values */
   M = 2;
   for (int i=0; i < 3; i++)
      for (int j=0; j < 4; j++)
         CPPUNIT_ASSERT(M(i,j) == 2);
   
   /* Scalar mult. */
   M *= 3;
   for (int i=0; i < M.rows(); i++)
      for (int j=0; j < M.cols(); j++)
         CPPUNIT_ASSERT(M(i,j) == 2*3);
   
   /* Copy */
   R = M;
   CPPUNIT_ASSERT(M.rows() == R.rows() && M.cols() == R.cols());
   
   for (int i=0; i < R.rows(); i++)
      for (int j=0; j < R.cols(); j++)
         CPPUNIT_ASSERT(R(i,j) == 2*3);
   
   /* Resize */
   M.newsize(4,4);
   CPPUNIT_ASSERT(M.rows() == 4  && M.cols() == 4);

   /* Diagonal access */
   V->newsize(4);
   *V = -1;
   M.diag(V);
   
   for (int i=0; i < M.rows(); i++)
      CPPUNIT_ASSERT( (*V)[i] == M(i,i));
   
   M = 2;
   V = M.diag();
   
   for (int i=0; i < M.rows(); i++)
      CPPUNIT_ASSERT( (*V)[i] == M(i,i));
   
   
   int n=0;
   for (int i=0; i < M.rows(); i++){
      for (int j=0; j < M.cols(); j++){
         M(i,j) = n;
         n++;
      }
   }
   
   R = M;
   M.transpose();
   
   n=0;
   for (int i=0; i < R.rows(); i++){
      for (int j=0; j < R.cols(); j++){
         CPPUNIT_ASSERT(M(j,i) == R(i,j));
      }
   }
   

}

void MatrixTest::NUMERIC_Test()
{
   Vector<int> V, R;
   Matrix<int> M;
   
   
   M.newsize(4,4);
   V.newsize(4);
   
   int n=0;
   for (int i=0; i < M.rows(); i++){
      V[i] = i;
      for (int j=0; j < M.cols(); j++){
         M(i,j) = n;
         n++;
      }
   }
  
   MatVecMult(&R, &M, &V);
  
  
   CPPUNIT_ASSERT( R[0] == 14 );
   CPPUNIT_ASSERT( R[1] == 38 );
   CPPUNIT_ASSERT( R[2] == 62 );
   CPPUNIT_ASSERT( R[3] == 86 );
      
   MatVecMult(&R, &M, &V, true);
 
   CPPUNIT_ASSERT( R[0] == 56 );
   CPPUNIT_ASSERT( R[1] == 62 );
   CPPUNIT_ASSERT( R[2] == 68 );
   CPPUNIT_ASSERT( R[3] == 74 );
   
   
   cVec cV, cR;
   cMat cM;
   
   cM.newsize(2,2);
   cV.newsize(2);
   
   cM(0,0)=dcomplex(1,2);
   cM(1,1)=dcomplex(5,6);
   cM(0,1)=dcomplex(3,4);
   cM(1,0)=dcomplex(3,-4);
   
   cV[0]=1;
   cV[1]=2;
   
   MatVecMult(&cR, &cM, &cV);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[0].real(), 7, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[0].imag(), 10, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[1].real(), 13, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[1].imag(), 8, TIGHT_EPS);
   
   MatVecMult(&cR, &cM, &cV,true,true);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[0].real(), 7, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[0].imag(), 6, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[1].real(), 13, TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( cR[1].imag(), -16, TIGHT_EPS);

}




