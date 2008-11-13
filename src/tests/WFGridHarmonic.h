#ifndef QDLIBWFGRIDHARMONIC_H
#define QDLIBWFGRIDHARMONIC_H

#include "qdlib/WFGridCartesian.h"

namespace QDLIB
{

   /**
    * Generate Harmonic Oszillator Groundstate in Gridsystem.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class WFGridHarmonic : public WFGridCartesian
   {
      private:
	 double _mass[MAX_DIMS];
	 double _k[MAX_DIMS];
      public:
         WFGridHarmonic();
         ~WFGridHarmonic();

	 virtual void Init( ParamContainer& params );
   };

}

#endif
