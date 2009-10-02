#ifndef QDLIBOPROJECTION_H
#define QDLIBOPROJECTION_H

#include "Operator.h"

#define MAX_WFSPACE 1024 /* Maximum number of storable WFs */

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
    * 
    * @author Markus Kowalewski
    */
   class OProjection : public Operator {
      private:
	 string _name;
	 
         double _sign;
	 int _size;
	 WaveFunction* _wfbuf[MAX_WFSPACE];
	 WaveFunction* _buf;
	 
	 void _destroy();
	 
      public:
	 OProjection();
      
	 ~OProjection();
      
	 void Add(WaveFunction* Psi);
	 
	 WaveFunction* Get(int n);
	 
         /** Number of elements. */
	 int Length() { return _size; };
	 
         /** Set sign of the sum */
         void Sign(bool positive) {
            if (positive) _sign=1;
            else _sign=-1;
         }
	 
	 /* Interface implementation, Operator */ 
   
	 virtual Operator* NewInstance() ;
		  
	 virtual string& Name();
	 
	 virtual void Init(ParamContainer &params);
	    
	 virtual void Init(WaveFunction *Psi);
   
	 virtual void UpdateTime(){};
	    
	 virtual dcomplex MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet);
	    
	 virtual double Expec(WaveFunction *Psi);
	    
	 virtual double Emax() {return 0;}
	    
	 virtual double Emin() {return 0;}
	    
	 virtual WaveFunction* Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);
	    
	 virtual WaveFunction* Apply(WaveFunction *Psi);
		  
	 virtual Operator* operator=(Operator* O);
	 
	 virtual Operator* Copy(Operator* O);
	    
	 virtual Operator* operator*(Operator* O);
      
	 virtual Operator* Offset(const double d) {return (Operator*) this; /* No Offset! */};
   
	 virtual Operator* Scale(const double d);
      
      
   };

}

#endif
