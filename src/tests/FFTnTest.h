/*
 * FFTnTest.h
 *
 *  Created on: 31.12.2010
 *      Author: Markus Kowalewski
 */

#ifndef FFTNTEST_H_
#define FFTNTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


/**
 *
 *      @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class FFTnTest : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(FFTnTest);
        CPPUNIT_TEST(NUMERIC_Test);
        CPPUNIT_TEST_SUITE_END();
   protected:
      void NUMERIC_Test();
    public:
      FFTnTest();

        ~FFTnTest();

};


#endif /* FFTNTEST_H_ */
