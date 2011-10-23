#ifndef OGRIDNABLATEST_H
#define OGRIDNABLATEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/OGridNabla.h"
#include "qdlib/WFGridCartesian.h"

using namespace QDLIB;


/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class OGridNablaTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( OGridNablaTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test_Even );
   CPPUNIT_TEST( NUMERIC_Test_Odd );
   CPPUNIT_TEST_SUITE_END();
   public:
      OGridNablaTest();
   
      ~OGridNablaTest();
      void setUp();
      void tearDown();
      
   protected:
      void API_Test();
      void NUMERIC_Test_Even();
      void NUMERIC_Test_Odd();
      
   private:
      ParamContainer p;
      WaveFunction *wf, *wf2;
      WaveFunction *wfo, *wf2o; /* Odd sizes */
      OGridNabla *O, *Oo;

};

#endif
