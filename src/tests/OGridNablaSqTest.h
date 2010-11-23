#ifndef OGRIDNABLASQTEST_H
#define OGRIDNABLASQTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/OGridNablaSq.h"
#include "qdlib/WFGridCartesian.h"

using namespace QDLIB;

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class OGridNablaSqTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( OGridNablaSqTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test_Even );
   CPPUNIT_TEST( NUMERIC_Test_Odd );
   CPPUNIT_TEST_SUITE_END();
   public:
      OGridNablaSqTest();
   
      ~OGridNablaSqTest();
      void setUp();
      void tearDown();
   protected:
      void API_Test();
      void NUMERIC_Test_Even();
      void NUMERIC_Test_Odd();
   private:
      ParamContainer p;
      WaveFunction *wf, *wf2;
      OGridNablaSq *O;
      WaveFunction *wfo, *wf2o; /* Odd Number of grid points */
      OGridNablaSq *Oo;
};

#endif
