#include "PeakFinderTest.h"

#include "function_gens.h"
#include "defs.h"
#include "tools/PeakFinder.h"

#define PF_TEST_PEAKS 4
#define PF_TEST_NX 64

CPPUNIT_TEST_SUITE_REGISTRATION(PeakFinderTest);

PeakFinderTest::PeakFinderTest()
		: CppUnit::TestFixture()
{
}


PeakFinderTest::~PeakFinderTest()
{
}

void PeakFinderTest::NUMERIC_Test()
{
   dVec gauss(PF_TEST_NX);
   dVec spec(PF_TEST_NX);
   
   double dp = double(PF_TEST_NX)/double(PF_TEST_PEAKS) ;
   
   spec = 0;
         
   for (int i=0; i < PF_TEST_PEAKS-1; i++){
      fgen_gauss<dVec>(gauss, 0., double(PF_TEST_NX)-1, dp * (i+1), dp/8);
      spec += gauss;
   }
   
   PeakFinder pf;
   
   pf.Find(spec);
   
   CPPUNIT_ASSERT( pf.NumPeaks() == PF_TEST_PEAKS-1 );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.23499640074665620326, pf.Mean(), TIGHT_EPS );
   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.33308297028133193596, pf.Sigma(), TIGHT_EPS );
   
   int k=0;
   while (!pf.End()){
      CPPUNIT_ASSERT( pf.NextPeak() ==  int(dp * (k+1)) );
      k++;
   }
   CPPUNIT_ASSERT( k == PF_TEST_PEAKS-1);
   
}


