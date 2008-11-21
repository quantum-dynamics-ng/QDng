#include "OGridsystem.h"

namespace QDLIB {
   
   OGridSystem::OGridSystem() : _file(NULL) {}
   
   OGridSystem::~OGridSystem()
   {
      if (_file != NULL) delete _file;
   }
	 
   /**
    * Provides Acces to the associated Filewriter.
    */
   OGridSystem::FileOGrid* OGridSystem::File()
   {
      if (_file == NULL) _file = new FileOGrid();
      return _file;
   }

   Operator* OGridSystem::operator +=(const double d)
   {
      for(lint i=0; i < lsize(); i++){
	 (*this)[i] += d;
      }
      return this;
   }

   Operator* OGridSystem::operator -=(const double d)
   {
      for(lint i=0; i < lsize(); i++){
	 (*this)[i] -= d;
      }
      return this;
   }

   Operator* OGridSystem::operator *=(const double d)
   {
      MultElements( (dVec*) this, d );
      return this;
   }
   
   
} /* namespace QDLIB */


