#include "OGridsystem.h"

namespace QDLIB {
   
   OGridSystem::OGridSystem() : _file(NULL) {cout << "OGridSystem::OGridSystem()\n";}
   
   OGridSystem::~OGridSystem()
   {
      cout << "OGridSystem::~OGridSystem()\n";
      cout << "_file: " << _file << endl;
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

   Operator* OGridSystem::Offset(const double d)
   {
      for(lint i=0; i < lsize(); i++){
	 (*this)[i] += d;
      }
      return this;
   }


   Operator* OGridSystem::Scale(const double d)
   {
      MultElements( (dVec*) this, d );
      return this;
   }
   
   
   Operator* OGridSystem::Copy(Operator * O)
   {
      OGridSystem *o = dynamic_cast<OGridSystem*>(O);
      
      /* Copy vector */
      *((dVec*) this) = *((dVec*) o);
      
      /* Copy Grid description */
      *((GridSystem*) this) = *((GridSystem*) o);
      
      return this;
   }

   
} /* namespace QDLIB */



