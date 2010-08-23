#include "WaveFunctionTest.h"
#include "defs.h"

#define WF_TEST_SIZE 10

CPPUNIT_TEST_SUITE_REGISTRATION(WaveFunctionTest);
using namespace QDLIB;

WaveFunctionTest::WaveFunctionTest()
 : CppUnit::TestFixture()
{
}


WaveFunctionTest::~WaveFunctionTest()
{
}

void WaveFunctionTest::setUp()
{
   CollectorWF *Cwf = CollectorWF::Instance();
   wfr = new WFT();
   wfc = new WFT();
   wfbuf = new WFT();

   Cwf->Register(wfr);
   Cwf->Register(wfc);
   Cwf->Register(wfbuf);
   
   wfr->newsize(WF_TEST_SIZE);
   wfc->newsize(WF_TEST_SIZE);
   
   vr.newsize(WF_TEST_SIZE);
   vc.newsize(WF_TEST_SIZE);

   wfbuf->newsize(WF_TEST_SIZE);
   dcomplex c;
   for (lint i=0; i < WF_TEST_SIZE; i++){
      (*wfc)[i] = dcomplex( sin(2 * M_PI *double(i) / double(WF_TEST_SIZE))
	    , cos(2 * M_PI *double(i) / double(WF_TEST_SIZE)) );
      vc[i] = dcomplex( sin(2 * M_PI *double(i) / WF_TEST_SIZE)
	    , cos(2 * M_PI *double(i) / WF_TEST_SIZE) );
      (*wfr)[i] = 0.5*sin(2 * M_PI *double(i) / double(WF_TEST_SIZE));
      vr[i] = 0.5*sin(2 * M_PI *double(i) / double(WF_TEST_SIZE));
      
   }
}

void WaveFunctionTest::tearDown()
{
   DELETE_ALL_WF();
}


/** \todo Indepently test (const x) and (non const x) methods */
void WaveFunctionTest::NUMERIC_Test()
{
   
//    operator*= (const double d)
   *wfbuf = *wfc; /* Copy operator from cVec parent */
   *wfbuf *= 2;
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(vc[i].real() * 2, (*wfbuf)[i].real(), TIGHT_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(vc[i].imag() * 2, (*wfbuf)[i].imag(), TIGHT_EPS);
   }
   
//    operator*= (const dcomplex d)
   *wfbuf = *wfc; /* Copy operator from cVec parent */
   *wfbuf *= dcomplex(1,2);
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] * dcomplex(1,2)).real(), (*wfbuf)[i].real(), TIGHT_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] * dcomplex(1,2)).imag() , (*wfbuf)[i].imag(), TIGHT_EPS);
   }
   
   
//    WaveFunction* operator=(double d)
   *((WaveFunction*) wfbuf) = 2;
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(2, (*wfbuf)[i].real(), TIGHT_EPS);
   }

//    WaveFunction* operator=(dcomplex d)
   *((WaveFunction*) wfbuf) = dcomplex(1,2);
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1, (*wfbuf)[i].real(), TIGHT_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(2, (*wfbuf)[i].imag(), TIGHT_EPS);
   }

//    WaveFunction* operator+(WaveFunction* Psi)
   WaveFunction *wfsum;
   wfsum = *wfc + wfr;
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] + vr[i]).real(), (*wfsum)[i].real(), TIGHT_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] + vr[i]).imag(), (*wfsum)[i].imag(), TIGHT_EPS);
   }
   DELETE_WF(wfsum);
   
//    WaveFunction* operator+=(WaveFunction* Psi)
   *wfbuf = *wfc; /* Copy operator from cVec parent */
   *((WaveFunction*) wfbuf) += wfc;
   for (lint i=0; i < WF_TEST_SIZE; i++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] + vc[i]).real(), (*wfbuf)[i].real(), TIGHT_EPS);
      CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] + vc[i]).imag(), (*wfbuf)[i].imag(), TIGHT_EPS);
   }
   
//    WaveFunction* operator-=(WaveFunction* Psi)
      *wfbuf = *wfc; /* Copy operator from cVec parent */
      *((WaveFunction*) wfbuf) -= wfc;
      for (lint i=0; i < WF_TEST_SIZE; i++){
	 CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] - vc[i]).real(), (*wfbuf)[i].real(), TIGHT_EPS);
	 CPPUNIT_ASSERT_DOUBLES_EQUAL((vc[i] - vc[i]).imag(), (*wfbuf)[i].imag(), TIGHT_EPS);
      }
}



