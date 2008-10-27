#ifndef WFCartesian1D_H
#define WFCartesian1D_H


#include "tnt.h"
#include "WaveFunction.h"

using namespace TNT;

namespace QDLIB {

  /**
   * WF on 1D Cartesian Grid.
   */
  class WFCartesian1D : public Wavefunction {
    public:
      
      /*
       *  Interface implemenation
       */
      
      const string& getName() { return string "WF_Cartesian_1D" };
      
      double Norm();
      double Normalize();
      
      /** Copy. */
      WaveFunction operator=(WaveFunction &Psi);
      /** Scalar product */
      dcomplex operator*(WaveFunction &Psi);
      /** Point Product. */
       WaveFunction& operator*(WaveFunction &Psi);
       
      /*
       *  specific extension
       */
       
       
  };

} /* namespace QDLIB */

#endif /* WFCartesian1D_H */