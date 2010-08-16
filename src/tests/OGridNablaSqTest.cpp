#include "OGridNablaSqTest.h"
#include "defs.h"
#include "function_gens.h"

using namespace QDLIB;
using namespace std;

#define WF_TEST_SIZE 64
#define XMIN -5;
#define XMAX 5;

CPPUNIT_TEST_SUITE_REGISTRATION(OGridNablaSqTest);

OGridNablaSqTest::OGridNablaSqTest()
   : CppUnit::TestFixture()
{
}


OGridNablaSqTest::~OGridNablaSqTest()
{
}

void OGridNablaSqTest::setUp()
{
   O = new OGridNablaSq();
   
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);
   
   wf = new WFGridCartesian();
   wf->Init(p);
   
   wf2 = new WFGridCartesian();
   wf2->Init(p);

   p.clear();
}

void OGridNablaSqTest::tearDown()
{
   p.clear();
   delete wf;
}

void OGridNablaSqTest::API_Test()
{
   CPPUNIT_ASSERT(O->Name() == "OGridNablaSq");
   
   /* Inits */
   CPPUNIT_ASSERT_THROW(O->Init(p), Exception);
   
   p.SetValue("dims", 1);
   p.SetValue("mass0", 0.5);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   

}

void OGridNablaSqTest::NUMERIC_Test()
{
   double L = 10;
//    double dx = L/(WF_TEST_SIZE-1);
   double pre = 2 * M_PI*M_PI / L / L;

   p.SetValue("dims", 1);
   p.SetValue("mass0", 1);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   
   fgen_sin_norm(*wf,-5,5);
   O->Apply(wf2, wf);
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }
   
   
   *wf2 = wf;
   
   O->Apply(wf2);
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }

   
}


