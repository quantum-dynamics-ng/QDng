#ifndef QDLIBOGOBBLER_H
#define QDLIBOGOBBLER_H

#include <OKSpace.h>

namespace QDLIB {

   /**
   * Gobbler operator which works with Butter-worth filter functions
   * @author Markus Kowalewski
   */
   class OGobbler : public OKSpace
   {
   public:
      OGobbler();
   
      ~OGobbler();
   
   };

}

#endif
