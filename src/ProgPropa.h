#ifndef QDLIBPROGPROPA_H
#define QDLIBPROGPROPA_H

#include "sys/Exception.h"
#include "sys/XmlNode.h"
#include "sys/QDGlobalClock.h"

#define DEFAULT_WRITE_CYCLE 10

namespace QDLIB {

   /**
    * This is the main module for a propagagation run.
    * 
    * XmlNode Attributes:
    * \li dt      time step
    * \li steps   number of time steps
    * \li wcycle  number of step to make until a wf will be written and the reporter will analyze.
    * 
    * @author Markus Kowalewski
    */
   class ProgPropa {
      private:
	 XmlNode &_propaNode;
	 
	 int _wcycle;     /* Write and analyze rate */
	 
	 void _InitParams();
      public:
	 ProgPropa(XmlNode &PropaNode);
	 ~ProgPropa();
	 
	 void Run();
   
   };

}

#endif
