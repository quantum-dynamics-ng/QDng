#ifndef TESTCHEBY_H
#define TESTCHEBY_H

#include "cppunit/extensions/HelperMacros.h"
#include "qdlib/OCheby.h"
#include "qdlib/OGridNablaSq.h"
#include "qdlib/OGridPotential.h"
#include "qdlib/OSum.h"
#include "OGridHarmonic.h"
#include "WFGridHarmonic.h"

/**
 *	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class TestCheby : public CPPUNIT_NS::TestFixture {
   CPPUNIT_TEST_SUITE( TestCheby );
   CPPUNIT_TEST( Init );
   CPPUNIT_TEST( HarmonicGroundstate1D );
   CPPUNIT_TEST( HarmonicGroundstate2D );
   CPPUNIT_TEST( Functions );
   CPPUNIT_TEST_SUITE_END();
   public:
      TestCheby();
   
      ~TestCheby();
      virtual void setUp();
      virtual void tearDown(); 
	 
      void Init();
      void HarmonicGroundstate1D();
      void HarmonicGroundstate2D();
      void Functions();
      void Propagation1D();
      void Propagation2D();
   private:
      QDLIB::OCheby U;
      QDLIB::OGridNablaSq T;
      QDLIB::OGridPotential V;
      QDLIB::OSum H;
      QDLIB::WFGridHarmonic psi;
      QDLIB::OGridHarmonic Vh;
	   
};

#endif



