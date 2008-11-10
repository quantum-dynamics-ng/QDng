#ifndef OGRIDSYSTEM_H
#define OGRIDSYSTEM_H

#include "Operator.h"
#include "GridSystem.h"
#include "sys/FileSingle.h"



namespace QDLIB {
  
   
   /**
    * Abstract class to represent real number operators on a grid.
    */
   class OGridSystem : public Operator, public dVec, public GridSystem
   {
      public:
	 typedef FileSingle<OGridSystem> FileOGrid;
	 
	 OGridSystem() : _file(NULL) {}
	 ~OGridSystem()
	 {
	    if (_file != NULL) delete _file;
	 }
	 
	 inline FileOGrid* File()
	 {
	    if (_file == NULL) _file = new FileOGrid();
	    return _file;
	 }
      private:
	 FileOGrid *_file;
	       
   };

   
   
} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDSYSTEM_H */
