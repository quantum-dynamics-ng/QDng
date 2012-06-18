/*
 * PML.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: markus
 */

#include "PML.h"

namespace QDLIB
{

   PML::PML() : _dim(-1), _gamma(0.25), _smax(1), _n(2), _thick(10)
   {
   }

   PML::~PML()
   {
   }

   /**
    * Read & check the parameters.
    */
   void PML::InitParams(ParamContainer& pm, uint dim)
   {
      _dim = dim;

      if (pm.isPresent("gamma")){
         vector<double> val;
         pm.GetArray("gamma", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: gamma for at least one dim is missing"));

         if (val.size() == 1)
            _gamma = val[0];
         else
            _gamma = val[dim];
      }

      if (pm.isPresent("thick")){
         vector<double> val;
         pm.GetArray("thick", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: thickness for at least one dim is missing"));

         if (val.size() == 1)
            _thick = val[0];
         else
            _thick = val[dim];
      }


      if (pm.isPresent("n")){
         vector<double> val;
         pm.GetArray("n", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: mononomial order for at least one dim is missing"));

         if (val.size() == 1)
            _n = val[0];
         else
            _n = val[dim];
      }

//      double k=M_PI/_grid.Dx(_dim)/2.; /** default is half k-max*/
//      if (pm.isPresent("k")){
//         vector<double> val;
//         pm.GetArray("k", val);
//         if (val.size() != 1 && dim > val.size())
//            throw (EParamProblem("PML: characteristic k-value order for at least one dim is missing"));
//
//         if (val.size() == 1)
//            k = val[0];
//         else
//            k = val[dim];
//      }


      /* Determine smax */
      if (pm.isPresent("smax"))
         pm.GetValue("smax", _smax);
      else {
         _smax = 1; /** \todo implement */
      }
   }

   /**
    * Apply the PML specific metric 1/f
    */
   void PML::ApplyTransform(cVec* wf)
   {
      _grid.ActiveDim(_dim);
      int lo = _grid.LowOthers();
      int Nx = _grid.DimSize(_dim);

      dcomplex phi = cexpI(_gamma*M_PI) * _smax * pow(1./double(_thick), _n);

      dcomplex* psi = wf->begin(0);
      for (int rep=0; rep < _grid.NumOthers(); rep++){
         int base = _grid.IndexBase(rep);

         int step=0;
         for (int i=0; i < _thick; i++){
            dcomplex pml = 1. / (1. + phi * pow( double(i-_thick-1),_n));
            psi[base+step] *= pml;
            psi[base+(Nx-1)*lo-step] *= pml;
            step += lo;
         }
      }
   }
}

 /* namespace QDLIB */
