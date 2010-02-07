#ifndef ODMULTISTATE_H_
#define ODMULTISTATE_H_

#include "OMultistate.h"
#include "ODSpace.h"
#include "TransformMSD.h"

namespace QDLIB
{

   /**
    * Simple Extension to OMultistates for Diagonalization of Operators.
    *
    * This class only makes sense if every Matrix Element is already diagonal
    * (e.g. Grids in position representation)
    */
   class ODMultistate: public QDLIB::OMultistate, public QDLIB::ODSpace
   {
      private:
         dMat **_X; /* Eigenvector Matrices */
         TransformMSD _XT; /* Transformation object */
         int _state_size; /* Size of a single state */
      public:
         ODMultistate();
         virtual ~ODMultistate();

         virtual Operator* Scale(const double d)
         {
            /* Avoid disambigious virtual overload */
            OMultistate::Scale(d);
         }

         void InitDspace();

   };

}

#endif /* ODMULTISTATE_H_ */
