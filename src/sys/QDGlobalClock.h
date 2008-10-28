#ifndef QDLIBQDGLOBALCLOCK_H
#define QDLIBQDGLOBALCLOCK_H

#include <QDClock.h>

namespace QDLIB {

/**
 * Singleton version of QDClock.
 *
 * This is intended to be used as global clock.
 * \author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
 */
   class QDGlobalClock : public QDClock
   {
      private:
	 static QDGlobalClock* _ref;
	 
	 /* Make sure that class is neither constructable nor destructable. */ 
	 QDGlobalClock() {}
	 QDGlobalClock(const QDGlobalClock &clock) {}
	 ~QDGlobalClock() {}
	 
      public:
	 
	 /**
	  * \return Pointer to singleton instance of QDGlobalClock.
	  */
	 static QDGlobalClock* Instance()
	 {
	    if (_ref == NULL){
	       _ref = new QDGlobalClock();
	    }
	    return _ref;
	 }
   
   };
   
   QDGlobalClock* QDGlobalClock::_ref = NULL;
}

#endif
