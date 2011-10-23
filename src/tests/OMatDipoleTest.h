#ifndef OMATDIPOLETEST_H_
#define OMATDIPOLETEST_H_


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class OMatDipoleTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(OMatDipoleTest);
   CPPUNIT_TEST(API_Test);
   CPPUNIT_TEST(NUMERIC_Test);
   CPPUNIT_TEST_SUITE_END();

   public:
      OMatDipoleTest();
      virtual ~OMatDipoleTest();
   protected:
      void API_Test();
      void NUMERIC_Test();
};

#endif /* OMATDIPOLETEST_H_ */
