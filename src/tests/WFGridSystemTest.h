#ifndef WFGRIDSYSTEMTEST_H
#define WFGRIDSYSTEMTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/WFGridSystem.h"

using namespace QDLIB;

class WFG : public WFGridSystem {
   private:
      string _name;
   public:
      WFG(): _name("Test") {}
      virtual WaveFunction* NewInstance()
      { 
         WFG* res;
         res = new WFG;
         res->newsize(size(),strides());
         return res;
      }
      virtual WaveFunction* operator= (WaveFunction *Psi){return NULL;}
      virtual const string& Name() {return _name;}
      virtual double Norm() {return 0;}
      void Normalize() {}
      dcomplex Integral(){return dcomplex(0);}
      virtual dcomplex operator*(WaveFunction* Psi){return dcomplex(0,9);}
};

/**
	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
class WFGridSystemTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( WFGridSystemTest );
   CPPUNIT_TEST( API_Test );
   CPPUNIT_TEST( NUMERIC_Test );
   CPPUNIT_TEST_SUITE_END();
   public:
      WFGridSystemTest();
   
      ~WFGridSystemTest();
      
      virtual void setUp();
      virtual void tearDown();
      
      void API_Test();
      void NUMERIC_Test();
   private:
      WFG *wf;
};

#endif
