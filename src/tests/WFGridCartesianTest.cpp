#include "WFGridCartesianTest.h"
#include "defs.h"
#include "function_gens.h"

CPPUNIT_TEST_SUITE_REGISTRATION(WFGridCartesianTest);

using namespace QDLIB;
using namespace std;

#define WF_TEST_SIZE 64

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
   delete wf;
}


void WFGridCartesianTest::API_Test()
{
   ParamContainer p2;
   string s;
   int n;
   
   CPPUNIT_ASSERT(wf->Name() == "WFGridCartesian");
   
   CPPUNIT_ASSERT_THROW(wf->Init(p2), Exception);
   
   /* Init */
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);

   CPPUNIT_ASSERT_NO_THROW(wf->Init(p));
  
   /* Copy test */
   WaveFunction *wf2;
   
   wf2 = wf->NewInstance();
   CPPUNIT_ASSERT_NO_THROW( *wf2 = wf );
   
   p2 = wf2->Params();
   
   
   p2.GetValue("dims",s);
   CPPUNIT_ASSERT( s == "1");
   
   p2.GetValue("N0",n);
   CPPUNIT_ASSERT( n == WF_TEST_SIZE);

   p2.GetValue("xmin0",s);
   CPPUNIT_ASSERT( s == "-5");

   p2.GetValue("xmax0",s);
   CPPUNIT_ASSERT( s == "5");

   CPPUNIT_ASSERT( wf->size() == wf2->size());
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT( (*wf)[i].real() == (*wf2)[i].real());
      CPPUNIT_ASSERT( (*wf)[i].imag() == (*wf2)[i].imag());
   }
   
   
}

void WFGridCartesianTest::NUMERIC_Test()
{
   ParamContainer p2;
//    double d;
   dcomplex c;
   
   /* Init */
   CPPUNIT_ASSERT_NO_THROW(wf->Init(p));
   
   /* Norm features */
   fgen_sin_norm(*wf, -5, 5);
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, wf->Norm(), LOOSE_EPS);
   
   fgen_sin(*wf, -5, 5);
   
   wf->Normalize();
   
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, wf->Norm(), LOOSE_EPS);
   
   /* Scalar Product */
   c = *wf * wf;
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, c.real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, c.imag(), LOOSE_EPS);
}



