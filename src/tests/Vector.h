#ifndef TVECTOR_H
#define tVECTOR_H

#include "TestFixture.h"
#include <cppunit/extensions/HelperMacros.h>

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class VectorTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( VectorTest );
   CPPUNIT_TEST( API_Test_SingleStride );
   CPPUNIT_TEST( API_Test_MultiStride );
   CPPUNIT_TEST( NUMERIC_Test_Strides );
   CPPUNIT_TEST_SUITE_END();
   
   public:
      VectorTest();
   
      ~VectorTest();
   protected:
      void API_Test_SingleStride();
      void API_Test_MultiStride();
      void NUMERIC_Test_Strides();

};

#endif
