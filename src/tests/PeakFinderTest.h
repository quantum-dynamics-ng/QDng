#ifndef PEAKFINDERTEST_H
#define PEAKFINDERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class PeakFinderTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( PeakFinderTest );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      PeakFinderTest();
      
      ~PeakFinderTest();
   protected:
      void NUMERIC_Test();

};

#endif
