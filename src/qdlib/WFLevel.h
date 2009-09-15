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
	 
         virtual void Init(ParamContainer &params);
	 
         virtual const string& Name();
	 
         virtual double Norm();
	 
         virtual void Normalize();
	 
         virtual void ToKSpace(Operator *O);
         
         virtual void ToXSpace(Operator *O);

         
	 WaveFunction* operator=(WaveFunction *Psi);
	 
	 dcomplex operator*(WaveFunction *Psi);
	 
	 
	 /* Own methods */
	 void Size(int size);
	 
	 WFLevel& operator=(WFLevel &Psi);
	 
         
	
   }; /* class WFLevel */
   
} /* namespace QDLIB */

#endif /* #ifndef WFLEVEL_H */
