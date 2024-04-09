// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef LASER_H
#define LASER_H

#include "math/typedefs.h"
#include "tools/ParamContainer.h"
#include "tools/QDClock.h"
#include "fft/fft.h"
#include "tools/FileSingle.h"

namespace QDLIB
{

   /**
    * Class which represents a laser field.
    */
   class Laser : public dVec
   {
      public:
         typedef FileSingle<Laser> FileLaser;
      private:
         string _name;
         QDClock *_clock;
         double _dt;
         double _scale; /* User defined scaling factor */
         FFT *_fft;
         cVec *_spectrum;
         ParamContainer _params;

         FileLaser _file;
      public:

         Laser();

         virtual ~Laser();

         FileLaser& File();

         void Init(ParamContainer &params);

         /* Return the parameter container */
         ParamContainer& Params() { return _params; }

         string& Name();

         /* Set scaling factor.
          * This factor is included in the data when using get.
          * The raw data is not modified.
          */
         void Scale(double d) { _scale = d; }

         /* Get scaling factor */
         double Scale() { return _scale; }

         dVec* PowerSpectrum();

         cVec* Spectrum();

         void ToTimeDomain();

         void Clock(QDClock *clock);

         QDClock* Clock();

         double Get();

         void Set(double value);

         double GetVectorPot();

         Laser& operator=(const Laser &laser);

         double PulseEnergy();

         double Dt() { return _dt; }

         void Dt(double dt);

         int Nt() { return dVec::size(); }

         void Nt(int size);

         virtual void Serialize (std::ostream& os);

         virtual void DeSerialize (std::istream& is);
   };

} /* namespace QDLIB */

#endif /* #ifndef LASER_H */
