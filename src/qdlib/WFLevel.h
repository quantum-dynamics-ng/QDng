#ifndef WFLEVEL_H
#define WFLEVEL_H

#include "qdlib/WaveFunction.h"

namespace QDLIB {
   
   /**
    * Wavefunction class for a simple level system.
    */
   class WFLevel: public WaveFunction
   {
      private:
	 string _name;
	 
      public:
         /* Interface implementation */
	 WFLevel();
	 
 	 ~WFLevel();
	 
	 WaveFunction* NewInstance();
	 
         virtual void Init(ParamContainer &params);
	 
         virtual const string& Name();
	 
         virtual double Norm();
	 
         virtual void Normalize();
	 
	 WaveFunction* operator=(WaveFunction *Psi);
	 
	 dcomplex operator*(WaveFunction *Psi);
	 
         virtual void Reduce(double tolerance);
         
         virtual void Restore();
	 
	 /* Own methods */
	 void Size(int size);
	 
	 WFLevel& operator=(WFLevel &Psi);
	 
         
	
   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */
