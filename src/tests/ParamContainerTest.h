/*
 * ParamContainerTest.h
 *
 *  Created on: Jan 16, 2013
 *      Author: markus
 */

#ifndef PARAMCONTAINERTEST_H_
#define PARAMCONTAINERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tools/ParamContainer.h"

using namespace QDLIB;

class ParamContainerTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ParamContainerTest );
   CPPUNIT_TEST( IO_Test );
   CPPUNIT_TEST( Type_Test );
   CPPUNIT_TEST_SUITE_END();

   protected:
      void IO_Test();
      void Type_Test();

   public:
      ParamContainerTest();
      virtual ~ParamContainerTest();

      void setUp();

   private:
      ParamContainer p;
};

#endif /* PARAMCONTAINERTEST_H_ */
