#include "Vector.h"
#include "math/typedefs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VectorTest);

#define VEC_TEST_SIZE 3
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
   
   dVec d2(VEC_TEST_SIZE, true);
   CPPUNIT_ASSERT(d2.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d2.strides() == 1);
   CPPUNIT_ASSERT(d2.sizeBytes() == VEC_TEST_SIZE * sizeof(double));
   
   dVec d3(VEC_TEST_SIZE, true, 5);
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
   

}

void VectorTest::API_Test_MultiStride()
{
   dVec d1;
   
   /* resizing & stride handling */
   d1.newsize(VEC_TEST_SIZE, VEC_TEST_STRIDES);
   CPPUNIT_ASSERT(d1.size() == VEC_TEST_SIZE);
   CPPUNIT_ASSERT(d1.strides() == VEC_TEST_STRIDES);
   
   
}


