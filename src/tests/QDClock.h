#ifndef TQDCLOCK_H
#define TQDCLOCK_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class QDClockTest : public CppUnit::TestFixture
{
      CPPUNIT_TEST_SUITE( QDClockTest );
      CPPUNIT_TEST( API_Test );
      CPPUNIT_TEST_SUITE_END();
   public:
      QDClockTest();
      ~QDClockTest();

   protected:
      void API_Test();
};

#endif
