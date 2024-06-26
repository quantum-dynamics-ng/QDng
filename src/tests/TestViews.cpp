#include "TestViews.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestViews);


using namespace QDLIB;

TestViews::TestViews()
{}


TestViews::~TestViews()
{}

void TestViews::complexSize( )
{
   CPPUNIT_ASSERT_EQUAL(2*sizeof(double), sizeof(dcomplex));
}

void TestViews::dView( )
{
   dvec.newsize(27);
   
   int dims[] = {3,3,3};
   dVecView view (dvec, 3, dims);
   dVec init(3);
   
   
   
   init[0] = -1;
   init[1] = 0;
   init[2] = 1;
   view.ActiveDim(0);
   
   
   view = init;
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, dvec[0], 1e-18  );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dvec[1], 1e-18  );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, dvec[2], 1e-18  );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, dvec[3], 1e-18  );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, dvec[4], 1e-18  );
   
}

void TestViews::cView( )
{
}




