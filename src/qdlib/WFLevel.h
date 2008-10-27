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
	 
 	 virtual ~WFLevel();
	 
	 WFLevel(ParamContainer &params);
	 
	 WFLevel(int size);

	 WaveFunction* NewInstance();
	 
	 void Init(ParamContainer &params);
	 
	 const string& Name();
	 
	 int ID();
	 
	 double Norm();
	 
	 void Normalize();
	 
	 WaveFunction* operator=(WaveFunction *Psi);
	 
	 dcomplex operator*(WaveFunction *Psi);
	 
	 void Size(int size);
	 
	 WFLevel& operator=(WFLevel &Psi);
	
   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */
