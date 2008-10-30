#include "QDClock.h"

namespace QDLIB
{
   QDClock::QDClock() : _dt(0), _nsteps(0), _step(0) {}
   
   QDClock::QDClock(int steps, double dt) : _dt(dt), _nsteps(steps), _step(0) {}
	 
   /**
    * \return lenght of time step.
    */
   inline double QDClock::Dt()
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
    * \return The actual system time.
    */
   inline double QDClock::Time()
   {
      return _step * _dt;
   }
   
   /**
    * \return number of actual time step.
    */
   inline int QDClock::TimeStep()
   {
      return _step;
   }
   
   /**
    * \return number of time steps.
    */
   inline int QDClock::Steps()
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
   }
   
}
