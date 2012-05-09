/*
 * OGridHOFD.cpp
 *
 *  Created on: 08.05.2012
 *      Author: Markus Kowalewski
 */
#include "OGridHOFD.h"
#include "WFGridSystem.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridHOFD)


   OGridHOFD::OGridHOFD() : _name("OGridHOFD"), _deriv(1), _order(2), _dim(-1), _pfac(1.), _buf(NULL)
   {
      for (int i=0; i < HOFD_NDIFF*HOFD_PERDIFF; i++)
         for (int j=0; j < HOFD_NCOEFF; j++)
            _cf[i][j] = 0;

      _cf[0][3] = -1./2; // Deriv. 1, Accu. 2
      _cf[0][5] =  1./2;

      _cf[1][2] =  1./12.; // Deriv. 1, Accu. 4
      _cf[1][3] = -2./3.;
      _cf[1][5] =  2./3.;
      _cf[1][6] = -1./12.;

      _cf[2][1] =  -1./60.; // Deriv. 1, Accu. 6
      _cf[2][2] =   3./20.;
      _cf[2][3] =  -3./4.;
      _cf[2][5] =   3./4.;
      _cf[2][6] =  -3./20.;
      _cf[2][7] =   1./60.;

      _cf[3][0] =  1./280.; // Deriv. 1, Accu. 8
      _cf[3][1] = -4./105.;
      _cf[3][2] =  1./5.;
      _cf[3][3] =  -4./5.;
      _cf[3][5] =  4./5.;
      _cf[3][6] = -1./5.;
      _cf[3][7] =  4./105.;
      _cf[3][8] =  -1./280.;

      _cf[4][3] = _cf[4][5] = 1.; // Deriv. 2, Accu. 2
      _cf[4][4] =            -2;

      _cf[5][2] = _cf[5][6] = -1./12.; // Deriv. 2, Accu. 4
      _cf[5][3] = _cf[5][5] =  4./3.;
      _cf[5][4] =             -5./2.;

      _cf[6][1] = _cf[6][7] =  1./90.; // Deriv. 2, Accu. 6
      _cf[6][2] = _cf[6][6] = -3./20.;
      _cf[6][3] = _cf[6][5] =  3./2.;
      _cf[6][4] =            -49./18.;

      _cf[7][0] = _cf[7][8] = -1./560.; // Deriv. 2, Accu. 8
      _cf[7][1] = _cf[7][7] =  8./315.;
      _cf[7][2] = _cf[7][6] = -1./5.;
      _cf[7][3] = _cf[7][5] =  8./5.;
      _cf[7][4] =           -205./72.;
   }

   OGridHOFD::~OGridHOFD()
   {
   }

   void OGridHOFD::Init(ParamContainer& params)
   {
      _params = params;

      _params.GetValue("deriv", _deriv);
      if (_deriv > HOFD_NDIFF || _deriv < 1 )
         throw(EParamProblem("Invalid derivative chosen", _deriv));

      _params.GetValue("order", _order);
      if (_order > HOFD_PERDIFF * 2 || _order % 2 != 0 || _order < 1)
         throw(EParamProblem("Invalid order of FD scheme", _order));

      if ( _params.isPresent("dim") ){
         _params.GetValue("dim", _dim);
      }

      if ( _params.isPresent("mass") ){
         _params.GetValue("mass", _pfac);
         _pfac = -1./_pfac/2.;
      }

   }

   void OGridHOFD::Init(WaveFunction* Psi)
   {
      WFGridSystem* psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      if (GridSystem::Size() > 0) return;  // Avoid init twice

      *((GridSystem*) this) = *((GridSystem*) psi);

      if (_dim > GridSystem::Dim() || _dim < -1)
         throw(EParamProblem("Invalid number of active dimension", _dim));
   }

   dcomplex OGridHOFD::Emax()
   {
      if (_deriv == 1)
         return dcomplex(0, _pfac * M_PI/GridSystem::Dx(0) );
      else
         return dcomplex(-_pfac * pow(M_PI/GridSystem::Dx(0),2) );
   }

   dcomplex OGridHOFD::Emin()
   {
      // \todo implement
      return dcomplex(0);
   }

   void OGridHOFD::Apply(WaveFunction* destPsi, WaveFunction* sourcePsi)
   {
      double h = 1./pow(GridSystem::Dx(_dim), _deriv) * _pfac;
      int Nx = GridSystem::DimSize(_dim);

      dcomplex* psi = sourcePsi->begin(0);
      dcomplex* Dpsi = destPsi->begin(0);
      double* cf = & (_cf[HOFD_PERDIFF*(_deriv-1)+_order/2-1][((HOFD_NCOEFF-1)-_order)/2]);

      /* Edge of grid */
      for (int i = 0; i <= _order/2-1 ; i++){
         Dpsi[i] = 0;
         Dpsi[Nx-i-1] = 0;
         for(int j=_order/2-i; j <= _order; j++){
            Dpsi[i] += cf[j] * psi[ i - _order/2 + j ];
            Dpsi[Nx-i-1] += cf[_order-j] * psi[ Nx-1 - (i - _order/2 + j) ];
         }
         Dpsi[i] *= h;
         Dpsi[Nx-i-1] *= h;
      }

      /* Center of grid */
      for (int i =_order/2; i < Nx - _order/2; i++){
         Dpsi[i] = 0;
         for(int j=0; j <= _order; j++){
            Dpsi[i] += cf[j] * psi[ i - _order/2  + j];
         }
         Dpsi[i] *= h;
      }
   }

   void OGridHOFD::Apply(WaveFunction* Psi)
   {
      if (_buf == NULL) _buf = Psi->NewInstance();
      else (_buf->Reaquire());

      *_buf = Psi;
      Apply(Psi, _buf);

      _buf->Retire();
   }

   Operator* OGridHOFD::operator =(Operator* O)
   {
      return Copy(O);
   }

   Operator* OGridHOFD::Copy(Operator* O)
   {
      OGridHOFD* o = dynamic_cast<OGridHOFD*>(O);
      if (o == NULL)
         throw (EIncompatible("Not of type OGridHOFD", O->Name()));

      _deriv = o->_deriv;
      _order = o->_order;
      _dim = o->_dim;

      OGridSystem::Copy(O);

      return this;
   }

} /* namespace QDLIB */
