// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OGridsystem.h"
#include "WFGridSystem.h"
#include "tools/ZCopyStream.h"


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


   void OGridSystem::Serialize (std::ostream& os)
   {
      // Write header
      grid_sys.header.set_data_size(sizeBytes());

      uint32_t size = grid_sys.header.ByteSizeLong();
      os.write(reinterpret_cast<char*>(&size), sizeof(size));

      if (! grid_sys.header.SerializeToOstream(&os) )
         throw(EIOError("Can't write Operator to stream"));

      // Write data
      os.write(reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

   void OGridSystem::DeSerialize (std::istream& is)
   {
      // read header
      uint32_t size;

      is.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

      char* buf = new char[size];
      is.read(buf, size);
      if (! grid_sys.header.ParseFromArray(buf, size) )
         throw(EIOError("Can't read WF from stream"));

      delete[] buf;

      newsize(GridSystem::Size());
      grid_sys.header.set_data_size(GridSystem::Size() * sizeof(*begin(0)));

      is.read(reinterpret_cast<char*>(begin(0)), sizeBytes());
   }

} /* namespace QDLIB */





