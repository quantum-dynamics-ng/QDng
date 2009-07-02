#ifndef OGRIDSYSTEM_H
#define OGRIDSYSTEM_H

#include "Operator.h"
#include "GridSystem.h"
#include "tools/FileSingle.h"



namespace QDLIB {
  
   
   /**
    * Abstract class to represent real number operators on a grid.
    */
   class OGridSystem : public Operator, public dVec, public GridSystem
   {
      public:
	 typedef FileSingle<OGridSystem> FileOGrid;
      private:
	 FileOGrid *_file;
	 
      public:
	 
	 OGridSystem();
	 ~OGridSystem();
	 
	 FileOGrid* File();
	 
	 virtual Operator* Offset(const double d);

	 virtual Operator* Scale(const double d);
	 
	 virtual Operator* Copy(Operator* O);
   };

   
   
} /* namespace QDLIB */
      
#endif /* #ifndef OGRIDSYSTEM_H */