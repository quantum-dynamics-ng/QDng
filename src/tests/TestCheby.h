#ifndef TESTCHEBY_H
#define TESTCHEBY_H

#include "cppunit/extensions/HelperMacros.h"
#include "qdlib/OCheby.h"
#include "qdlib/OHermitianMatrix.h"
#include "qdlib/WFLevel.h"

/**
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class TestCheby : public CPPUNIT_NS::TestFixture {
   CPPUNIT_TEST_SUITE( TestCheby );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      TestCheby();
      ~TestCheby();

      void setUp();
      void tearDown();

      void API_Test();
      void NUMERIC_Test();
   private:
      QDLIB::OCheby *U;
      QDLIB::Operator *H;
      QDLIB::WaveFunction *psi0, *psi1;
      QDLIB::QDClock TheClock;
};

#endif



