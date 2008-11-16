#ifndef TESTLOGGER_H
#define TESTLOGGER_H

#include "sys/Logger.h"
#include "cppunit/extensions/HelperMacros.h"

/**
 @author Markus Kowalewski
*/

class TestLogger: public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE( TestLogger );
   CPPUNIT_TEST( RunLogger );
   CPPUNIT_TEST_SUITE_END();
   
   public:
      TestLogger();
      ~TestLogger();

      void RunLogger();
      
};

#endif
