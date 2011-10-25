/*
 * MemoryTest.h
 *
 *  Created on: 23.10.2011
 *      Author: markus
 */

#ifndef MEMORYTEST_H_
#define MEMORYTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class MemoryTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( MemoryTest );
   CPPUNIT_TEST( CORE_Test );
   CPPUNIT_TEST( COPY_Test );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      MemoryTest();
      ~MemoryTest();

   protected:
      void CORE_Test();
      void COPY_Test();
      void API_Test();
};

#endif /* MEMORYTEST_H_ */
