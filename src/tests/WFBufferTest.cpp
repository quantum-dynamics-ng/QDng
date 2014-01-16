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
   WFLevel *wf0, *wf1=NULL;
   ParamContainer& gp = GlobalParams::Instance();
   ParamContainer pm;
   
   WFBuffer *buf = NULL;
   WaveFunction* wf;
   
   gp.clear();
   gp.SetValue("MaxBufferSize", sizeof(dcomplex)*2); /* Buffer size in bytes => 2 WFs*/
   
   pm.SetValue("size",1);
   
   wf0 = new WFLevel();
   wf0->Init(pm);
  
   /* Init the buffer */
   CPPUNIT_ASSERT_NO_THROW(buf = new WFBuffer());
   CPPUNIT_ASSERT_NO_THROW( buf->Init(wf0) );
   CPPUNIT_ASSERT(buf->Size() == 1);
   
   /* Size limitation */
   buf->ResizeBuffer(3);
   buf->AutoLock(3); /* Set auto lock too large */
   CPPUNIT_ASSERT_NO_THROW(buf->Set(0, wf0));
   CPPUNIT_ASSERT_NO_THROW(buf->Set(1, wf0));
   CPPUNIT_ASSERT_NO_THROW(buf->Set(2, wf0));

   CPPUNIT_ASSERT_NO_THROW(wf = buf->Get(0));
   CPPUNIT_ASSERT_NO_THROW(wf = buf->Get(1));
   CPPUNIT_ASSERT_THROW(wf = buf->Get(2), EMemError); /* bang */

   CPPUNIT_ASSERT(wf != NULL);

   buf->Clear();
   buf->AutoLock(1); /* Set auto lock to fit */
   CPPUNIT_ASSERT_NO_THROW(buf->Set(0, wf0));
   CPPUNIT_ASSERT_NO_THROW(buf->Set(1, wf0));
   CPPUNIT_ASSERT_NO_THROW(buf->Set(2, wf0));
   CPPUNIT_ASSERT_NO_THROW(wf = buf->Get(0));
   CPPUNIT_ASSERT_NO_THROW(wf = buf->Get(1));
   CPPUNIT_ASSERT_NO_THROW(wf = buf->Get(2));

   buf->Clear();
   *((WaveFunction*) wf0) = 1.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(0, wf0));
   *((WaveFunction*) wf0) = 2.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(1, wf0));
   CPPUNIT_ASSERT(buf->_ondisk == 1);
   
   CPPUNIT_ASSERT(buf->Size() == 2);
   
   /* Check automatic resize */
   *((WaveFunction*) wf0) = 3.;
   CPPUNIT_ASSERT_NO_THROW(buf->Set(2, wf0));
   CPPUNIT_ASSERT(buf->Size() == 3);
   CPPUNIT_ASSERT(buf->_BytesInMem() == 16);
   CPPUNIT_ASSERT(buf->_ondisk == 2);
   
    /* Lets see if we get every thing back */
   CPPUNIT_ASSERT_NO_THROW(wf1 = dynamic_cast<WFLevel*>(buf->Get(0)));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 1 && (*wf1)[0].imag() == 0);
   CPPUNIT_ASSERT_NO_THROW(wf1 = dynamic_cast<WFLevel*>(buf->Get(1)));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 2 && (*wf1)[0].imag() == 0);
   CPPUNIT_ASSERT_NO_THROW(wf1 = dynamic_cast<WFLevel*>(buf->Get(2)));
   CPPUNIT_ASSERT( (*wf1)[0].real() == 3 && (*wf1)[0].imag() == 0);
   
   /* Safety of locking */
   buf->AutoLock(0);
   wf = buf->Get(0);
   CPPUNIT_ASSERT_NO_THROW(buf->Lock(0));
   wf = buf->Get(1);
   CPPUNIT_ASSERT_NO_THROW(buf->Lock(1));
   
   CPPUNIT_ASSERT_NO_THROW(buf->UnLock(0));
   CPPUNIT_ASSERT_NO_THROW(buf->UnLock(1));
   
   delete buf;
   DELETE_ALL_WF();
}
}
