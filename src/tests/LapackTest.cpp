#include "LapackTest.h"

#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LapackTest);

LapackTest::LapackTest()
 : CppUnit::TestFixture()
{
}


LapackTest::~LapackTest()
{
}




void LapackTest::NUMERIC_Test_double()
{
   dVec eVal;
   dMat M,eVec;
   
   /* check Matrix diagonalazation */
   M.newsize(2,2);
   
   M(0,0)=0;
   M(1,1)=0;
   M(1,0) = M(0,1) = 2;
   
   eVal.newsize(2);
   
   eVec = M;
  
   //cout << LAPACK::FullDiagHermitian(&eVec, &eVal) << endl;
   CPPUNIT_ASSERT(LAPACK::FullDiagHermitian(&eVec, &eVal) == 0);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvalue", -2, eVal[0], TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvalue",  2, eVal[1], TIGHT_EPS);
   
   /* Orthogonal & normalized eigenvectors? */
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Orthogonal", 0, eVec(0,0) * eVec(0,1) + eVec(1,0) * eVec(1,1), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvector norm", 1, eVec(0,0) * eVec(0,0) + eVec(1,0) * eVec(1,0), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvector norm", 1, eVec(0,1) * eVec(0,1) + eVec(1,1) * eVec(1,1), LOOSE_EPS);
}

void LapackTest::NUMERIC_Test_complex()
{
   dVec eVal;
   cMat M,eVec;
   
   /* check Matrix diagonalazation */
   M.newsize(2,2);
   
   M(0,0)=0;
   M(1,1)=0;
   M(0,1)=dcomplex(0,+2);
   M(1,0)=dcomplex(0,-2);;
   
   eVal.newsize(2);
   
   eVec = M;
  
   //cout << LAPACK::FullDiagHermitian(&eVec, &eVal) << endl;
   CPPUNIT_ASSERT(LAPACK::FullDiagHermitian(&eVec, &eVal) == 0);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvalue", -2, eVal[0], TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvalue",  2, eVal[1], TIGHT_EPS);
   
   /* Orthogonal & normalized eigenvectors? */
   dcomplex c;
   c = conj(eVec(0,0)) * eVec(0,1) + conj(eVec(1,0)) * eVec(1,1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Orthogonal", 0, cabs(c), LOOSE_EPS);
   
   c = conj(eVec(0,0)) * eVec(0,0) + conj(eVec(1,0)) * eVec(1,0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvector norm", 1, cabs(c), LOOSE_EPS);
   
   c = conj(eVec(0,1)) * eVec(0,1) + conj(eVec(1,1)) * eVec(1,1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Eigenvector norm", 1, cabs(c), LOOSE_EPS);
}

