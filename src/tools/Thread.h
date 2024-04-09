// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * Thread.h
 *
 *  Created on: 01.11.2011
 *      Author: markus
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>


namespace QDLIB
{

   /**
    * This is a simple posix thread iplementation.
    */
   class Thread
   {
      private:
         pthread_t _thread;
         pthread_attr_t _attr;
         Thread(Thread &t) {} /* A thread must not be copied */
      protected:
         static void* EntryPoint(void *pthis);

         void Exit();

      public:
         Thread();
         ~Thread();

         int Cancel();
         int Join();
         int Start();

         static void Sleep(unsigned int sec);

         virtual void Execute() = 0;
   };


   /**
    * Simple class for pthread mutex.
    */
   class Mutex
   {
      private:
         pthread_mutex_t _mutex;
         Mutex(Mutex &m) {} /* A mutex must not be copied */
      public:
         Mutex();
         ~Mutex();
         int Lock();
         int UnLock();
   };
}

#endif /* THREAD_H_ */
