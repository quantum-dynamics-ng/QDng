#ifndef QDLIBOLASER_H
#define QDLIBOLASER_H

#include "qdlib/Operator.h"
#include "qdlib/Laser.h"

namespace QDLIB {

   /**
    * Abstract base class for all operator types using an arbitrary number of laser fields.
    *
    * This class serves as a container for laser fields as well as a uniform interface to access
    * the laser for external needs (OCT e.g.)
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OLaser : private virtual Operator
   {
      private:
	 int _numlasers;
	 Laser* _laser;        /* Laserpointer :) */
	 dVec* _Pol;           /* Polarisation vectors */
	 bool _UsePol;         /* Use the polarisation vecotors ? */
	 
      protected:
	 void UsePol(bool use);
	 void NumLaser(int num);
	 
      public:
	 OLaser();
	 OLaser(int num);
	 
	 ~OLaser();
	 
	 void SetLaser(Laser &laser);
	 void SetLaser(int n, Laser &laser);
	 
	 Laser* GetLaser() { return &(_laser[0]); } 
	 Laser* GetLaser(int n) { return &(_laser[n]); } 
	 
	 /* Return the number of lasers in use */
	 int NumLasers() { return _numlasers; }
   
	 bool UsePol() { return _UsePol; }
	 
	 dVec& GetPol() { return _Pol[0]; } 
	 dVec& GetPol(int n) { return _Pol[n]; } 
	 
	 void SetPol(dVec &pol);
	 void SetPol(int n, dVec &pol);
   };

}

#endif
