#ifndef TESTGETOPT_H
#define TESTGETOPT_H

#include "tools/Getopt.h"

#include <cppunit/extensions/HelperMacros.h>


/**
 @author Markus Kowalewski
*/

class TestGetopt: public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE( TestGetopt );
   CPPUNIT_TEST( Init );
   CPPUNIT_TEST( ShowHelp );
   CPPUNIT_TEST( CheckArgs );
   CPPUNIT_TEST( Values );
   CPPUNIT_TEST_SUITE_END();
   public:
      void Init();
      void ShowHelp();
      void CheckArgs();
      void Values();
	    
      TestGetopt();
      ~TestGetopt();

};

#endif
