#ifndef TGRIDSYSTEM_H
#define TGRIDSYSTEM_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "qdlib/GridSystem.h"

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class GridSystemTest : public CppUnit::TestFixture
{
      CPPUNIT_TEST_SUITE( GridSystemTest );
      CPPUNIT_TEST( API_Test );
      CPPUNIT_TEST_SUITE_END();
      
   public:
      GridSystemTest();
   
      ~GridSystemTest();

   protected:
      void API_Test();
      
};

#endif
