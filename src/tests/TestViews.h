#ifndef TESTVIEWS_H
#define TESTVIEWS_H

#include "cppunit/extensions/HelperMacros.h"
#include "math/typedefs.h"

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class TestViews : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE( TestViews );
   CPPUNIT_TEST( complexSize );
   CPPUNIT_TEST( dView );
   CPPUNIT_TEST( cView );
   CPPUNIT_TEST_SUITE_END();
   public:
      TestViews();

      ~TestViews();
      
      void complexSize();
      void dView();
      void cView();
      
   private:
      QDLIB::dVec dvec;
      QDLIB::cVec cvec;

};

#endif
