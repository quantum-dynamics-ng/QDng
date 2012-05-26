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
      for (int i=0; i < MAX_DIMS; i++){
         _mass[i] = -1./2.;
      }
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

      /* Get masses */
      if ( _params.isPresent("mass") || _params.isPresent("mass0") ){
         char c[256];
         string s;
         bool eff = false;  /* Indicate at least 1 non-zero dim */

         for (int i = 0; i < Dim(); i++) {
            sprintf(c, "mass%d", i);
            s = c;
            if (_params.isPresent(s)) {
               _params.GetValue(string(c), _mass[i]);
               if (_mass[i] == 0)
                  throw(EParamProblem("Zero mass defined"));

               eff = true;
            } else _mass[i] = -1/2; /* Mark as -1 => don't build k-space */
         }

         if (!eff)
            throw(EParamProblem("Nabla^2 Operator is empty (no masses defined)"));
      }
   }

   dcomplex OGridHOFD::Emax()
   {
      if (_deriv == 1)
         return dcomplex(0, _pfac * M_PI/GridSystem::Dx(0) );
      else {
         double T = 0;
         for (int i = 0; i < GridSystem::Dim(); i++)
            if (_mass[i] > 0)
               T += 1 / (_mass[i] * GridSystem::Dx(i) * GridSystem::Dx(i));

         T *= (M_PI * M_PI / 2);
         return dcomplex(T);
      }
   }

   dcomplex OGridHOFD::Emin()
   {
      if (_deriv == 1)
         return dcomplex(0, -_pfac * M_PI/GridSystem::Dx(0) );
      else
         return dcomplex(0);
   }

   void OGridHOFD::Apply(WaveFunction* destPsi, WaveFunction* sourcePsi)
   {
      if (_dim == -1){
         _hofd->SetFactor(-1./(2.*_mass[0]));
         _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, 0);
         for (int dim=1; dim < Dim(); dim++){
            _hofd->SetFactor(-1./(2.*_mass[dim]));
            _hofd->Diff((cVec*) destPsi, (cVec*) sourcePsi, dim, true);
         }
      } else
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
