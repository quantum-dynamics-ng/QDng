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

         int _dim;         /* Applies to dim */
         double _gamma;    /* Phase factor for scaling KO */
         double _smax;     /* sigma_max */
         int _p;           /* degree of polynomial */
         int _thick;       /* thickness of layer in grid points */
         int _side;        /* Activate at begin (1) and/or end (2) */
         cVec _f1;         /* Layer metric 1/f */
         cVec _df1;        /* Layer metric derivative d/dx 1/f */

      public:
         PML();
         virtual ~PML();

         void InitParams(ParamContainer &pm, uint dim);
         void SetGrid(GridSystem &grid) { _grid = grid; }

         int Layer() { return _thick; }
         double gamma() { return _gamma; }
         double sigma() { return _smax; }
         int p() { return _p; }

         const cVec& f1() { return _f1; }

         void ApplyTransformAdd(cVec *dwf, cVec *wf);
         void ApplyTransform(cVec *wf);

         void ApplyTransformDiff(cVec* wf);
   };

} /* namespace QDLIB */
#endif /* PML_H_ */
