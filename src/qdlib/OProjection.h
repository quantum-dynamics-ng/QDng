#ifndef QDLIBOPROJECTION_H
#define QDLIBOPROJECTION_H

#include "qdlib/Operator.h"
#include "WFBuffer.h"

namespace QDLIB {

   /**
    * Projection operator.
    * 
    * \f$ \sum_i |\Psi_i><\Psi_i| \f$
    * 
    * parameters:
    * \li positive Set the sign of the projector (false=negative)
    * \li files   Read wavefunction set from a sequence of files
    * \li num     number of files to read
    * \li start   number to start with
    * \li step    step size trough the sequence
    * \li inv     sets the inverse Projection operator \f$ \sum_i |1-\Psi_i><\Psi_i| \f$
    * @author Markus Kowalewski
    */
   class OProjection : public Operator, public WFBuffer {
      private:
	 string _name;
	 
         double _sign;
         bool _inv;        /* true: inverse projection 1-P */
	 WaveFunction* _buf;
	 
	 void _destroy();
	 
      public:
	 OProjection();
      
	 ~OProjection();
	 
         /** Number of elements. */
	 int Length() { return Size(); };
	 
         /** Set sign of the sum */
         void Sign(bool positive) {
            if (positive) _sign=1;
            else _sign=-1;
         }
	 
	 /* Interface implementation, Operator */ 
   
	 virtual Operator* NewInstance() ;
		  
	 virtual const string& Name();
	 
	 virtual void Init(ParamContainer &params);
	    
	 virtual void Init(WaveFunction *Psi);
   
	 virtual void UpdateTime(){};
	    
	 virtual dcomplex Emax() {return 1;}
	    
	 virtual dcomplex Emin() {return 0;}
	    
	 virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	    
	 virtual void Apply(WaveFunction *Psi);
		  
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	    
         virtual bool Valid(WaveFunction *Psi);
      
      
   };

}

#endif
