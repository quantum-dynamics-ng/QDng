#ifndef QDLIBOGRIDGMAT_H
#define QDLIBOGRIDGMAT_H

#include <OGridsystem.h>

namespace QDLIB {

   /**
    * Kinetic energie Operator for the G-matrix method.
    * 
    * @author Markus Kowalewski
    */
   class OGridGMat : public OGridSystem
   {
      private:
	 typedef Matrix<OGridSystem> Gmatrix_t;
	 typedef Matrix<dVec> Gkspace_t;
	 
	 Gmatrix_t _Gmat;
	 Gkspace_t _kspace;
	 
      public:
	 OGridGMat();
      
	 ~OGridGMat();
   
   };

}

#endif
