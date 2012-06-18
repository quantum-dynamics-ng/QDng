#include "OGridNablaSqTest.h"
#include "defs.h"
#include "function_gens.h"

using namespace QDLIB;
using namespace std;

#define WF_TEST_SIZE 8
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
   
   /* Odd number of points */
   p.SetValue("N0", WF_TEST_SIZE + 1);
   wfo = new WFGridCartesian();
   wfo->Init(p);
   
   wf2o = new WFGridCartesian();
   wf2o->Init(p);

   Oo = new OGridNablaSq();
   
   p.clear();
}

void OGridNablaSqTest::tearDown()
{
   p.clear();
   DELETE_ALL_WF();
   DELETE_ALL_OP();
}

void OGridNablaSqTest::API_Test()
{
   CPPUNIT_ASSERT(O->Name() == "OGridNablaSq");
   
   /* Inits */
   p.SetValue("dims", 0);
   CPPUNIT_ASSERT_THROW(O->Init(p), Exception);
   
   p.SetValue("dims", 1);
   p.SetValue("mass", 0.5);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   

}

void OGridNablaSqTest::NUMERIC_Test_Even()
{
   double L = 10;
   double dx = L/(WF_TEST_SIZE-1);
   double pre = M_PI / (L + dx); /* Pre-factor due to derivative an Tkin operator */
   pre *= 2 * pre;

   p.SetValue("dims", 1);
   p.SetValue("mass", 1);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   
   fgen_sin_norm(*wf,-5,5);
   O->Apply(wf2, wf);

   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }
   
   *wf2 = wf;
   
   O->Apply(wf2, wf);
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }

   
}

void OGridNablaSqTest::NUMERIC_Test_Odd()
{
   double L = 10;
   double dx = L/(WF_TEST_SIZE);
   double pre = M_PI / (L + dx); /* Pre-factor due to derivative an Tkin operator */
   pre *= 2 * pre;

   p.SetValue("dims", 1);
   p.SetValue("mass", 1);
   CPPUNIT_ASSERT_NO_THROW(Oo->Init(p));
   CPPUNIT_ASSERT_NO_THROW(Oo->Init(wfo));
   
   fgen_sin(*wfo,-5,5);
   Oo->Apply(wf2o, wfo);

   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].real(), (*wf2o)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].imag(), (*wf2o)[i].imag() , LOOSE_EPS);
   }
   
   *wf2o = wfo;
   
   ((Operator*) Oo)->Apply(wf2o);
   
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].real(), (*wf2o)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].imag(), (*wf2o)[i].imag() , LOOSE_EPS);
   }

   
}

