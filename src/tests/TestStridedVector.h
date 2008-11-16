#ifndef TESTSTRIDEDVECTOR_H
#define TESTSTRIDEDVECTOR_H


#include <cppunit/extensions/HelperMacros.h>

#include "math/typedefs.h"
/**
	@author Markus Kowalewski
*/
class TestStridedVector : public CPPUNIT_NS::TestFixture
{
      CPPUNIT_TEST_SUITE( TestStridedVector );
      CPPUNIT_TEST( InitVector );
      CPPUNIT_TEST( Copy );
      CPPUNIT_TEST( MathOperators );
      CPPUNIT_TEST( Swap );
      CPPUNIT_TEST_SUITE_END();
   public:
      TestStridedVector();
   
      ~TestStridedVector();
      
      void InitVector();
      void Copy();
      void MathOperators();
      void Swap();
};

#endif
