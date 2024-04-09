// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "QDClock.h"
#include <math.h>
#include "Exception.h"

namespace QDLIB
{
   QDClock::QDClock() : _dt(0), _nsteps(0), _step(0) {}
   
   QDClock::QDClock(int steps, double dt) : _dt(dt), _nsteps(steps), _step(0) {}
	 
   /**
    * Lenght of time step.
    */
   double QDClock::Dt() const
   {
      return _dt;
   }
   
   /**
    * Set the length of time step.
    */
   void QDClock::Dt(double dt)
   {
      _dt = dt;
   }
	 
   /**
    * The actual system time.
    */
   double QDClock::Time() const
   {
      return _step * _dt;
   }
   
   /**
    * Number of actual time step.
    */
   int QDClock::TimeStep() const
   {
      return _step;
   }
   
   /**
    * Set the actual time step.
    */
   void QDClock::TimeStep(int step)
   {
      _step = step;
   }
   
   /**
    * Number of time steps.
    */
   int QDClock::Steps() const
   {
      return _nsteps;
   }
   
   /**
    * Set the number of time steps
    */
   void QDClock::Steps(int Steps)
   {
      _nsteps = Steps;
   }
	 
   
   /**
    * Set the clock to zero time.
    */
   void QDClock::Begin()
   {
      _step = 0;
   }
   
   /**
    * Set the clock to final time.
    */
   void QDClock::End()
   {
      _step = _nsteps-1;
   }
   
   /**
    * Increase the clock by one time step.
    */
   QDClock& QDClock::operator++()
   {
      _step++;
      return *this;
   }
   
   /**
    * Decrease the clock by one time step.
    */
   QDClock& QDClock::operator--()
   {
      _step--;
      return *this;
   }
   
   /**
    * Copy operator.
    */
   QDClock& QDClock::operator=(const QDClock &clock)
   {
      _dt = clock._dt;
      _nsteps = clock._nsteps;
      _step = clock._step;
      return *this;
   }
   
   /**
    * Synchronize time to given clock.
    */
   void QDClock::Sync(const QDClock &master)
   {
      double step = master.Time() / _dt;

      if ( (step - round(step)) != 0)
         throw (EParamProblem("Clock sync error: non integer step"));

      _step = int(step);
   }
}
