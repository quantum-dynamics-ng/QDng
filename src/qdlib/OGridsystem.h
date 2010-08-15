#ifndef OGRIDSYSTEM_H
#define OGRIDSYSTEM_H

#include "ODSpace.h"
#include "GridSystem.h"
#include "tools/FileSingle.h"

namespace QDLIB
{

   /**
    * Abstract class to represent real number operators on a grid.
    */
   class OGridSystem: public ODSpace, public dVec, public GridSystem
   {
      public:
         typedef FileSingle<OGridSystem> FileOGrid;
      private:
         FileOGrid *_file;
      public:

         OGridSystem();
         ~OGridSystem();

         FileOGrid* File();

         virtual Operator* Copy(Operator* O);

         virtual bool Valid(WaveFunction *Psi);

         /* DSpace interface */
         virtual void InitDspace();
         virtual Transform* Transformation()
         {
            return NULL;
         }

   };

} /* namespace QDLIB */

#endif /* #ifndef OGRIDSYSTEM_H */
