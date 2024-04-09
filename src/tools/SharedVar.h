// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

/*
 * SharedVar.h
 *
 *  Created on: Jul 26, 2012
 *      Author: markus
 */

#ifndef SHAREDVAR_H_
#define SHAREDVAR_H_

#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef _OPENMP
 #define SetLock(LOCK) omp_set_lock(LOCK)
 #define UnSetLock(LOCK) omp_unset_lock(LOCK)
#else
 #define SetLock(LOCK);
 #define UnSetLock(LOCK);
#endif

namespace QDLIB
{

   /**
    * Scalar shared memory protected variable.
    */
   template <class C>
   class SharedVar {
      private:
         C _value;
#ifdef _OPENMP
         omp_lock_t* _lock;
#else
         int _lock;
#endif
      public:
         SharedVar()
         {
#ifdef _OPENMP
            omp_init_lock(_lock);
#endif
         }

         SharedVar(C val) : _value(val)
         {
#ifdef _OPENMP
            omp_init_lock(_lock);
#endif
         }

         ~SharedVar()
         {
#ifdef _OPENMP
            omp_destroy_lock(_lock);
#endif
         }

         C operator=(const C &rhs)
         {
            SetLock(_lock);
            _value = rhs;
            UnSetLock(_lock);
            return _value;
         }

         C operator*=(const C &rhs)
         {
            SetLock(_lock);
            _value *= rhs;
            UnSetLock(_lock);
            return _value;
         }

         C operator+=(const C &rhs)
         {
            SetLock(_lock);
            _value += rhs;
            UnSetLock(_lock);
            return _value;
         }

         C GetValue()
         {
            SetLock(_lock);
            C res = _value;
            UnSetLock(_lock);
            return res;
         }

   };

} /* namespace QDLIB */
#endif /* SHAREDVAR_H_ */
