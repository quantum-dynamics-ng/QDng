#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "qdlib/WaveFunction.h"

namespace QDLIB {
   /**
    * Generic Class for WF-Basistransformations.
    *  @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class Transform {
   public:
      
      /**
       * Forward transformation into foreign basis.
       */
      virtual void Forward(WaveFunction *Psi) = 0;
      
      /**
       * Inverse transformation back into original basis.
       */      
      virtual void Backward(WaveFunction *Psi) = 0;
   
      /**
       * The factor which normalizes a one-way transformation.
       * 
       * The correct value is available after use of forward/backward.
       * (that means for the last transformation)
       */
      virtual double Normalization() = 0;
   }; /* class Transform */

} /* namespace QDLIB */
#endif
