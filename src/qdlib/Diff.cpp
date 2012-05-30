/*
 * Diff.cpp
 *
 *  Created on: 11.05.2012
 *      Author: markus
 */

#include "Diff.h"
#include "Kspace.h"
#include "math/VectorView.h"
#include "fft/FFTGlobal.h"

namespace QDLIB
{

   Diff::Diff() : _deriv(1), _dim(-1), _mixed(false), _method(FFT), _hofd(NULL)
   {}

   Diff::Diff(int derivative, bool mixed) : _deriv(derivative), _dim(-1), _mixed(mixed), _method(FFT), _hofd(NULL)
   {}

   Diff::~Diff()
   {
      if (_hofd != NULL) delete _hofd;
   }

   void Diff::_InitKspace ()
   {
      dVec *kspace1;
      dVecView *view;

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

         /* */
         if (_deriv == 1)
            kspace1 = Kspace::Init1Dddx(_grid, i);
         else
            kspace1 = Kspace::Init1Dd2dx2(_grid, i);

         view = new dVecView(_kspace1[i],_grid.Dim(), _grid.DimSizes());

         view->ActiveDim(i);
         *view = *kspace1;

         delete view;
         delete kspace1;
      }

      /* Setup mixed derivatives */
      if (_mixed) {
         for (int i=0; i < _grid.Dim(); i++){
            for (int j=0; j <= i; j++){

               if (_kspaceMix[i][j].size() == 0) {
                  _kspaceMix[i][j].newsize(_grid.Size());
               }

               view = new dVecView(_kspaceMix[i][j], _grid.Dim(), _grid.DimSizes());

               kspace1 = Kspace::Init1Dddx(_grid, i);
               view->ActiveDim(i);
               *view = *kspace1;
               delete kspace1;

               kspace1 = Kspace::Init1Dddx(_grid, j);
               view->ActiveDim(j);
               *view = *kspace1;
               delete kspace1;

               delete view;
            }
         }
      }

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
         _hofd = new cHOFD(_deriv, order);
         _hofd->SetGrid(_grid);

      } else if (_method == FFT) {
         _InitKspace();
         FFTGlobal::Instance().FlushWisdom();
      }


      if (_grid.Dim() == 0)
         throw (EParamProblem("No grid defined for difference operator"));

      if (pm.isPresent("dim")){
         pm.GetValue("dim", _dim);

         if (_dim > _grid.Dim()-1)
            throw (EParamProblem("Invalid dimension for difference operator"));
      }
   }

   /**
    *  Differenciate along given dim.
    **/
   void Diff::DnDxn(WaveFunction* out, WaveFunction* in, int dim, double d)
   {
      switch (_method){
         case FFT:
            out->FastCopy (*in);
            _FFT.Forward(out, dim);
            MultElementsComplex((cVec*) out, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));
            _FFT.Backward(out, dim);
            break;
         case HOFD:
            _hofd->SetFactor(d);
            _hofd->Diff(out, in, dim);
            break;
      }
   }

   /**
    * Differenciate wave function in given dimension and add to out.
    * Input is destroyed after operation.
    *
    * \param d prefactor. Is multiplied to wf.
    */
   void Diff::DnDxnAdd(WaveFunction* out, WaveFunction* in, int dim, double d)
   {
      switch (_method) {
         case FFT:
            _FFT.Forward(in, dim);
            MultElementsComplex((cVec*) in, (dVec*) &(_kspace1[dim]), d / double(_grid.DimSize(dim)));
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
      switch (_method) {
         case FFT:
            out->FastCopy (*in);
            _FFT.Forward(out, dim1);
            _FFT.Forward(out, dim2);

            if (dim2 > dim1)
               MultElementsComplex((cVec*) out, (dVec*) &(_kspaceMix[dim2][dim1]), d / double( _grid.DimSize(dim1)*_grid.DimSize(dim2) ));
            else
               MultElementsComplex((cVec*) out, (dVec*) &(_kspaceMix[dim1][dim2]), d / double( _grid.DimSize(dim1)*_grid.DimSize(dim2) ));

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
