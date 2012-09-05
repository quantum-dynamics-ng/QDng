/*
 * Diff.cpp
 *
 *  Created on: 11.05.2012
 *      Author: markus@math.uu.se
 */

#include "Diff.h"
#include "Kspace.h"
#include "math/VectorView.h"
#include "fft/FFTGlobal.h"

namespace QDLIB
{

   Diff::Diff() : _deriv(1), _dim(-1), _mixed(false), _single(true), _coll(false),
                  _method(FFT), _pfac (MAX_DIMS), _hofd(NULL)
   {
      _pfac = 1.0;
   }

   Diff::Diff(int derivative, bool mixed) : _deriv(derivative), _dim(-1), _mixed(mixed), _single(true), _coll(false),
                      _method(FFT), _pfac (MAX_DIMS), _hofd(NULL)
   {
      _pfac = 1.0;
   }

   Diff::~Diff()
   {
      if (_hofd != NULL) delete _hofd;
   }

   void Diff::MultiplyKspace(cVec* data, int dim1, int dim2)
   {

   }

   void Diff::MultiplyKspace(cVec* data, int dim)
   {

   }

   void Diff::MultiplyKspace(cVec* data)
   {

   }

   void Diff::_InitKspace ()
   {
      if (_method != FFT) return;

      dVec *kspace1;
      dVecView *view;

      /* Setup single dim derivatives */
      if (_single){
         int begin = 0;
         int end = _grid.Dim();
         if (_dim > -1) {
            begin = _dim;
            end = _dim;
         }

         /* Setup derivatives for every coordinate */
         for (lint i = begin; i < end; i++) {
            /* derivatives */
            if (_kspace1[i].size() == 0) {
               _kspace1[i].newsize(_grid.Size());
            }

            _kspace1[i] = 1.;

            /* */
            if (_deriv == 1)
               kspace1 = Kspace::Init1Dddx(_grid, i, _pfac[i]);
            else
               kspace1 = Kspace::Init1Dd2dx2(_grid, i, _pfac[i]);

            view = new dVecView(_kspace1[i],_grid.Dim(), _grid.DimSizes());

            view->ActiveDim(i);
            *view = *kspace1;

            delete view;
            delete kspace1;
         }
      }

      /* Setup mixed derivatives */
      if (_mixed) {
         for (int i=0; i < _grid.Dim(); i++){
            for (int j=0; j <= i; j++){

               if (_kspaceMix[i][j].size() == 0) {
                  _kspaceMix[i][j].newsize(_grid.Size());
               }

               view = new dVecView(_kspaceMix[i][j], _grid.Dim(), _grid.DimSizes());

               kspace1 = Kspace::Init1Dddx(_grid, i, _pfac[i]);
               view->ActiveDim(i);
               *view = *kspace1;
               delete kspace1;

               kspace1 = Kspace::Init1Dddx(_grid, j, _pfac[i]);
               view->ActiveDim(j);
               *view = *kspace1;
               delete kspace1;

               delete view;
            }
         }
      }

      /* Setup sum over dims */
      if (_coll){
         dVec *kspace1;


         _kspace.newsize(_grid.Size());

         _kspace = 0;
         dVecView view(_kspace, _grid.Dim(), _grid.DimSizes());

         /* Init k-space for every dimension */
         for (int i = 0; i < _grid.Dim(); i++) {

            kspace1 = Kspace::Init1Dd2dx2(_grid, i, _pfac[i]);

            view.ActiveDim(i);
            view += *kspace1;
            delete kspace1;
         }

      }

   }

   Transform* Diff::GetTransform()
   {
      if (_method == FFT) return &_FFT;
      else throw(EIncompatible("Transform for HOFDs is not implemented"));
   }

