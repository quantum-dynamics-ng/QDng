#ifndef VECTOR_H
#define VECTOR_H

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
   CPPUNIT_TEST_SUITE_END();
   
   public:
      VectorTest();
   
      ~VectorTest();
   protected:
      void API_Test_SingleStride();
      void API_Test_MultiStride();

};

#endif
