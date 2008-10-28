#ifndef QDLIBOPROPAGATOR_H
#define QDLIBOPROPAGATOR_H

#include "Operator.h"
#include "sys/QDClock.h"

namespace QDLIB {

   /**
   * Abstract base class for propagators.
   * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
   */
   class OPropagator : public Operator
   {
      private:
	 QDClock *_clock;
      public:
	 OPropagator() : _clock(NULL) {}
      
	 ~OPropagator();
	    
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
	  * Set the clock of the propagator.
	  */
	 void Clock(const QDClock *clock)
	 {
	    _clock = clock;
	 }
	 
	 
	 /**
	  * \return propagators clock.
	  */
	 QDClock* Clock()
	 {
	    return _clock;
	 }
	 
	 
	 
   };

}

#endif
