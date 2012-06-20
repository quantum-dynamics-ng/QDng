/*
 * PML.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: markus
 */

#include "PML.h"
#include "tools/Logger.h"

namespace QDLIB
{

   PML::PML() : _dim(-1), _gamma(0.25), _smax(1), _p(4), _thick(10)
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


      if (pm.isPresent("p")){
         vector<double> val;
         pm.GetArray("p", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: mononomial order for at least one dim is missing"));

         if (val.size() == 1)
            _p = val[0];
         else
            _p = val[dim];
      }

      double k=M_PI/_grid.Dx(_dim)/2.; /** default is half k-max*/
      if (pm.isPresent("k")){
         vector<double> val;
         pm.GetArray("k", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: characteristic k-value order for at least one dim is missing"));

         if (val.size() == 1)
            k = val[0];
         else
            k = val[dim];
      }

      double otol = 1e-5;
      /* Determine smax */
      if (pm.isPresent("smax"))
         pm.GetValue("smax", _smax);
      else {
         /* estimate sigma_max by error on outer boundary
          * Ignore error at the interface.
          */

         if (pm.isPresent("otol"))
            pm.GetValue("otol", otol);

         _smax = - log(otol) * (double(_p) + 1) / (1.141 * k * _thick * _grid.Dx(_dim));
      }

      dcomplex phi = cexpI(_gamma*M_PI) * _smax * pow(1./double(_thick), _p);
      _f1.newsize(_thick);
      for (int i=0; i < _thick; i++)
         _f1[i] = 1. / (1. + phi * pow( double(i-_thick-1),_p));

      Logger& log = Logger::InstanceRef();

      log.coutdbg().precision(4);
      log.coutdbg() << endl << "PML for dim " << _dim << endl;
      log.IndentInc();
      log.coutdbg() << "Tol. outer:\t" <<  scientific << otol << endl;
      log.coutdbg() << "k:\t" <<  scientific << k << endl;
      log.coutdbg() << "smax:\t" << fixed << _smax  << endl;
      log.IndentDec();
      log.flush();
   }

   /**
    * Apply the PML specific metric 1/f
    */
   void PML::ApplyTransform(cVec* wf)
   {
      _grid.ActiveDim(_dim);
      int lo = _grid.LowOthers();
      int Nx = _grid.DimSize(_dim);

      dcomplex* psi = wf->begin(0);
      for (int rep=0; rep < _grid.NumOthers(); rep++){
         int base = _grid.IndexBase(rep);

         int step=0;
         for (int i=0; i < _thick; i++){
            psi[base+step] *= _f1[i];
            psi[base+(Nx-1)*lo-step] *= _f1[i];
            step += lo;
         }
      }
   }
}

 /* namespace QDLIB */
