#ifndef TESTDVECMINMAX_H
#define TESTDVECMINMAX_H

#include "cppunit/TestFixture.h"
#include "math/typedefs.h"
#include "cppunit/extensions/HelperMacros.h"


class TestMinMax : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE( TestMinMax );
   CPPUNIT_TEST( MinCheck );
   CPPUNIT_TEST( MaxCheck );
   CPPUNIT_TEST_SUITE_END();

   public:
      TestMinMax();
      virtual ~TestMinMax();

      virtual void setUp();
      virtual void tearDown(); 
      
   protected:
      void MinCheck();
      void MaxCheck();
   private:
      QDLIB::dVec *vec;
};

#endif 
