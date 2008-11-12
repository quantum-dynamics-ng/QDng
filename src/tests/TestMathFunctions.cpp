#include "TestMathFunctions.h"
#include "cppunit/portability/FloatingPoint.h"

#include "math/typedefs.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestMathFunctions);

TestMathFunctions::TestMathFunctions()
 : CPPUNIT_NS::TestFixture()
{
}


TestMathFunctions::~TestMathFunctions()
{
}

void TestMathFunctions::setUp( )
{
}

void TestMathFunctions::tearDown( )
{
}

/**
 * Check the faculty function.
 * 
 * Values up to 100! should give a non NaN result.
 */
void TestMathFunctions::FacultyCheck( )
{
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, QDLIB::Faculty(0), 0.0000000001);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, QDLIB::Faculty(1), 0.0000000001);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(120.0, QDLIB::Faculty(5), 0.0000000001);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(2.43290200817664e18, QDLIB::Faculty(20), 1e10);
   CPPUNIT_ASSERT( !CPPUNIT_NS::floatingPointIsUnordered( QDLIB::Faculty(100) ) );
}

/**
 * Check the Bessel function and its numerical stability.
 * 
 * In the stable range it is between -1 and 1.
 * For large dt it must be stable at least up an argument of 10^6.
 */
void TestMathFunctions::BesselCheck( )
{

   QDLIB::dVec coeff;
   
   QDLIB::BesselJ0(1000000,100000, coeff);
/*   double bessel=0;
   for (int i=0; i < coeff.size(); i++){
      bessel += coeff[i];
   }
   cout << bessel << endl;
   cout << coeff << endl;*/
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, coeff[coeff.size()-1], 1e-30 );
   CPPUNIT_ASSERT( QDLIB::BesselJ0(40,100, coeff) < 10);
   
}



