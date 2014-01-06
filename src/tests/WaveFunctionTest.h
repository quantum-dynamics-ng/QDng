#ifndef WAVEFUNCTIONTEST_H
#define WAVEFUNCTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/WaveFunction.h"

using namespace QDLIB;


/* Define a dummy child class to obtain a real, instanciable class */
class WFT : public QDLIB::WaveFunction
{
   private:
      string _name;
   public:
      WFT(): _name("Test") {}
      virtual WaveFunction* NewInstance()
      { 
	 WFT* res;
	 res = new WFT();
	 res->newsize(size(),strides());
	 return res;
      }
      virtual void Init(ParamContainer &params){}
      virtual const string& Name() {return _name;}
      virtual double Norm() {return 0;}
      void Normalize() {}
      virtual WaveFunction* operator=(WaveFunction* Psi){return NULL;}
      virtual dcomplex operator*(WaveFunction* Psi){return dcomplex(0,9);}
      
      virtual void Reduce(double tolerance) {}
      virtual void Restore() {}

      virtual void Serialize (std::ostream& os) {}
      virtual void DeSerialize (std::istream& is) {}
};


/**
 * 
 *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
class WaveFunctionTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( WaveFunctionTest );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      WaveFunctionTest();
   
      ~WaveFunctionTest();

      virtual void setUp();
      virtual void tearDown();
   protected:
      void NUMERIC_Test();
   private:
      WFT *wfc, *wfr, *wfbuf;
      cVec vc, vr;
};

#endif
