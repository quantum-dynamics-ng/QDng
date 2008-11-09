#ifndef QDLIBWFGRIDCARTESIAN_H
#define QDLIBWFGRIDCARTESIAN_H

#include <WFGridSystem.h>

namespace QDLIB {

   /**
    * WaveFunction on cartesian grid.
    *  @author Markus Kowalewski
    */
   class WFGridCartesian : public WFGridSystem
   {
      private:
	 string _name;
      public:
	 WFGridCartesian();
      
	 ~WFGridCartesian();
	    /* Interface implementation, WaveFunction */
      
	       virtual WaveFunction* NewInstance();
		     
	       virtual void Init(ParamContainer &params);
	       
	       virtual const string& Name();
	       
	       virtual int ID(){};
		  
	       virtual double Norm();
	       
	       virtual void Normalize();
	    
	       virtual WaveFunction* operator=(WaveFunction* Psi);
	       
	       virtual dcomplex operator*(WaveFunction* Psi);
      };

}

#endif
