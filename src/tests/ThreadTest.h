/*
 * ThreadTest.h
 *
 *  Created on: 01.11.2011
 *      Author: markus
 */

#ifndef THREADTEST_H_
#define THREADTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ThreadTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ThreadTest );
   CPPUNIT_TEST( CORE_Test );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      ThreadTest();
      virtual ~ThreadTest();

   protected:
      void CORE_Test();
      void API_Test();
};

#endif /* THREADTEST_H_ */
