/*
 * PML.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: markus@math.uu.se
 */

#include "PML.h"
#include "tools/Logger.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#define PML_BEG 1
#define PML_END 2

namespace QDLIB
{

   PML::PML() : _dim(-1), _gamma(0.25), _smax(1), _p(4), _thick(10), _side(PML_BEG+PML_END)
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

      if (pm.isPresent("lmap")){
         vector<int> val;
         pm.GetArray("lmap", val);
         if (val.size() != 1 && dim > val.size())
            throw (EParamProblem("PML: layer mapping for at least one dim is missing"));

         if (val.size() == 1)
            _side = val[0];
         else
            _side = val[dim];

         if (_side < 0 || _side > 3)
            throw (EParamProblem("PML: layer mapping is nonsense"));
      }

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

         if (_thick > _grid.DimSize(_dim))
            throw (EParamProblem("PML: layer is thicker than the whole grid"));
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
      _df1.newsize(_thick);
      for (int i=0; i < _thick; i++){
         _f1[i] = 1. / (1. + phi * pow( double(i-_thick-1),_p));
         _df1[i] = -1 / (_f1[i] * _f1[i]) * phi * _p * pow( double(i-_thick-1),_p - 1);
      }

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
   void PML::ApplyTransform(cVec* wf, bool conj)
   {
      _grid.ActiveDim(_dim);
      int lo = _grid.LowOthers();
      int Nx = _grid.DimSize(_dim);

      dcomplex* psi = wf->begin(0);

//#ifdef _OPENMP
//#pragma omp parallel for
//#endif
      for (int rep=0; rep < _grid.NumOthers(); rep++){
         int base = _grid.IndexBase(rep);

         int step=0;
         switch (_side){
            case PML_BEG:
               if (conj){
                  for (int i=0; i < _thick; i++){
                     psi[base+step] *= _f1[i].conj();
                     step += lo;
                  }
               } else {
                  for (int i=0; i < _thick; i++){
                     psi[base+step] *= _f1[i];
                     step += lo;
                  }
               }
               break;
            case PML_END:
               if (conj){
                  for (int i=0; i < _thick; i++){
                     psi[base+(Nx-1)*lo-step] *= _f1[i].conj();
                     step += lo;
                  }
               } else {
                  for (int i=0; i < _thick; i++){
                     psi[base+(Nx-1)*lo-step] *= _f1[i];
                     step += lo;
                  }
               }
               break;
            default:
               if (conj){
                  for (int i=0; i < _thick; i++){
                     psi[base+step] *= _f1[i].conj();
                     psi[base+(Nx-1)*lo-step] *= _f1[i].conj();
                     step += lo;
                  }
               } else {
                  for (int i=0; i < _thick; i++){
                     psi[base+step] *= _f1[i];
                     psi[base+(Nx-1)*lo-step] *= _f1[i];
                     step += lo;
                  }
               }
         }
      }
   }

   /**
    * Apply the PML specific metric 1/f
    */
   void PML::ApplyTransformDiff(cVec* wf)
   {
      _grid.ActiveDim(_dim);
      int lo = _grid.LowOthers();
      int Nx = _grid.DimSize(_dim);

      dcomplex* psi = wf->begin(0);

//#ifdef _OPENMP
//#pragma omp parallel for
//#endif
      for (int rep=0; rep < _grid.NumOthers(); rep++){
         int base = _grid.IndexBase(rep);

         int step=0;
         switch (_side){
            case PML_BEG:
               for (int i=0; i < _thick; i++){
                  psi[base+step] *= _df1[i];
                  step += lo;
               }
               break;
            case PML_END:
               for (int i=0; i < _thick; i++){
                  psi[base+(Nx-1)*lo-step] *= _df1[i];
                  step += lo;
               }
               break;
            default:
               for (int i=0; i < _thick; i++){
                  psi[base+step] *= _df1[i];
                  psi[base+(Nx-1)*lo-step] *= _df1[i];
                  step += lo;
               }
         }
      }
   }

   /**
    * Apply the PML specific metric 1/f
    * \bug broken (interior is not correctly added!)
    * \bug adjoint missing
    */
   void PML::ApplyTransformAdd(cVec* dwf, cVec* wf)
   {
      _grid.ActiveDim(_dim);
      int lo = _grid.LowOthers();
      int Nx = _grid.DimSize(_dim);

      dcomplex* psi = wf->begin(0);
      dcomplex* dpsi = dwf->begin(0);

//#ifdef _OPENMP
//#pragma omp parallel for
//#endif
      for (int rep=0; rep < _grid.NumOthers(); rep++){
         int base = _grid.IndexBase(rep);

         int step=0;
         switch (_side){
            case PML_BEG:
               for (int i=0; i < _thick; i++){
                  dpsi[base+step] += psi[base+step] * _f1[i];
                  step += lo;
               }
               break;
            case PML_END:
               for (int i=0; i < _thick; i++){
                  dpsi[base+(Nx-1)*lo-step] += psi[base+(Nx-1)*lo-step] * _f1[i];
                  step += lo;
               }
               break;
            default:
               for (int i=0; i < _thick; i++){
                  dpsi[base+step] += psi[base+step] * _f1[i];
                  dpsi[base+(Nx-1)*lo-step] += psi[base+(Nx-1)*lo-step] * _f1[i];
                  step += lo;
               }
         }
      }
   }
}

 /* namespace QDLIB */
