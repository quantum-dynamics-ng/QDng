#include "Vector.h"
#include "math/typedefs.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VectorTest);

#define VEC_TEST_SIZE 12
#define VEC_TEST_STRIDES 4

using namespace QDLIB;

VectorTest::VectorTest()
 : CppUnit::TestFixture()
{
}


VectorTest::~VectorTest()
{
}

void VectorTest::API_Test_SingleStride()
{
   /* Constructors */
   dVec d1(VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d1.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d1.strides() == 1);
   CPPUNIT_ASSERT(d1.sizeBytes() == VEC_TEST_SIZE * sizeof(double));
   
   dVec d2(VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d2.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d2.strides() == 1);
   CPPUNIT_ASSERT(d2.sizeBytes() == VEC_TEST_SIZE * sizeof(double));
   
   dVec d3(VEC_TEST_SIZE);
   d3 = 5.;
   CPPUNIT_ASSERT(d3.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d3.strides() == 1);
   CPPUNIT_ASSERT(d3.sizeBytes() == VEC_TEST_SIZE * sizeof(double));
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d3[i] == 5);
   }
   
   /* Accessor test */
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      d1[i] = i;
      CPPUNIT_ASSERT(d1[i] == i);
   }
  
   /* Copy methods & xchange */
   d1 = d3;
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 5);
   }

   d1 = 3;
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 3);
   }

   
   d2.FastCopy(d3);
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d2[i] == 5);
   }
   
   d1=1;
   d2=2;
   d1.swap(d2);
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 2);
      CPPUNIT_ASSERT(d2[i] == 1);
   }
   
   double *d = d1.begin(0);
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d[i] == 2);
   }
   
   
   
   /* Stride methods in single stride mode */
   CPPUNIT_ASSERT(d1.lsize() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d1.start() == 0);
   CPPUNIT_ASSERT(d1.stop() == VEC_TEST_SIZE);
   
   d1=1;
   d2=2;
   CPPUNIT_ASSERT(d1.StrideCopy(d2,0,0));
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 2);
   }

   double **m = d1.begin();
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(m[0][i] == 2);
   }
   

   /* Storage test */
   CPPUNIT_ASSERT( d1.begin(0) != NULL);
   d1.RetireStorage();
   CPPUNIT_ASSERT( d1.begin(0) == NULL);
   d1.ReaquireStorage();
   CPPUNIT_ASSERT( d1.begin(0) != NULL);
}

void VectorTest::API_Test_MultiStride()
{
   dVec d1,d2;
   
   /* resizing & stride handling - continous strides */
   d1.newsize(VEC_TEST_SIZE, VEC_TEST_STRIDES);
   CPPUNIT_ASSERT(d1.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d1.lsize() == VEC_TEST_SIZE/VEC_TEST_STRIDES);
   CPPUNIT_ASSERT(d1.strides() == VEC_TEST_STRIDES);
   
   /* Accessor test */
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      d1[i] = double(i);
      CPPUNIT_ASSERT(d1[i] == double(i));
   }
   
   double *dp,**v;
   lint k=0;
   v = d1.begin();
   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      dp = d1.begin(s);
      for (lint j=0; j < d1.lsize(); j++){
	 CPPUNIT_ASSERT(dp[j] == double(k));
	 CPPUNIT_ASSERT(v[s][j] == double(k));
	 k++;
      }
   }
   
   /* Copy methods & xchange */
   d2 = d1;
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d2[i] == double(i));
   }

   d1 = 3;
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 3);
   }

   d2.FastCopy(d1);
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d2[i] == 3);
   }
   
   d1=1;
   d2=2;
   d1.swap(d2);
   for (lint i=0; i < VEC_TEST_SIZE; i++){
      CPPUNIT_ASSERT(d1[i] == 2);
      CPPUNIT_ASSERT(d2[i] == 1);
   }
   
   /* Stride Copy */
   dVec da[VEC_TEST_STRIDES];
   
   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      da[s].newsize(VEC_TEST_SIZE/VEC_TEST_STRIDES);
      da[s] = double(s);
      d1.StrideCopy(da[s], 0, s);
   }
   
   v = d1.begin();
   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      for (lint j=0; j < d1.lsize(); j++){
	 CPPUNIT_ASSERT(v[s][j] == double(s));
      }
   }
   
   /* Stride Refs */
   class dVecStrides : public dVec {
      public:
         dVecStrides(lint n, lint s) : dVec(n,s) {}
	 bool SRef (dVec &vec, lint source, lint dest)
	 {
	    return StrideRef(vec, source, dest);
	 }
   };

   dVecStrides ds(0,VEC_TEST_STRIDES);
   
   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      CPPUNIT_ASSERT (ds.SRef(da[s], 0, s));
   }

   v = ds.begin();
   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      for (lint j=0; j < d1.lsize(); j++){
	 CPPUNIT_ASSERT(v[s][j] == double(s));
      }
   }

   /* Storage test */
   for (lint s=0; s < VEC_TEST_STRIDES; s++)
      CPPUNIT_ASSERT( ds.begin(s) != NULL);

   ds.RetireStorage();

   for (lint s=0; s < VEC_TEST_STRIDES; s++){
      CPPUNIT_ASSERT( ds.begin(s) == NULL);
   }

   ds.ReaquireStorage();
   for (lint s=0; s < VEC_TEST_STRIDES; s++)
      CPPUNIT_ASSERT( ds.begin(s) != NULL);

}

void VectorTest::NUMERIC_Test_Strides()
{
   dVec A, B, C;
   
   A.newsize(24,8);
   B.newsize(24,8);
   
   A = 7.0;
   B = 3.0;
   
   C = A + B;
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, C[0], LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, C[23], LOOSE_EPS);
   
   C = A * B; /* Scalar product */
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 504.0, C[0], LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 504.0, C[23], LOOSE_EPS);
   
   C = elementwise_mult(A,B);
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 21.0, C[0], LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 21.0, C[23], LOOSE_EPS);
   

}


