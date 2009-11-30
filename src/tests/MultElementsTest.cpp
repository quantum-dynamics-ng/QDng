#include "MultElementsTest.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MultElementsTest);

#define VEC_TEST_SIZE 12
#define VEC_TEST_STRIDES 4

using namespace QDLIB;
MultElementsTest::MultElementsTest()
 : CppUnit::TestFixture()
{
}


MultElementsTest::~MultElementsTest()
{
}

void MultElementsTest::setUp()
{
   cA.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   cB.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   cC.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   dA.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   dB.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   dC.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   
   cRes.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   dRes.newsize(VEC_TEST_SIZE,VEC_TEST_STRIDES);
   
   for(lint i=cA.start(); i < cA.stop(); i++){
      cA[i] = dcomplex(double(i),double(i) * 1.5);
      cB[i] = dcomplex(double(VEC_TEST_SIZE+i),double(VEC_TEST_SIZE+i) * 1.5);
      cC[i] = dcomplex(double(2*VEC_TEST_SIZE+i),double(2*VEC_TEST_SIZE+i) * 1.5);
      dA[i] = double(i)*1.1;
      dB[i] = double(VEC_TEST_SIZE+i)*1.1;
      dC[i] = double(2*VEC_TEST_SIZE+i)*1.1;
   }
}

void MultElementsTest::tearDown()
{
}




void MultElementsTest::NUMERIC_Add()
{
   dcomplex c;
   double d;
   
   cRes = cA;
   /* void AddElements(cVec *A, cVec *B)*/
   AddElements(&cRes, &cB);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cA[i] + cB[i];
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
   }
                                   
   cRes = cA;
   d=3.5;
   /* void AddElements(cVec *A, cVec *B, double d)*/
   AddElements(&cRes, &cB, d);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cA[i] + d * cB[i];
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
   }
}

void MultElementsTest::NUMERIC_Exp()
{
   dcomplex c,c1;
   
   //     inline void ExpElements(cVec *A, cVec *B, dcomplex c)
   c1=dcomplex(4.4,5.5);
   ExpElements(&cRes, &cB, c1);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cexp(c1 * cB[i]);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
   }   
   
//     inline void ExpElements(cVec *A, dVec *B, dcomplex c) 
   c1=dcomplex(0,5.5); /* pure imag exponenent => keep abs-value*/
   ExpElements(&cRes, &dB, c1);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cexp(c1 * dB[i]);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1, cabs(cRes[i]), LOOSE_EPS);
   }   
   
   c1=dcomplex(0.1,0.2); /* general case */
   ExpElements(&cRes, &dB, c1);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cexp(c1 * dB[i]);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
   }

}


void MultElementsTest::NUMERIC_Real2Cplx()
{
   dcomplex c,c1;
   double d;
   
//    inline void MultElementsComplexEq(cVec *C ,cVec *A, dVec *B, double d)
   d=3.3;
   MultElementsComplexEq(&cRes, &cA, &dB, d);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cA[i] * dcomplex(0,dB[i]) * d;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), LOOSE_EPS);
   } 
//    inline void MultElementsComplex(cVec *C ,cVec *A, dVec *B, double d)
   d=3.3;
   cRes = cA;
   MultElementsComplex(&cRes, &cB, &dC, d);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cA[i] * cB[i] * dcomplex(0,dC[i]) * d;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), 1e-10);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), 1e-10);
   } 
   
   
//    inline void MultElementsComplex(cVec *A, dVec *B, double d)
   d=3.1;
   cRes = cA;
   MultElementsComplex(&cRes, &dB, d);
   for(lint i=cA.start(); i < cA.stop(); i++){
      c = cA[i] * dcomplex(0,dB[i]) * d;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.real(), cRes[i].real(), 1e-10);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(c.imag(), cRes[i].imag(), 1e-10);
   } 

}

