#ifndef QDLIBOPROPAGATOR_H
#define QDLIBOPROPAGATOR_H

#include "qdlib/OList.h"


namespace QDLIB {

   /**
    * Abstract base class for propagators.
    * 
    * Represents a expansion/realisation for \f$ e^{c H dt}\f$.
    * Where c can be configured for forward or backward propagation (sign of c)
    * or propagation in real and imaginary time (c=i or c=1).
    * 
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class OPropagator : public OList
   {
      private:
	 dcomplex _c0;
      protected:
	 /** Indicates forward propagation. */
	 bool forward;
	 /** Indicates imaginary time propagation. */
	 bool imaginary;
         /** The associated Hamiltonian
          * Must initialized by the implementing class after AddNeed is finished
          */
         Operator* H;
      public:
	 OPropagator() : _c0(0,0), forward(true), imaginary(false), H(NULL)
	 {
	 }
      
	 ~OPropagator() { DELETE_OP(H); }
	    
	 
	 virtual void Clock(QDClock *cl)
	 {
	    clock = cl;
	    if (clock->Dt() > 0) forward = true;
	    else {
	       forward = false;
	       _conj = true;
	       OList::Conj(true);
	    }

	    _c0 = (-1)*I*clock->Dt();
	 }
	 	 
	 /**
	  * Set forward propagation.
	  * 
	  * This is the default behavior.
	  */
	 void Forward()
	 {
	    if (clock == NULL)
	       throw (EIncompatible("Propagator has no clock"));

	    if (imaginary)
	       _c0 = clock->Dt();
	    else
	       _c0 = (-1)*I*clock->Dt();

	    forward = true;

	    _conj = false;

	    OList::Conj(false);
	 }
	 
	 /**
	  * Set backward propagation.
	  */
	 void Backward()
	 {
       if (clock == NULL)
          throw (EIncompatible("Propagator has no clock"));

	    if (imaginary)
	       _c0 = -clock->Dt();
	    else
	       _c0 = (1)* I *clock->Dt();

	    forward = false;

	    _conj = true;

	    OList::Conj(true);
	 }
	 	 
	 /**
	  * Set propagation in imaginary time.
	  */
	 void ImaginaryTime()
	 {
	    if (forward)
	       _c0 = clock->Dt();
	    else
	       _c0 = (-1) * clock->Dt();
	    imaginary = true;
	 }
	 
	 virtual void Conj(bool conj)
	 {
	    if (conj)
	       Backward();
	    else
	       Forward();
	 }

	 /**
	  * Set propagation in real time.
	  * 
	  * Default behavior.
	  */
	 void RealTime()
	 {
	    if (forward)
	       _c0 =  (-1) * I * clock->Dt();
	    else
	       _c0 =  I * clock->Dt();
	    imaginary = false;
	 }
	 
	
	 /**
	  * Set the exponent explicitly.
	  * 
	  * \f$ exp^{c H dt} \f$
	  */
	 inline void Exponent(dcomplex c)
	 {
	    _c0 = c;
	 }
	 
 	 /**
	  * Get the exponent.
	  */
	 inline dcomplex Exponent()
	 {
	    return _c0;
	 }

	 /* This doesn't make sense with Propagators => define dummies. */
	 virtual dcomplex Emax(){return 0;}
	 virtual dcomplex Emin(){return 0;}
	 
	 virtual Operator* Copy(Operator *O)
	 {
	   
	    OPropagator *P = dynamic_cast<OPropagator*>(O);
	    clock = P->clock;
	    _c0 = P->_c0;
	    forward = P->forward;
	    imaginary = P->imaginary;
	   
	    OList::Copy(O);
	    return this;
	 }	 
	 
	 Operator* Hamiltonian() { return H; }

	 void Init(WaveFunction* Psi)
    {
      if (clock == NULL)
         throw(EParamProblem("Progagator is missing a clock"));

      if (clock->Dt() == 0)
         throw(EParamProblem("No time step defined"));

      if (Psi == NULL)
          throw(EParamProblem("Invalid WaveFunction"));

      OList::Init(Psi);
    }

   };

}

#endif
