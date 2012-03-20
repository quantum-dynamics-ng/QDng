#include "TestCheby.h"
#include "cppunit/portability/FloatingPoint.h"
#include "tools/Exception.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestCheby);

using namespace QDLIB;


TestCheby::TestCheby()  : CPPUNIT_NS::TestFixture()
{
}


TestCheby::~TestCheby()
{
}


void TestCheby::setUp()
{
   ParamContainer pm;
   OHermitianMatrix* h;

   U = new OCheby();

   TheClock.Dt(M_PI);
   TheClock.Steps(2);

   U->Clock(&TheClock);

   /* Create a simple RWA-like two - level system */
   H = new OHermitianMatrix();

   pm.SetValue("size", 2);
   H->Init(pm);

   h = dynamic_cast<OHermitianMatrix*> (H);
//   (*h)(0, 0) = (*h)(1, 1) = 0;
//   (*h)(0, 1) = (*h)(1, 0) = 0.5;
   (*h)(0, 0) = (*h)(1, 0) = (*h)(0, 1) = 0.0;
   (*h)(1, 1) = 0.5;



   psi0 = new WFLevel();
   psi0->Init(pm);
   (*psi0)[0] = 1;
   (*psi0)[1] = 0;

   H->Init(psi0);
   psi1 = psi0->NewInstance();

   *psi1 = dcomplex(0);

   CollectorOp *Cop = CollectorOp::Instance();
   Cop->Register(U);
   Cop->Register(H);

   CollectorWF *Cwf = CollectorWF::Instance();
   Cwf->Register(psi0);
}

void TestCheby::tearDown()
{
   DELETE_ALL_OP();
   DELETE_ALL_WF();
}

void TestCheby::API_Test( )
{
   ParamContainer pm;
   string s;
   string key, value;
   double d;
   

   CPPUNIT_ASSERT(U->Name() == "OCheby");


   /* Needs - Hamiltonian */
   CPPUNIT_ASSERT_NO_THROW(U->Add(s= "hamiltonian", H));

   /* Initalization */
   CPPUNIT_ASSERT_NO_THROW(U->Init(psi0));

   pm = U->Params();
   pm.GetValue("prec", d);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(1e-30, d, TIGHT_EPS );

   pm.GetValue("offset.imag", d);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, d, TIGHT_EPS );

   pm.GetValue("offset.real", d);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, d, TIGHT_EPS );

   pm.GetValue("scaling", d);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.25, d, TIGHT_EPS );

}


void TestCheby::NUMERIC_Test( )
{

   /* Init */
   string s("hamiltonian");
   CPPUNIT_ASSERT_NO_THROW(U->Add(s, H));
   CPPUNIT_ASSERT_NO_THROW(U->Init(psi0));

   /* psi = (1,0) - nothing should happen */
   U->Apply(psi1, psi0);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(1, (*psi1)[0].real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[0].imag(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[1].real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[1].imag(), LOOSE_EPS );

   /* psi = (0,1) - 1/4 rotation */

   (*psi0)[0] = 0;
   (*psi0)[1] = 1;
   U->Apply(psi1, psi0);


   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[0].real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[0].imag(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0, (*psi1)[1].real(), LOOSE_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL(-1, (*psi1)[1].imag(), LOOSE_EPS );

}

