#include "OGSPOTest.h"
#include "defs.h"

using namespace std;
using namespace QDLIB;

CPPUNIT_TEST_SUITE_REGISTRATION( OGSPOTest);

OGSPOTest::OGSPOTest()
{
}

OGSPOTest::~OGSPOTest()
{
}

void OGSPOTest::setUp()
{
   ParamContainer pm;
   OHermitianMatrix* h;
   OMatDipole* ht;
   U = new OGSPO();

   TheClock.Dt(M_PI);
   TheClock.Steps(2);

   U->Clock(&TheClock);

   /* Create a simple RWA-like two - level system */
   H = new OHermitianMatrix();
   Ht = new OMatDipole();

   pm.SetValue("size", 2);
   H->Init(pm);

   pm.SetValue("empty", true);
   Ht->Init(pm);

   h = dynamic_cast<OHermitianMatrix*> (H);
   (*h)(0, 0) = (*h)(1, 1) = 0;
   (*h)(0, 1) = (*h)(1, 0) = 0.5;

   ht = dynamic_cast<OMatDipole*> (Ht);
   (*ht)(0, 0) = (*ht)(1, 1) = 0;
   (*ht)(0, 1) = (*ht)(1, 0) = 1;
   
   TheLaser = ht->GetLaser();

   TheLaser->Dt(M_PI);
   TheLaser->Nt(2);
   TheLaser->Clock(&TheClock);

   (*TheLaser)[0] = 0;
   (*TheLaser)[1] = -0.25;


   psi0 = new WFLevel();
   psi0->Init(pm);
   (*psi0)[0] = 1;
   (*psi0)[1] = 0;

   H->Init(psi0);
   psi1 = psi0->NewInstance();
}

void OGSPOTest::tearDown()
{
   delete psi0;
   delete psi1;
}

void OGSPOTest::API_Test()
{
   ParamContainer pm;
   string key, value;
   char a = 'A';
   char abc[] = " \0";

   /* Name */
   CPPUNIT_ASSERT(U->Name() == "OGSPO");

   /* Needs - expect at least A,B,C */
   pm = U->TellNeeds();
   pm.ResetPosition();
   for (char i = 0; i < 3; i++) {
      pm.GetNextValue(key, value);
      abc[0] = a + i;
      CPPUNIT_ASSERT_MESSAGE(abc, key == string(abc));
   }

}

/** Single Operator - Time independant */
void OGSPOTest::NUMERIC_Test_H0()
{
   ParamContainer pm;
   string s = "A";

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Add Operator", U->AddNeeds(s, H));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Init PM", U->Init(pm));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Init Psi", U->Init(psi0));

   U->Apply(psi1, psi0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1 - Propa half cycle", 0,((*psi1)[0].conj() * (*psi1)[0]).real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1 - Propa half cycle", 1,((*psi1)[1].conj() * (*psi1)[1]).real(), LOOSE_EPS);

   *psi1 = psi0;
   U->Apply(psi1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2 - Propa half cycle", 0,((*psi1)[0].conj() * (*psi1)[0]).real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2 - Propa half cycle", 1,((*psi1)[1].conj() * (*psi1)[1]).real(), LOOSE_EPS);
}

/** Two operators - Time dependetant */
void OGSPOTest::NUMERIC_Test_Ht()
{
   ParamContainer pm;
   string s = "A";

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Add Operator", U->AddNeeds(s, H));
   s = "B";
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Add Operator", U->AddNeeds(s, Ht));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Init PM", U->Init(pm));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Init Psi", U->Init(psi0));

   TheClock.Begin();
   U->Apply(psi1, psi0);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1 - Propa half cycle", 0,((*psi1)[0].conj() * (*psi1)[0]).real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply1 - Propa half cycle", 1,((*psi1)[1].conj() * (*psi1)[1]).real(), LOOSE_EPS);

   *psi1 = psi0;
   U->Apply(psi1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2 - Propa half cycle", 0,((*psi1)[0].conj() * (*psi1)[0]).real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply2 - Propa half cycle", 1,((*psi1)[1].conj() * (*psi1)[1]).real(), LOOSE_EPS);


   /* Now The Laser is on ! */
   ++TheClock;
   *psi1 = psi0;
   U->Apply(psi1);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply Ht - Propa half cycle", 0.5,((*psi1)[0].conj() * (*psi1)[0]).real(), LOOSE_EPS);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Apply Ht - Propa half cycle", 0.5,((*psi1)[1].conj() * (*psi1)[1]).real(), LOOSE_EPS);

}
