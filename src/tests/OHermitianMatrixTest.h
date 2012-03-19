#ifndef OHERMITIANMATRIX_H
#define OHERMITIANMATRIX_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */

class OHermitianMatrixTest : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(OHermitianMatrixTest);
        CPPUNIT_TEST(API_Test);
        CPPUNIT_TEST(NUMERIC_Test);
        CPPUNIT_TEST_SUITE_END();

    protected:
       void API_Test();
       void NUMERIC_Test();
    public:
        OHermitianMatrixTest();

        ~OHermitianMatrixTest();

};

#endif
