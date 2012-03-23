#include "WFBufferTest.h"
#include "qdlib/WFLevel.h"
#include "tools/GlobalParams.h"
#include "qdlib/WFBuffer.h"

namespace QDLIB {
CPPUNIT_TEST_SUITE_REGISTRATION(WFBufferTest);

using namespace QDLIB;
using namespace std;



WFBufferTest::WFBufferTest()
{

}

WFBufferTest::~WFBufferTest()
{
  DELETE_ALL_WF();
}

void WFBufferTest::API_Test_DiskBuf()
{
   WaveFunction *wf0, *wf1=NULL;
   ParamContainer& gp = GlobalParams::Instance();
   ParamContainer pm;
   
   WFBuffer *buf = NULL;
   
   gp.clear();
   gp.SetValue("MaxBufferSize", sizeof(dcomplex)*2); /* Buffer size in bytes => 2 WFs*/
   
   pm.SetValue("size",1);
   
   wf0 = new WFLevel();
   wf0->Init(pm);
  
   CPPUNIT_ASSERT_NO_THROW(buf = new WFBuffer());
   
   CPPUNIT_ASSERT_NO_THROW(buf->Size(2));
   
   CPPUNIT_ASSERT_NO_THROW( buf->Init(wf0) );
   
   CPPUNIT_ASSERT(buf->Size() == 2);
   
   CPPUNIT_ASSERT_THROW(buf->AutoLock(4), EParamProblem);
   CPPUNIT_ASSERT_NO_THROW(buf->AutoLock(1));
   
   *wf0 = 1.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(0, wf0));
   *wf0 = 2.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(1, wf0));
   CPPUNIT_ASSERT(buf->_inmem == 2);
   CPPUNIT_ASSERT(buf->_ondisk == 0);
   
   CPPUNIT_ASSERT(buf->Size() == 2);
   
   /* Check automatic resize */
   *wf0 = 3.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(2, wf0));
   CPPUNIT_ASSERT(buf->Size() == 3);
   CPPUNIT_ASSERT(buf->_inmem == 2);
   CPPUNIT_ASSERT(buf->_ondisk == 1);
   
    /* Lets see if we get every thing back */
   CPPUNIT_ASSERT_NO_THROW(wf1 = buf->Get(0));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 1 && (*wf1)[0].imag() == 0);
   CPPUNIT_ASSERT_NO_THROW(wf1 = buf->Get(1));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 2 && (*wf1)[0].imag() == 0);
   CPPUNIT_ASSERT_NO_THROW(wf1 = buf->Get(2));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 3 && (*wf1)[0].imag() == 0);
   
   /* Safety of locking */
   CPPUNIT_ASSERT_NO_THROW(buf->Lock(0));
   CPPUNIT_ASSERT_THROW(buf->Lock(1), EParamProblem);
   
   CPPUNIT_ASSERT_NO_THROW(buf->UnLock(0));
   CPPUNIT_ASSERT_NO_THROW(buf->UnLock(1));
   
   delete buf;
   DELETE_ALL_WF();
}
}
