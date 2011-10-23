#ifndef TMPFILETEST_H
#define TMPFILETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


/**
 *
 *      @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class TmpFileTest : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(TmpFileTest);
        CPPUNIT_TEST(API_Test);
        CPPUNIT_TEST_SUITE_END();
   protected:
      void API_Test();
    public:
        TmpFileTest();

        ~TmpFileTest();

};

#endif
