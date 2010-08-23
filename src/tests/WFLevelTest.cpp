#include "WFLevelTest.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(WFLevelTest);

using namespace QDLIB;
using namespace std;

WFLevelTest::WFLevelTest()
 : CppUnit::TestFixture()
{
}


WFLevelTest::~WFLevelTest()
{
}

void WFLevelTest::API_Test()
{
   ParamContainer pm;
   WFLevel *wf = new WFLevel();
   CollectorWF::Instance()->Register(wf);
   WaveFunction *wf2 = NULL;
   
   /* The name */
   CPPUNIT_ASSERT( wf->Name() == "WFLevel");
   
   /* Empty init */
   CPPUNIT_ASSERT_THROW(wf->Init(pm), EParamProblem );
   
   /* Init */
   pm.SetValue("size", 2);
   CPPUNIT_ASSERT_NO_THROW( wf->Init(pm) );
   
   CPPUNIT_ASSERT(wf->size() == 2);
   
   /* Fresh instance */
   wf2 = wf->NewInstance();
   CPPUNIT_ASSERT(wf2->size() == 2);
   
   /* Followed by copy */
   (*wf)[0] = 1;
   (*wf)[1] = 2;
   *wf2 = wf;
   CPPUNIT_ASSERT( (*wf2)[0].real() == 1 &&  (*wf2)[1].real() == 2);
   
   DELETE_ALL_WF();
}

void WFLevelTest::NUMERIC_Test()
{
   ParamContainer pm;
   WFLevel *wf = new WFLevel();
   CollectorWF::Instance()->Register(wf);
   
   pm.SetValue("size", 2);
   wf->Init(pm);
   /* Scalar Product & Norm */
   (*wf)[0] = 1;
   (*wf)[1] = dcomplex(0,1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Norm", sqrt(2), wf->Norm(), TIGHT_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Scalar Product", 2, (*wf * wf).real(), TIGHT_EPS);
   
   wf->Normalize();
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Normalized", 1, wf->Norm(), LOOSE_EPS);
   
   DELETE_ALL_WF();
}


