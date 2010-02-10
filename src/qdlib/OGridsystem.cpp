#include "OGridsystem.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGridSystem::OGridSystem() : ODSpace(), _file(NULL) {}

   OGridSystem::~OGridSystem()
   {
      if (_file != NULL) delete _file;
   }

   /**
    * Provides access to the associated File-writer.
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
      scaling = d;
      return this;
   }

   Operator* OGridSystem::Copy(Operator * O)
   {
      OGridSystem *o = dynamic_cast<OGridSystem*>(O);

      if (o == NULL)
         throw ( EIncompatible("Copy OGridSystem <- ", O->Name()) );

      /* Copy vector */
      *((dVec*) this) = *((dVec*) o);

      /* Copy Grid description */
      *((GridSystem*) this) = *((GridSystem*) o);

      return this;
   }

   bool OGridSystem::Valid(WaveFunction * Psi)
   {
      if ( Psi == NULL ) return false;

      WFGridSystem* Psi_GS = dynamic_cast<WFGridSystem*>(Psi);

      if ( Psi_GS == NULL ) return false;

      if ( *((GridSystem*) this) != *((GridSystem*) Psi_GS) ) return false;

      return true;
   }

   void OGridSystem::InitDspace()
   {
      _dspace = (dVec*) this;
   }

} /* namespace QDLIB */





