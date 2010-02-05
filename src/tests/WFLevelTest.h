#ifndef WFLEVELTEST_H
#define WFLEVELTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/WFLevel.h"
/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/

class WFLevelTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( WFLevelTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
    public:
        WFLevelTest();

        ~WFLevelTest();
   protected:
      void API_Test();
      void NUMERIC_Test();
};

#endif
