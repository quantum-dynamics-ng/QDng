#include "ODMultistate.h"
#include "linalg/LapackDiag.h"

namespace QDLIB
{

   ODMultistate::ODMultistate() :
      _state_size(0)
   {

   }

   ODMultistate::~ODMultistate()
   {
      for (int i = 0; i < _state_size; i++) {
         delete _X[i];
      }
   }

   void ODMultistate::InitDspace()
   {
      /* First init */
      if (_dspace == NULL) {
         _dspace = new dVec();
         _dspace->newsize(_state_size * States(), States());
         _X = new dMat*[_state_size];
         for (int i = 0; i < _state_size; i++) {
            _X[i] = new dMat(States(), States());
         }
      }
      dVec evals(States());

      /* diagonalize at every grid point */
      for (int i = 0; i < _state_size; i++) {
//         for (int k = 0; k < States(); k++) {
//            (X[i])(k,l)
//         }
         LAPACK::FullDiagHermitian(_X[i], &evals);
         for (int j = 0; j < States(); j++) {
            double *d = _dspace->begin(j);
            d[i] = evals[j];
         }
      }
      _XT.SetMatrixArray(_X);
   }
}
