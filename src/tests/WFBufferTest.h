#ifndef WFBUFFERTEST_H
#define WFBUFFERTEST_H

#include <TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "qdlib/WFBuffer.h"
/**
        @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
*/
namespace QDLIB {
   class WFBufferTest : public CppUnit::TestFixture
   {
      CPPUNIT_TEST_SUITE( WFBufferTest );
      CPPUNIT_TEST( API_Test_DiskBuf );
      CPPUNIT_TEST_SUITE_END();
       public:
           WFBufferTest();
           ~WFBufferTest();
      protected:
         void API_Test_DiskBuf();
   };
}
#endif
