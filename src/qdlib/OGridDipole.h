#ifndef OGRIDDIPOLE_H
#define OGRIDDIPOLE_H

#include "OGridPotential.h"
#include "Laser.h"

namespace QDLIB {
   
   /**
    * Class which represents a dipole interaction on the grid.
    * 
    * Parameters:
    * \li laser   the file containing the laser field.
    * \li All other parameters from GridPotential
    */
   class OGridDipole : public OGridPotential
   {
      private:
	       
	 string _name;
         Laser _laser;
	 bool _init;
      public:
         void SetLaser(Laser &laser);
	 Laser& GetLaser();
	 
	 OGridDipole();
	 
	 ~OGridDipole() {}
	 
	       virtual void Clock ( QDClock *cl);
	 
	 virtual Operator* NewInstance();
	 
	 virtual void Init(ParamContainer &params);
	 
	 virtual void UpdateTime();
	 
	 virtual double Emax();
	 
	 virtual double Emin();
	 
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	
	 virtual WaveFunction* Apply(WaveFunction *Psi);

	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);

   };

} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDDIPOLE_H */
