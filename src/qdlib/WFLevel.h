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
	 bool _isDspace;
	 dMat *_D;      /* Matrix that transfroms to diagonal basis */
	 
      public:
         /* Interface implementation */
	 WFLevel();
	 
 	 ~WFLevel();
	 
	 WaveFunction* NewInstance();
	 
	 void Init(ParamContainer &params);
	 
	 const string& Name();
	 
	 double Norm();
	 
	 void Normalize();
	 
	 WaveFunction* operator=(WaveFunction *Psi);
	 
	 dcomplex operator*(WaveFunction *Psi);
	 
	 
	 /* Own methods */
	 void Size(int size);
	 
	 WFLevel& operator=(WFLevel &Psi);
	 
	 /** Check if in diagonal representation */
	 bool isDspace() { return _isDspace; }
	 
	 /** Set to representation */
	 void isDspace(bool is) { _isDspace = is; }
         
	 void SetTransform(dMat *mat) { _D = mat; }
	 
	 void ToDspace();
         
	 void ToXspace();
	
   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */
