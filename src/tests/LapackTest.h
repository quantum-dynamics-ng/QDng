#ifndef LAPACKTEST_H
#define LAPACKTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "linalg/LapackDiag.h"

/**
 *
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class LapackTest : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(LapackTest);
        CPPUNIT_TEST(NUMERIC_Test_double);
        CPPUNIT_TEST(NUMERIC_Test_complex);
        CPPUNIT_TEST_SUITE_END();
   protected:
      void NUMERIC_Test_double();
      void NUMERIC_Test_complex();
    public:
        LapackTest();

        ~LapackTest();

};

#endif
