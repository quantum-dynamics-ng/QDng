#include "QDClock.h"
#include "tools/QDClock.h"
#include "defs.h"

CPPUNIT_TEST_SUITE_REGISTRATION(QDClockTest);

using namespace QDLIB;

QDClockTest::QDClockTest()
 : CppUnit::TestFixture()
{
}


QDClockTest::~QDClockTest()
{
}

void QDClockTest::API_Test()
{
   QDClock cl(20, 1.5), cl2;
   
   /* Constructor ? */
   CPPUNIT_ASSERT(cl.Dt() == 1.5);
   CPPUNIT_ASSERT(cl.Steps() == 20);
   
   /* Getters & Setters */
   cl.Dt(5.6);
   cl.Steps(33);
   
   CPPUNIT_ASSERT(cl.Dt() == 5.6);
   CPPUNIT_ASSERT(cl.Steps() == 33);

   cl.Begin();
   CPPUNIT_ASSERT(cl.TimeStep() == 0);
   CPPUNIT_ASSERT(cl.Time() == 0);

   cl.End();
   CPPUNIT_ASSERT(cl.TimeStep() == 33 - 1 );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( (33 - 1) * 5.6, cl.Time(), TIGHT_EPS);
   
   /* operators */
   cl.Begin();
   ++cl;
   CPPUNIT_ASSERT(cl.TimeStep() == 1);
   CPPUNIT_ASSERT(cl.Time() == 5.6);
   
   --cl;
   CPPUNIT_ASSERT(cl.TimeStep() == 0);
   CPPUNIT_ASSERT(cl.Time() == 0);

   /* Copy */
   cl2 = cl;
   CPPUNIT_ASSERT(cl2.Dt() == 5.6);
   CPPUNIT_ASSERT(cl2.Steps() == 33);

}