   void Diff::InitParams(ParamContainer pm)
   {
      /* Choose method for differenciation */

      if (pm.isPresent("method")){
         string method;
         pm.GetValue("method", method);
         if (method == "FFT")
            _method = FFT;
         else if (method == "HOFD")
            _method = HOFD;
         else
             throw(EParamProblem("Unknown differenciation method", method));

      }

      if (_method == HOFD) {

         int order = DIFF_DEF_ORDER;
         if (pm.isPresent("order")){
            pm.GetValue("order", order);
         }

         bool pbc;
         pm.GetValue("pbc", pbc, false);

         _hofd = new cHOFD(_deriv, order);
         _hofd->SetGrid(_grid);
         _hofd->SetFactor(_pfac);
         _hofd->SetPBC(pbc);

      } else if (_method == FFT) {
         _InitKspace();
         FFTGlobal::Instance().FlushWisdom();
      }

      if (_grid.Dim() == 0)
         throw (EParamProblem("No grid defined for difference operator"));

      if (pm.isPresent("dim")){
         pm.GetValue("dim", _dim);

         if (_dim > _grid.Dim()-1 || _dim  < -1)
            throw (EParamProblem("Invalid dimension for difference operator"));
      }

   }


   /**
    * Differenciate along all dims.
    *
    * \f$  \f$
    */
   void Diff::DnDxn(WaveFunction* out, WaveFunction* in, double d)
   {
      switch (_method){
         case FFT:
            _FFT.Forward(in);
            out->IsKspace(true); /* The result is in K-space */
            MultElements((cVec*) out, (cVec*) in, &_kspace, d / double(_grid.Size()));
            in->IsKspace(false); /* switch back to X-space -> we don't change sourcePsi */
            _FFT.Backward(out);
            break;
         case HOFD:
            _hofd->SetFactor(d);
            _hofd->Diff(out, in);
            break;
      }
   }

   /**
    *  Differenciate along given dim.
    **/
   void Diff::DnDxn(WaveFunction* out, WaveFunction* in, int dim, double d)
   {
      switch (_method){
         case FFT:
            _FFT.Forward(in, dim);
            out->IsKspace(true);
            if (_deriv % 2  == 1)
               MultElementsComplexEq((cVec*) out, (cVec*) in, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));
            else
               MultElements((cVec*) out, (cVec*) in, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));

            in->IsKspace(false);
            _FFT.Backward(out, dim);
            break;
         case HOFD:
            _hofd->SetFactor(d * _pfac[dim]);
            _hofd->Diff(out, in, dim);
            break;
      }
   }

   /**
    * Differenciate wave function in given dimension and add to out.
    * Input is destroyed after operation.
    * \todo make non-destructive
    * \param d prefactor. Is multiplied to wf.
    */
   void Diff::DnDxnAdd(WaveFunction* out, WaveFunction* in, int dim, double d)
   {
      switch (_method) {
         case FFT:
            _FFT.Forward(in, dim);
            if (_deriv % 2  == 1)
               MultElementsComplex((cVec*) in, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));
            else
               MultElements((cVec*) in, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));

            _FFT.Backward(in, dim);
            *out += in;
            break;
         case HOFD:
            _hofd->SetFactor(d);
            _hofd->Diff(out, in, dim, true);
            break;
      }
   }

   /**
    * Mixed derivative.
    * \todo TEST
    */
   void Diff::DxDy(WaveFunction* out, WaveFunction* in, int dim1, int dim2, double d)
   {
      int i = dim1;
      int j = dim2;

      switch (_method) {
         case FFT:
            out->FastCopy (*in);
            _FFT.Forward(out, dim1);
            _FFT.Forward(out, dim2);

            if (dim2 > dim1) { i=dim2; j=dim1; }

            if (_deriv % 2  == 1)
               MultElementsComplex((cVec*) out, (dVec*) &(_kspaceMix[i][j]), d / double( _grid.DimSize(dim1)*_grid.DimSize(dim2) ));
            else
               MultElements((cVec*) out, (dVec*) &(_kspaceMix[i][j]), d / double( _grid.DimSize(dim1)*_grid.DimSize(dim2) ));

            _FFT.Backward(out, dim2);
            _FFT.Backward(out, dim1);
            break;
         case HOFD:
            _hofd->SetFactor(d);
            _hofd->SetDeriv(1);
//            _hofd->Diff(out->GetSpaceBuffer(), in, dim1);
//            _hofd->Diff(out, out->GetSpaceBuffer(), dim2);
            _hofd->Diff(out, out->GetSpaceBuffer(), in, dim1, dim2);
            _hofd->SetDeriv(_deriv);
            break;
      }
   }


} /* namespace QDLIB */
