#ifndef QDLIBWFGRIDCARTESIAN_H
#define QDLIBWFGRIDCARTESIAN_H

#include "qdlib/WFGridSystem.h"
#include "tools/FileSingle.h"

namespace QDLIB {

   /**
    * WaveFunction on cartesian grid.
    * 
    * Takes the following parameters:
    * \li dims
    * \li N#
    * \li xmin#
    * \li xmax#
    * 
    *  @author Markus Kowalewski
    */
   class WFGridCartesian : public WFGridSystem
   {
      friend class FileSingle<WaveFunction>;
      private:
         string _name;

      public:
         WFGridCartesian();

         ~WFGridCartesian();
         /* Interface implementation, WaveFunction */

         virtual WaveFunction* NewInstance();

         virtual void Init(ParamContainer &params);

         virtual const string& Name();

         virtual WaveFunction* operator=(WaveFunction* Psi);

         virtual dcomplex operator*(WaveFunction* Psi);

         virtual dcomplex Integral();
      };

}

#endif
