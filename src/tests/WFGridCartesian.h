#ifndef TWFGRIDCARTESIAN_H
#define TWFGRIDCARTESIAN_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class WFGridCartesianTest : public CppUnit::TestFixture
{   
   CPPUNIT_TEST_SUITE( WFGridCartesianTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      WFGridCartesianTest();
   
      ~WFGridCartesianTest();
      
   protected:
      void API_Test();

};

#endif
