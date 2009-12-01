#ifndef WFGRIDCARTESIANTEST_H
#define WFGRIDCARTESIANTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/WFGridCartesian.h"

using namespace QDLIB;

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class WFGridCartesianTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( WFGridCartesianTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      WFGridCartesianTest();
   
      ~WFGridCartesianTest();

      void setUp();
      void tearDown();
      
   protected:
      void API_Test();
      void NUMERIC_Test();
      
   private:
      ParamContainer p;
      WFGridCartesian *wf;
};

#endif
