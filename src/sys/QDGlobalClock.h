#ifndef QDLIBQDGLOBALCLOCK_H
#define QDLIBQDGLOBALCLOCK_H

#include "QDClock.h"

namespace QDLIB {

/**
 * Singleton version for QDClock.
 *
 * This is intended to be used as global clock.
 * \author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
   class QDGlobalClock
   {
      private:
	 static QDClock *_ref;
	 /* Make sure that class is neither constructable nor destructable. */ 
	 QDGlobalClock() {}
	 QDGlobalClock(const QDGlobalClock &clock) {}
	 ~QDGlobalClock() {}
	 
      public:
	 
	 /**
	  * \return Pointer to singleton instance of QDGlobalClock.
	  */
	 static QDClock* Instance()
	 {
	    
	    if (!_ref){
	       _ref = new QDClock();
	    }
	    return _ref;
	 }
   
   };
   
  QDClock* QDGlobalClock::_ref = 0;
}

#endif
