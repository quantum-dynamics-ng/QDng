#ifndef MATRIXTEST_H
#define MATRIXTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "math/typedefs.h"

using namespace QDLIB;
/**
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class MatrixTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(MatrixTest);
   CPPUNIT_TEST(API_Test);
   CPPUNIT_TEST(NUMERIC_Test);
   CPPUNIT_TEST_SUITE_END();
   protected:
      void API_Test();
      void NUMERIC_Test();
    public:
        MatrixTest();
        ~MatrixTest();

};

#endif
