#ifndef TESTMATHFUNCTIONS_H
#define TESTMATHFUNCTIONS_H

#include "cppunit/TestFixture.h"
#include "math/typedefs.h"
#include "cppunit/extensions/HelperMacros.h"

/**
 *    Test for mathematic functions
 * 
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class TestMathFunctions : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE( TestMathFunctions );
   CPPUNIT_TEST( FacultyCheck );
   CPPUNIT_TEST( BesselCheck );
   CPPUNIT_TEST_SUITE_END();
   
   public:
      TestMathFunctions();
   
      ~TestMathFunctions();
      
      virtual void setUp();
      virtual void tearDown(); 
      
   protected:
      void FacultyCheck();
      void BesselCheck();

};

#endif
