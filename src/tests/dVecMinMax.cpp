#include "dVecMinMax.h"

#include "cppunit/portability/FloatingPoint.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestMinMax);

TestMinMax::TestMinMax() : vec(NULL)
{
}

TestMinMax::~TestMinMax()
{
   if (vec != NULL) delete vec;
}

void TestMinMax::setUp()
{
   double d[] = {10.0, 15.1, -3.50001, -2.2023, 1.000, 0.0, -3.500000, 15.10000001};
   vec = new QDLIB::dVec(8, d);
}


void TestMinMax::tearDown(){}

void TestMinMax::MinCheck()
{
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.50001 , QDLIB::VecMin(*vec) , .000001);
}


void TestMinMax::MaxCheck()
{
   CPPUNIT_ASSERT_DOUBLES_EQUAL(15.10000001 , QDLIB::VecMax(*vec) , .000000001);
}
