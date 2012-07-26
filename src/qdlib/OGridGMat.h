#ifndef QDLIBOGRIDGMAT_H
#define QDLIBOGRIDGMAT_H

#include "qdlib/OGridPotential.h"
#include "qdlib/WFGridSystem.h"
#include "qdlib/Diff.h"
#include "qdlib/PML.h"

namespace QDLIB {

   /**
    * Kinetic energy Operator for the G-matrix method.
    * 
    * parameters:
    * \li dims   Number of internal coordiantes
    * \li gmat   G-matrix element base file name
    * \li coup   Turn on kinetic coupling [default: true]
    * \li G      Constant G-matrix elements (given as lower triangular lin. matrix)
    * \li Gdef   Define source of matrix elements:
    *            c = constant, g = grided, x=none (=0)
    *            (given as lower triangular lin. matrix)
    *
    * @author Markus Kowalewski
    */
   class OGridGMat : public Operator, public GridSystem
   {
      private:
         typedef Matrix<OGridPotential> Gmatrix_t;
         typedef Vector<dVec> Gkspace_t;
         typedef enum {none, cnst, grd} source_t;  /* defines source of G-matrix elements */

         string _name;
         lint _size;
         OGridPotential*** _Gmat; /* 2D Array coordinate dependent elements*/
         dMat _GmatC; /* 2D Array constant G elements */
         Matrix<source_t> _src;
         dVec** _GmatDiff;
         dVec* _kspace;
         WFGridSystem** _wfbuf; /* 1D Array */
         WFGridSystem *buf;
         WaveFunction *buf1;
         WaveFunction* _Abuf;

         Diff _diff; /* differenciator object */
         PML*  _pml; /* Optional PML objects */

         bool _KinCoup;
         bool _ChainRule;  /* Re-arange derivatives by chain rule */
      public:
         OGridGMat();

         ~OGridGMat();

         /* Interface implementation */
         virtual Operator*
         NewInstance();

         virtual void Init(ParamContainer &params);

         virtual void Init(WaveFunction *Psi);

         virtual const string& Name();

         virtual void UpdateTime() {} /* Nothing to do here */

         virtual dcomplex Emax();

         virtual dcomplex Emin();

         virtual void Apply(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void ApplyAdd(WaveFunction *destPsi, WaveFunction *sourcePsi);

         virtual void Apply(WaveFunction *Psi);

         virtual Operator* operator=(Operator* O);

         virtual Operator* Copy(Operator* O);

         virtual bool Valid(WaveFunction *Psi);
   };

}

#endif
