#ifndef OSUM_H
#define OSUM_H

#include "qdlib/OList.h"

namespace QDLIB {
   
   /**
    * Represents a sum of arbitrary Operators.
    * 
    * The class stores operators and use it as a sum of them.
    * 
    * \f$ O_{sum} = \sum_i^{Size} \hat O_i \f$
    * 
    * All Operator objects will be destructed.
    */
   class OSum: public OList {
      private:
         string _name;
         WaveFunction* _WFbuf[MAX_OPS];    /* Use a buffer for wfs, to get rid of much mallocs */
      public:
         
         ~OSum();
	 
         OSum();
	 
         /* Interface implementation, Operator */
         virtual Operator* NewInstance();
	 	 
         virtual const string& Name();
	 
         virtual dcomplex Emax();
	 
         virtual dcomplex Emin();
	 
         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual void Apply(WaveFunction *Psi);
         
         virtual void ApplyParent(WaveFunction *destPsi, WaveFunction *sourcePsi);
	 
         virtual void Apply(Operator* destOp, Operator* sourceOp);
         
         virtual void Apply(Operator* Op);
         
         virtual Operator* Copy(Operator* O);

   };
   
} /* namespace QDLIB */

#endif /* #ifndef OSUM_H */
