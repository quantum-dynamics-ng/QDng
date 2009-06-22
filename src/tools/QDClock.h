#ifndef QDCLOCK_H
#define QDCLOCK_H

namespace QDLIB
{
   /**
    * Simple clock class.
    */
   class QDClock
   {
      private:
	 double _dt;
	 int _nsteps;
	 int _step;
      public:
	 
	 QDClock();
	 QDClock(int steps, double dt);
	 
	 double Dt();
	 void Dt(double dt);
	 
	 double Time();
	 
	 int TimeStep();
	 int Steps();
	 
	 void Steps(int Steps); 
	 
	 void Begin();
	 void End();
	 
	 QDClock& operator++();
	 QDClock& operator--();
	 QDClock& operator=(const QDClock &clock);
   };
}

#endif
