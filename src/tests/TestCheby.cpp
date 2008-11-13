#include "TestCheby.h"
#include "cppunit/portability/FloatingPoint.h"
#include "sys/Exception.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestCheby);

using namespace QDLIB;


TestCheby::TestCheby()  : CPPUNIT_NS::TestFixture()
{
}


TestCheby::~TestCheby()
{
}

void TestCheby::setUp( )
{
}

void TestCheby::tearDown( )
{
}

void TestCheby::HarmonicGroundstate1D( )
{
   ParamContainer p;
   string s;
   string name;
   
   /* System parameters */
   p.SetValue( "dims", 1);
   p.SetValue( "N0", 1024);
   p.SetValue( "xmin0", -3);
   p.SetValue( "xmax0", 3);
   p.SetValue( "mass0", 1800);
   p.SetValue( "k0", 0.3363139634);
   
   
//    cout << endl << p;
   T.Init(p);
   Vh.Init(p);
   psi.Init(p);
   
   H.Add(&T);
   H.Add(&Vh);
   
   CPPUNIT_ASSERT_EQUAL(2, H.Size());
   

   
   /* 1D Groundstate */
   CPPUNIT_ASSERT ( ! psi.isKspace() );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("WF norm failed", 1.0, psi.Norm(), 1e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Kinetic energy", 0.003417245699, T.Expec( &psi), 1e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Potential energy", 0.003417245699, Vh.Expec( &psi), 1e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Groundstate energy", 0.006834491398, H.Expec( &psi), 1e-5);
   
   
}

void TestCheby::HarmonicGroundstate2D( )
{
   ParamContainer p;
   string s;
   string name;
   
   /* System parameters */
   p.SetValue( "dims", 2);
   p.SetValue( "N0", 1024);
   p.SetValue( "xmin0", -3);
   p.SetValue( "xmax0", 3);
   p.SetValue( "mass0", 1800);
   p.SetValue( "k0", 0.3363139634);
   p.SetValue( "N1", 1024);
   p.SetValue( "xmin1", -3);
   p.SetValue( "xmax1", 3);
   p.SetValue( "mass1", 1800);
   p.SetValue( "k1", 0.3363139634);
   
   
//    cout << endl << p;
   T.Init(p);
   Vh.Init(p);
   psi.Init(p);
   
   H.Add(&T);
   H.Add(&Vh);
   
   CPPUNIT_ASSERT_EQUAL(2, H.Size());
   
   
   
   /* 1D Groundstate */
   CPPUNIT_ASSERT ( ! psi.isKspace() );
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("WF norm failed", 1.0, psi.Norm(), 1e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Kinetic energy", 0.006834491398  , T.Expec( &psi), 5e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Potential energy",0.006834491398  , Vh.Expec( &psi), 5e-5);
   CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE ("Groundstate energy", 0.01366898280, H.Expec( &psi), 5e-5);
   
   
}

void TestCheby::Init( )
{
   ParamContainer p;
   string s;
   string name;
   
   /* Init routine */
   CPPUNIT_ASSERT_NO_THROW( U.Init(p) );
   p.SetValue( "order", 0);
   CPPUNIT_ASSERT_THROW( U.Init(p), EParamProblem );
   /* Empty reinit */
   CPPUNIT_ASSERT_THROW( U.ReInit(), EParamProblem );
   
   /* The "needs" interface */
   p = U.TellNeeds();
   p.ResetPosition();
   p.GetNextValue(name, s);
   
   CPPUNIT_ASSERT_NO_THROW( U.AddNeeds( name, &H) );
   /* ReInit with sense less Hamiltonain (empty sum) */
//    try {U.ReInit(); } catch (Exception e) { cout << endl << e.GetMessage() << endl;}
   CPPUNIT_ASSERT_THROW( U.ReInit(),  EParamProblem);
   
   /* ReInit with sense less Hamiltonain (unitialized T + V) */
   H.Add(&T);
   H.Add(&V);
   CPPUNIT_ASSERT_THROW(U.ReInit(), EParamProblem);
}

void TestCheby::Functions( )
{
}

void TestCheby::Propagation1D( )
{
}

void TestCheby::Propagation2D( )
{
}



