#ifndef QDLIBOPROPAGATOR_H
#define QDLIBOPROPAGATOR_H

#include "Operator.h"
#include "sys/QDClock.h"
#include "math/dcomplex.h"

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
   class OPropagator : public Operator
   {
      private:
	 dcomplex _c0;
      protected:
	 /** Indicates forward propagation. */
	 bool forward;
	 /** Indicates imaginary time propagation. */
	 bool imaginary;
	 /** Holds the clock of the propagator. */
         QDClock *clock;
      public:
	 OPropagator() : _c0(0,0), forward(true), imaginary(false), clock(NULL)
	 {
	 }
      
	 ~OPropagator() {}
	    
	 /**
	 * Should return a ParameterContainer reference which contains some keywords.
	 * 
	 * This keywords are thougt to distinguish between different parts
	 * of the Hamilton. Only the keys are important. The values can be empty.
	  * The rest of the parameters are done as usual with Init().
	 */
	 virtual ParamContainer& TellNeeds() = 0;
	 
	 /**
	  * Now we can add the needs of the propagator.
	  * 
	  * \param Key One of the access keys which was returned from TellNeeds()
	  * \param O   An operator associated with Key
	  */
	 virtual void AddNeeds(string &Key, Operator *O) = 0;
	 
	 /**
	  * This is used to init the propagator after changing the exponent.
	  * (Clock, Forward, Backward, ImaginaryTime, RealTime, Exponent).
	  */
	 virtual void ReInit() = 0;
	 
	 /**
	  * Set propagators clock.
	  */
	 void Clock(QDClock* cl)
	 {
	    clock = cl;
	 }
	 
	 
	 /**
	  * \return propagators clock.
	  */
	 QDClock* Clock()
	 {
	    return clock;
	 }
	 
	 /**
	  * Set forward propagation.
	  * 
	  * This is the default behavior.
	  */
	 void Forward()
	 {
	    if (imaginary)
	       _c0 = clock->Dt();
	    else
	       _c0 = I*clock->Dt();
	    forward = true;
	 }
	 
	 /**
	  * Set backward propagation.
	  */
	 void Backward()
	 {
	    if (imaginary)
	       _c0 = -clock->Dt();
	    else
	       _c0 = (-1)* I *clock->Dt();
	    forward = false;
	 }
	 	 
	 /**
	  * Set propagation in imaginary time.
	  */
	 void ImaginaryTime()
	 {
	    if (forward)
	       _c0 = -clock->Dt();
	    else
	       _c0 = clock->Dt();
	    imaginary = true;
	 }
	 
	 /**
	  * Set propagation in real time.
	  * 
	  * Default behavior.
	  */
	 void RealTime()
	 {
	    if (forward)
	       _c0 =  I * clock->Dt();
	    else
	       _c0 = (-1) * I * clock->Dt();
	    imaginary = false;
	 }
	 
	
	 /**
	  * Set the exponent explicitly.
	  * 
	  * \f$ exp^{c H dt} \f$
	  */
	 void Exponent(dcomplex c)
	 {
	    _c0 = c;
	 }
	 
 	 /**
	  * Get the exponent.
	  */
	 dcomplex Exponent()
	 {
	    return _c0;
	 }

   };

}

#endif
