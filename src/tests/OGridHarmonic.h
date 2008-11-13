#ifndef QDLIBOGRIDHARMONIC_H
#define QDLIBOGRIDHARMONIC_H

#include "qdlib/OGridPotential.h"

namespace QDLIB
{

   /**
   	@author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class OGridHarmonic : public OGridPotential
   {
      private:
	 double _k[MAX_DIMS];
      public:
         OGridHarmonic();

         ~OGridHarmonic();
	 
	 virtual void Init( ParamContainer& params );
   };

}

#endif
