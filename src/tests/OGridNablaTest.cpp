#include "OGridNablaTest.h"
#include "defs.h"
#include "function_gens.h"

using namespace QDLIB;
using namespace std;

#define WF_TEST_SIZE 8
#define XMIN -5;
#define XMAX 5;

CPPUNIT_TEST_SUITE_REGISTRATION(OGridNablaTest);

OGridNablaTest::OGridNablaTest()
 : CppUnit::TestFixture()
{
}


OGridNablaTest::~OGridNablaTest()
{
}

void OGridNablaTest::setUp()
{
   O = new OGridNabla();
   
   p.SetValue("dims",1);
   p.SetValue("N0", WF_TEST_SIZE);
   p.SetValue("xmin0", -5);
   p.SetValue("xmax0", 5);
   
   wf = new WFGridCartesian();
   wf->Init(p);
   
   wf2 = new WFGridCartesian();
   wf2->Init(p);

   
   
   CollectorOp *Cop = CollectorOp::Instance();
   Cop->Register(O);
   
   CollectorWF *Cwf = CollectorWF::Instance();
   Cwf->Register(wf);
   Cwf->Register(wf2);
   
   /* Odd grid */
   p.SetValue("N0", WF_TEST_SIZE+1);
   wfo = new WFGridCartesian();
   wfo->Init(p);
   
   wf2o = new WFGridCartesian();
   wf2o->Init(p);

   
   Oo = new OGridNabla();
   Cop->Register(Oo);
   Cwf->Register(wfo);
   Cwf->Register(wf2o);
   
   p.clear();
}

void OGridNablaTest::tearDown()
{
   p.clear();
   DELETE_ALL_OP();
   DELETE_ALL_WF();
}

void OGridNablaTest::API_Test()
{
   CPPUNIT_ASSERT(O->Name() == "OGridNabla");
   
   /* Inits */
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   
   p.SetValue("dims", 1);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   
   /* Prefactor */
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1,O->PreFactor(), TIGHT_EPS);
   
   O->PreFactor(0.5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, O->PreFactor(), TIGHT_EPS);
   
   
}

void OGridNablaTest::NUMERIC_Test_Even()
{
   double L = 10;
   double dx = L/(WF_TEST_SIZE-1);
   double pre = 2 * M_PI / (L+dx); /* Pre-factor due to derivative and operator */

   p.SetValue("dims", 1);
   p.SetValue("factor", 1);
   CPPUNIT_ASSERT_NO_THROW(O->Init(p));
   CPPUNIT_ASSERT_NO_THROW(O->Init(wf));
   
   fgen_sin_norm(*wf,-5,5);
   O->Apply(wf2, wf);
   
   fgen_cos_norm(*wf,-5,5);

   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }
   
   
   fgen_sin_norm(*wf,-5,5);
   *wf2 = wf;
   O->Apply(wf2);
   
   fgen_cos_norm(*wf,-5,5);
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].real(), (*wf2)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wf)[i].imag(), (*wf2)[i].imag() , LOOSE_EPS);
   }
}


void OGridNablaTest::NUMERIC_Test_Odd()
{
   double L = 10;
   double dx = L/(WF_TEST_SIZE);
   double pre = 2 * M_PI / (L+dx); /* Pre-factor due to derivative and operator */

   p.SetValue("dims", 1);
   p.SetValue("factor", 1);
   CPPUNIT_ASSERT_NO_THROW(Oo->Init(p));
   CPPUNIT_ASSERT_NO_THROW(Oo->Init(wfo));
   
   fgen_sin(*wfo,-5,5);
   Oo->Apply(wf2o, wfo);
   
   fgen_cos(*wfo,-5,5);

   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].real(), (*wf2o)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].imag(), (*wf2o)[i].imag() , LOOSE_EPS);
   }
   
   
   fgen_sin(*wfo,-5,5);
   *wf2o = wfo;
   Oo->Apply(wf2o);

   fgen_cos(*wfo,-5,5);

   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].real(), (*wf2o)[i].real() , LOOSE_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL( pre * (*wfo)[i].imag(), (*wf2o)[i].imag() , LOOSE_EPS);
   }
}



