#ifndef COMPLEXNUMBERS_H
#define COMPLEXNUMBERS_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "math/dcomplex.h"


/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class ComplexNumbers : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( ComplexNumbers );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   
   public:
      ComplexNumbers();
      ~ComplexNumbers();
      
   protected:
      void API_Test();
      void NUMERIC_Test();

};

#endif
