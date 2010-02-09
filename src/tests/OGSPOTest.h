#ifndef OGSPOTEST_H_
#define OGSPOTEST_H_


#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/OGSPO.h"
#include "qdlib/OHermitianMatrix.h"
#include "qdlib/OMatDipole.h"
#include "qdlib/WFLevel.h"

class OGSPOTest: public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(OGSPOTest);
   CPPUNIT_TEST(API_Test);
   CPPUNIT_TEST(NUMERIC_Test_H0);
   CPPUNIT_TEST(NUMERIC_Test_Ht);
   CPPUNIT_TEST_SUITE_END();

   public:
      OGSPOTest();
      virtual ~OGSPOTest();

      void setUp();
      void tearDown();
   protected:
      void API_Test();
      void NUMERIC_Test_H0();
      void NUMERIC_Test_Ht();

   private:
      QDLIB::OGSPO *U;
      QDLIB::Operator *H, *Ht;
      QDLIB::WaveFunction *psi0, *psi1;
      QDLIB::QDClock TheClock;
      QDLIB::Laser *TheLaser;
};

#endif /* OGSPOTEST_H_ */
