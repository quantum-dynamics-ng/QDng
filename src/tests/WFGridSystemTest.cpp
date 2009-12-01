#include "WFGridSystemTest.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(WFGridSystemTest);

#define WF_TEST_SIZE 64

using namespace QDLIB;

WFGridSystemTest::WFGridSystemTest()
 : CppUnit::TestFixture()
{
}


WFGridSystemTest::~WFGridSystemTest()
{
}

void fgen_sin(cVec &vec, double min, double max)
{
   double dx = (max - min)/(vec.size()-1);
   
   for (lint i=0; i < vec.size(); i++){
      vec[i] = sin( (double(i) * dx + min)  * 2 / M_PI);
   }
}

void WFGridSystemTest::setUp()
{
   wf = new WFG;
   wf->newsize(WF_TEST_SIZE);
   fgen_sin(*wf, -5, 5);
}

void WFGridSystemTest::tearDown()
{
   delete wf;
}

void WFGridSystemTest::API_Test()
{
   ParamContainer p;
   WFG *wf2;
   
   /* Empty Init */
   CPPUNIT_ASSERT_THROW(wf->Init(p), Exception);
   
   /* Init with correst params */
   wf->Dim(1);
   wf->DimSizes(0,WF_TEST_SIZE);
   wf->Xmin(0,-5);
   wf->Xmax(0,5);
   
   CPPUNIT_ASSERT_NO_THROW( wf->Init(p));
   
   /* Check if Transformation is running */
   CPPUNIT_ASSERT(! wf->isKspace() );
   wf->ToKspace();
   CPPUNIT_ASSERT( wf->isKspace() );
   wf->ToXspace();
   CPPUNIT_ASSERT(! wf->isKspace() );
   
   /* Check copy */
   wf2 = dynamic_cast<WFG*>(wf->NewInstance());
   *((WFGridSystem*) wf2)  = (WFGridSystem*) wf;
   
   CPPUNIT_ASSERT_NO_THROW( wf2->Init(p));
   
   CPPUNIT_ASSERT( *((GridSystem*) wf) == *((GridSystem*) wf2));
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT( (*wf)[i].real() == (*wf2)[i].real());
      CPPUNIT_ASSERT( (*wf)[i].imag() == (*wf2)[i].imag());
   }
   
   /* Check if Transformation is running */
   CPPUNIT_ASSERT(! wf2->isKspace() );
   wf2->ToKspace();
   CPPUNIT_ASSERT( wf2->isKspace() );
   wf2->ToXspace();
   CPPUNIT_ASSERT(! wf2->isKspace() );
   
   delete wf2;
}

void WFGridSystemTest::NUMERIC_Test()
{
   ParamContainer p;
   
   /* Init with correst params */
   wf->Dim(1);
   wf->DimSizes(0,WF_TEST_SIZE);
   wf->Xmin(0,-5);
   wf->Xmax(0,5);
   
   CPPUNIT_ASSERT_NO_THROW( wf->Init(p));
   wf->ToKspace();
   
   /* for proper fft */
   CPPUNIT_ASSERT_DOUBLES_EQUAL( -(*wf)[1].imag(),  (*wf)[WF_TEST_SIZE-1].imag(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( (*wf)[1].real(),  (*wf)[WF_TEST_SIZE-1].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, (*wf)[0].imag(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, (*wf)[0].real(), LOOSE_EPS);
   
   for (lint i=2; i < WF_TEST_SIZE-1; i++)
      CPPUNIT_ASSERT( cabs((*wf)[i]) < cabs((*wf)[1])/8 );
}




