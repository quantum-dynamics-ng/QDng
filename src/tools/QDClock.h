// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDCLOCK_H
#define QDCLOCK_H

namespace QDLIB
{
   /**
    * Simple class representing clock for QD operations.
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

         double Dt() const;
         void Dt(double dt);

         double Time() const;

         int TimeStep() const;
         void TimeStep(int step);
         int Steps() const;

         void Steps(int Steps);

         void Begin();
         void End();

         QDClock& operator++();
         QDClock& operator--();
         QDClock& operator=(const QDClock &clock);

         void Sync(const QDClock &master);
   };
}

#endif
