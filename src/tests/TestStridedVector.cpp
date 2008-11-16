#include "TestStridedVector.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestStridedVector);
using namespace QDLIB;

TestStridedVector::TestStridedVector()
 : CPPUNIT_NS::TestFixture()
{
}


TestStridedVector::~TestStridedVector()
{
}

void TestStridedVector::InitVector()
{
   dVec vec;
   
   /* Single strided test */
   vec.newsize(4);
   CPPUNIT_ASSERT(vec.size() == 4);
   CPPUNIT_ASSERT(vec.lsize() == 4);
   CPPUNIT_ASSERT(vec.start() == 0);
   CPPUNIT_ASSERT(vec.stop() == 4);
   CPPUNIT_ASSERT(vec.strides() == 1);
   
   vec[0] = 1;
   vec[1] = 2;
   vec[2] = 3;
   vec[3] = 4;
   
   CPPUNIT_ASSERT(vec[0] == 1);
   CPPUNIT_ASSERT(vec[1] == 2);
   CPPUNIT_ASSERT(vec[2] == 3);
   CPPUNIT_ASSERT(vec[3] == 4);
   
   /* Two strides */
   vec.newsize(4,2);
   
   CPPUNIT_ASSERT(vec.size() == 4);
   CPPUNIT_ASSERT(vec.lsize() == 2);
   CPPUNIT_ASSERT(vec.start() == 0);
   CPPUNIT_ASSERT(vec.stop() == 4);
   CPPUNIT_ASSERT(vec.strides() == 2);
   
   vec[0] = 4;
   vec[1] = 3;
   vec[2] = 2;
   vec[3] = 1;
   
   CPPUNIT_ASSERT(vec[0] == 4);
   CPPUNIT_ASSERT(vec[1] == 3);
   CPPUNIT_ASSERT(vec[2] == 2);
   CPPUNIT_ASSERT(vec[3] == 1);

   /* Three strides */
   vec.newsize(12,4);
   vec[4] = 10;
   CPPUNIT_ASSERT(vec[4] == 10);
   vec[11] = 20;
   CPPUNIT_ASSERT(vec[11] == 20);
   
   double **a;
   a = vec.begin();
   CPPUNIT_ASSERT(a[1][1] == 10);
   CPPUNIT_ASSERT(a[3][2] == 20);
   
   double *b;
   b = vec.begin(1);
   CPPUNIT_ASSERT(b[1] == 10);
   b = vec.begin(3);
   CPPUNIT_ASSERT(b[2] == 20);
}

void TestStridedVector::Copy()
{
   dVec A, B;
   
   A.newsize(16,4);
   A = 5.0;
   
   
   B = A;
   
   CPPUNIT_ASSERT(A[0] == B[0]);
   CPPUNIT_ASSERT(A[15] == B[15]);
   
   B = 0;
   
   CPPUNIT_ASSERT(B[0] == 0);
   CPPUNIT_ASSERT(B[15] == 0);
   
   B.newsize(8,2);
   
   B = 0;
   B[0] = B[1] = B[2] = B[3] = 1;
   
   CPPUNIT_ASSERT( A.StrideCopy(B, 0, 1) ) ;
   CPPUNIT_ASSERT( A[4] == A[5] == A[6] == A[7] == 1);
   
   

}

void TestStridedVector::MathOperators()
{
   dVec A, B, C;
   
   A.newsize(24,8);
   B.newsize(24,8);
   
   A = 7.0;
   B = 3.0;
   
   C = A + B;
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, C[0], 1e-16);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, C[23], 1e-16);
   
   C = A * B; /* Scalar product */
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 504.0, C[0], 1e-16);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 504.0, C[23], 1e-16);
   
   C = elementwise_mult(A,B);
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 21.0, C[0], 1e-16);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 21.0, C[23], 1e-16);
   
}

void TestStridedVector::Swap()
{
   dVec A, B;
   
   A.newsize(24,3);
   B.newsize(24,3);
   
   A = 9.;
   B = 11.;
   
   A.swap(B);
	 
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 11.0, A[0], 1e-18);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 11.0, A[23], 1e-18);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, B[0], 1e-18);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.0, B[23], 1e-18);
  
}


