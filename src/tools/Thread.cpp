// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * Thread.cpp
 *
 *  Created on: 01.11.2011
 *      Author: markus
 */

#include "Thread.h"
#include <unistd.h>

namespace QDLIB
{

   Thread::Thread()
   {
      pthread_attr_init(&_attr);
   }

   Thread::~Thread()
   {
      pthread_attr_destroy(&_attr);
   }

   /**
    * Serve as dummy method for startup by pthread_create.
    */
   void* Thread::EntryPoint(void *pthis)
   {
      Thread* thread = (Thread*) pthis;

      thread->Execute();

      return NULL;
   }

   /**
    * Exit the running thread.
    */
   void Thread::Exit()
   {
      pthread_exit(NULL);
   }

   /**
    * Startup method
    */
   int  Thread::Start()
   {
      return pthread_create(&_thread, &_attr, Thread::EntryPoint, this);
   }

   /**
    * Wait for thread to finish.
    */
   int Thread::Join()
   {
      void *retval;
      int res = pthread_join(_thread, &retval);
      return res;
   }

   /**
    * Cancel a running thread.
    */
   int Thread::Cancel()
   {
      return pthread_cancel(_thread);
   }


   /**
    * Sleep sec seconds.
    */
   void Thread::Sleep(unsigned int sec)
   {
      unsigned int res = sec;

      while (res != 0){
         res = sleep(res);
      }

   }

   Mutex::Mutex()
   {
      pthread_mutex_init(&_mutex, NULL);
   }

   Mutex::~Mutex()
   {
      pthread_mutex_destroy(&_mutex);
   }

   int Mutex::Lock()
   {
      return pthread_mutex_lock(&_mutex);
   }

   int Mutex::UnLock()
   {
      return pthread_mutex_unlock(&_mutex);
   }
}
