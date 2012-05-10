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


   OGridHOFD::OGridHOFD() : _name("OGridHOFD"), _deriv(1), _order(2), _dim(-1), _pfac(1.), _hofd(NULL), _buf(NULL)
   {
   }

   OGridHOFD::~OGridHOFD()
   {
      if (_hofd != NULL) delete _hofd;
   }

   void OGridHOFD::Init(ParamContainer& params)
   {
      _params = params;

      _params.GetValue("deriv", _deriv);

      _params.GetValue("order", _order);

      if ( _params.isPresent("dim") ){
         _params.GetValue("dim", _dim);
      }

      if ( _params.isPresent("mass") ){
         _params.GetValue("mass", _pfac);
         _pfac = -1./_pfac/2.;
      }


      if (_hofd != NULL) delete _hofd;

      _hofd = new cHOFD(_deriv, _order);
      _hofd->SetFactor(_pfac);
   }

   void OGridHOFD::Init(WaveFunction* Psi)
   {
      WFGridSystem* psi;

      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      *((GridSystem*) this) = *((GridSystem*) psi);

      if (_dim >= GridSystem::Dim() || _dim < -1)
         throw(EParamProblem("Invalid number of active dimension", _dim));

      _hofd->SetGrid(*((GridSystem*) this));
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
      if (_dim == -1)
         _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi);
      else
         _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, _dim);
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
