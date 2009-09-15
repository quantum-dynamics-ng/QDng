#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "WaveFunction.h"

namespace QDLIB {
   /**
    * Generic Class for WF-Basistransformations.
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class Transform {
   public:
      Transform();
      ~Transform();
      
      /**
       * Forward transformation into foreign basis.
       */
      virtual void Forward(Wavefunction *Psi) = 0;
      
      /**
       * Inverse transformation back into original basis.
       */      
      virtual void Inverse(Wavefunction *Psi) = 0;
   
   }; /* class Transform */

} /* namespace QDLIB */
#endif
