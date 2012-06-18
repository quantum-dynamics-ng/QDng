/*
 * PML.h
 *
 *  Created on: Jun 18, 2012
 *      Author: markus@math.uu.se
 */

#ifndef PML_H_
#define PML_H_

#include "qdlib/GridSystem.h"
#include "tools/ParamContainer.h"

namespace QDLIB
{

   /**
    * Implements the PML metric operation.
    *
    * This class applies to a single dimension.
    */
   class PML
   {
      private:
         GridSystem _grid;

         int _dim;
         double _gamma;
         double _smax;
         int _n;
         int _thick;

      public:
         PML();
         virtual ~PML();

         void InitParams(ParamContainer &pm, uint dim);
         void SetGrid(GridSystem &grid) { _grid = grid; }

         void ApplyTransform(cVec *wf);
   };

} /* namespace QDLIB */
#endif /* PML_H_ */
