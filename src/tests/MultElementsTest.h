#ifndef MULTELEMENTSTEST_H
#define MULTELEMENTSTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "math/typedefs.h"

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class MultElementsTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( MultElementsTest );
   CPPUNIT_TEST( NUMERIC_Real2Cplx );
   CPPUNIT_TEST( NUMERIC_Add );
   CPPUNIT_TEST( NUMERIC_Exp );
   CPPUNIT_TEST_SUITE_END();
   public:
      MultElementsTest();
      ~MultElementsTest();
      
      virtual void setUp();
      virtual void tearDown();
      
   protected:
      void NUMERIC_Add();
      void NUMERIC_Exp();
      void NUMERIC_Real2Cplx();

   private:
      QDLIB::cVec cRes, cA, cB, cC;
      QDLIB::dVec dRes, dA, dB, dC;
};

#endif
