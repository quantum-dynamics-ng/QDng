#include "WFGridCartesianTest.h"
#include "defs.h"
#include "function_gens.h"

CPPUNIT_TEST_SUITE_REGISTRATION(WFGridCartesianTest);

using namespace QDLIB;
using namespace std;

#define WF_TEST_SIZE 32

WFGridCartesianTest::WFGridCartesianTest()
 : CppUnit::TestFixture()
{
}


WFGridCartesianTest::~WFGridCartesianTest()
{
}

void WFGridCartesianTest::setUp()
{
   wf = new WFGridCartesian();
   
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);

}

void WFGridCartesianTest::tearDown()
{
   p.clear();
   DELETE_ALL_WF();
}


void WFGridCartesianTest::API_Test()
{
   ParamContainer p2;
   string s;
   
   CPPUNIT_ASSERT(wf->Name() == "WFGridCartesian");
   
   CPPUNIT_ASSERT_THROW(wf->Init(p2), Exception);
   
   /* Init */
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);

   CPPUNIT_ASSERT_NO_THROW(wf->Init(p));
  
   /* Init with some numbers */
   for (int i=0; i < WF_TEST_SIZE; i++)
      (*wf)[i] = double(i);
            
   /* Copy test */
   WaveFunction *wf2;
   
   wf2 = wf->NewInstance();
   CPPUNIT_ASSERT_NO_THROW( *wf2 = wf );
   
   p2 = wf2->Params();
   
   
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wf2)->Dim() == 1);
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wf2)->DimSize(0) == WF_TEST_SIZE);
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wf2)->Xmin(0) == -5);
   CPPUNIT_ASSERT( dynamic_cast<WFGridCartesian*>(wf2)->Xmax(0) == 5);

   CPPUNIT_ASSERT( wf->size() == wf2->size());
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT( (*wf)[i].real() == (*wf2)[i].real());
      CPPUNIT_ASSERT( (*wf)[i].imag() == (*wf2)[i].imag());
   }
   
   
}

void WFGridCartesianTest::NUMERIC_Test()
{
   ParamContainer p2;
   dcomplex c;
   
   /* Init */
   CPPUNIT_ASSERT_NO_THROW(wf->Init(p));
   
   /* Norm features */
   fgen_sin_norm(*wf, -5, 5);
   
   *wf *= 2;

   CPPUNIT_ASSERT_DOUBLES_EQUAL(2, wf->Norm(), LOOSE_EPS);
   
   fgen_sin_norm(*wf, -5, 5);
   
   wf->Normalize();
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, wf->Norm(), LOOSE_EPS);
   
   /* Scalar Product */
   c = *wf * wf;
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, c.real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, c.imag(), LOOSE_EPS);
}



