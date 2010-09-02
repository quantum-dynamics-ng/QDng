#include "OLaser.h"

namespace QDLIB {

   OLaser::OLaser() : _numlasers(1), _Pol(NULL), _UsePol(false)
   {
      _laser = new Laser[1];
      _isTimedependent = true;
   }
   
   QDLIB::OLaser::OLaser(int num) :  _numlasers(num), _Pol(NULL), _UsePol(false)
   {
      _laser = new Laser[num];
   }
   
   OLaser::~OLaser()
   {
      delete[] _laser;
      if (_Pol != NULL) delete[] _Pol;
   }

   /**
    * This decides wether the to use polarisation vectors or not.
    * 
    * All former vectors will be destroyed.
    */
   void OLaser::UsePol(bool use)
   {
      _UsePol = use;
      
      if (use){
	 if (_Pol != NULL) delete[] _Pol;
	 _Pol = new dVec[_numlasers];
      }
   }


   /**
    * Set the number of laser fields.
    * 
    * All previously contained fields will be destroyed.
    */
   void OLaser::NumLaser(int num)
   {
      delete[] _laser;
      if (_Pol != NULL) delete[] _Pol;
      
      _laser = new Laser[num];
      if (_UsePol) _Pol = new dVec[num];
      
      _numlasers = num;
   }

   /**
    * Set Laser (index=0).
    */
   void OLaser::SetLaser(Laser & laser)
   {
      SetLaser(0, laser);
   }

   /**
    * Set Laser with index n.
    */
   void OLaser::SetLaser(int n, Laser & laser)
   {
      _laser[n] = laser;
   }
   
   
   void OLaser::SetPol(dVec & pol)
   {
      SetPol(0, pol);
   }

   void OLaser::SetPol(int n, dVec & pol)
   {
      _Pol[n] = pol;
   }
   
}






