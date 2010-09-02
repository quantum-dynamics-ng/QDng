#include "OMatDipoleTest.h"
#include "defs.h"

#include "qdlib/OMatDipole.h"
#include "qdlib/WFLevel.h"
#include "tools/fstools.h"

using namespace std;
using namespace QDLIB;

CPPUNIT_TEST_SUITE_REGISTRATION(OMatDipoleTest);

OMatDipoleTest::OMatDipoleTest()
{
   // TODO Auto-generated constructor stub

}

OMatDipoleTest::~OMatDipoleTest()
{
   // TODO Auto-generated destructor stub
}

void OMatDipoleTest::API_Test()
{
   CollectorOp *Cop = CollectorOp::Instance();
   
   OMatDipole *M = new OMatDipole();
   Cop->Register(M);
   
   Laser* laser;
   ParamContainer pm;
   QDClock clock;

   clock.Dt(1);
   clock.Steps(3);


   CPPUNIT_ASSERT(M->Name() == "OMatDipole");

   /* missing laser file  name should throw */
   pm.SetValue("size", 2);
   CPPUNIT_ASSERT_THROW(M->Init(pm), EParamProblem);

   /* explicit empty init */
   pm.SetValue("empty", true); /* Do empty init */

//   try { M->Init(pm); } catch (Exception e) {cout<<e.GetMessage();}
   CPPUNIT_ASSERT_NO_THROW(M->Init(pm));

   /* Set the clock & laser */
   laser = M->GetLaser();
   laser->Dt(1);
   laser->Nt(3);
   M->Clock(&clock);

   /* Clock should be the same */
   CPPUNIT_ASSERT(laser->Clock() == &clock);

   /* Check Emin-Emax*/
   (*M)(0,0) = 0;
   (*M)(1,1) = 0;
   (*M)(0,1) = (*M)(1,0) = 1;

   (*laser)[0] = 0.5;
   (*laser)[1] = 2;
   (*laser)[2] = 1.5;

   M->InitDspace();
   clock.Begin();

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Emin", -0.5, M->Emin().real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Emax", 2, M->Emax().real(), LOOSE_EPS);
   
   DELETE_ALL_OP();
}


void OMatDipoleTest::NUMERIC_Test()
{
   CollectorOp *Cop = CollectorOp::Instance();
   CollectorWF *Cwf = CollectorWF::Instance();

   
   OMatDipole *M = new OMatDipole();
   Cop->Register(M);
   WaveFunction *wf = new WFLevel();
   WaveFunction *wf2;
   Cwf->Register(wf);
   
   Laser* laser;
   ParamContainer pm;
   QDClock clock;

   clock.Dt(1);
   clock.Steps(3);

   pm.SetValue("size", 2);
   pm.SetValue("empty", true); /* Do empty init */

   M->Init(pm);
   laser = M->GetLaser();
   laser->Dt(1);
   laser->Nt(3);
   M->Clock(&clock);

   (*M)(0,0) = 0;
   (*M)(1,1) = 0;
   (*M)(0,1) = (*M)(1,0) = 1;

   (*laser)[0] = 0.5;
   (*laser)[1] = 2;
   (*laser)[2] = 1.5;

   wf->Init(pm);
   (*wf)[0] = 1;
   (*wf)[1] = 1;
   wf->Normalize();

   wf2 = wf->NewInstance();
   CPPUNIT_ASSERT_NO_THROW( ((Operator*) M)->Init(wf));
   clock.Begin();

   M->Apply(wf2, wf);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1", laser->Get()/sqrt(2), (*wf2)[0].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1", laser->Get()/sqrt(2), (*wf2)[1].real(), LOOSE_EPS);
   ++clock;

   M->Apply(wf2, wf);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1", laser->Get()/sqrt(2), (*wf2)[0].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1", laser->Get()/sqrt(2), (*wf2)[1].real(), LOOSE_EPS);

   *wf2 = wf;
   M->Apply(wf2);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2", laser->Get()/sqrt(2), (*wf2)[0].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2", laser->Get()/sqrt(2), (*wf2)[1].real(), LOOSE_EPS);

   cVec exp(2);
//   M->InitDspace();
   M->InitExponential( &exp, dcomplex(0,-1) );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", -0.416146836547142, exp[0].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM", +0.909297426825682, exp[0].imag(), LOOSE_EPS);

   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM",  -0.416146836547142, exp[1].real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("expM",  -0.909297426825682, exp[1].imag(), LOOSE_EPS);
   
   DELETE_ALL_OP();
   DELETE_ALL_WF();

}

