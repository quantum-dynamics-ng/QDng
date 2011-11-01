/*
 * ThreadTest.cpp
 *
 *  Created on: 01.11.2011
 *      Author: markus
 */

#include "ThreadTest.h"
#include "tools/Thread.h"
#include "tools/GlobalParams.h"

using namespace QDLIB;

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadTest);

ThreadTest::ThreadTest()
{
}

ThreadTest::~ThreadTest()
{
}

class MyThread : public Thread {
   private:
      bool _done;
      Mutex _m;

   public:
      MyThread() : _done(false) {}

      void Execute()
      {
         _m.Lock();
         _done = true;
         _m.UnLock();
      }

      bool Done()
      {
         _m.Lock();
         bool res = _done;
         _m.UnLock();
         return res;
      }
};

class MyLoopThread : public Thread {
      public:
         void Execute()
         {
            Sleep(30);
         }
};

void ThreadTest::CORE_Test()
{
   MyThread thread;

   CPPUNIT_ASSERT( thread.Start() == 0);

   CPPUNIT_ASSERT( thread.Join() == 0 );

   CPPUNIT_ASSERT( thread.Done() );

}

void ThreadTest::API_Test()
{
   MyLoopThread thread;

   CPPUNIT_ASSERT( thread.Start() == 0);
   CPPUNIT_ASSERT( thread.Cancel() == 0);

}
