/*
 * OGridStokesAlbrecht.cpp
 *
 *  Created on: 05.03.2012
 *      Author: mko
 */

#include "OGridStokesAlbrecht.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridStokesAlbrecht)

   OGridStokesAlbrecht::OGridStokesAlbrecht() : _name("OGridStokesAlbrecht"), _buf(NULL), _psi(NULL), _eta(0), _xe(0), _ve(0)
   {
   }

   OGridStokesAlbrecht::~OGridStokesAlbrecht()
   {
      DELETE_WF(_psi);
      DELETE_WF(_buf);
   }
   void OGridStokesAlbrecht::Init(ParamContainer &params)
   {
      /* Check our own stuff */

      if ( !params.isPresent("eta") )
         throw(EParamProblem("Stokes operators needs a stokes parameters"));

      if ( !params.isPresent("R") )
         throw(EParamProblem("Stokes operators needs a stokes radius"));

      double value;
      _eta = 6*M_PI;

      params.GetValue("R", value);
      _eta *= value;

      params.GetValue("eta", value);
      _eta *= value;
   }

   void OGridStokesAlbrecht::Init(WaveFunction *Psi)
   {
      if (_buf != NULL) return;

      if (Size() != 2)
         throw ( EParamProblem("Albrecht: Misterious number of operators. Should be 2") );

      OList::Init(Psi);

      _buf = Psi->NewInstance();
      _psi = Psi->NewInstance();
   }

   dcomplex OGridStokesAlbrecht::Emax()
   {
      return dcomplex( _eta * Get(0)->Emax() *  Get(1)->Emax());
   }

   void OGridStokesAlbrecht::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {

      /* Recalc expectation values */
      if ( Operator::RecalcInternals() ) {
         _xe = Get(1)->Expec(sourcePsi) / sourcePsi->Norm();

         Get(0)->Apply(_psi, sourcePsi); /* T*psi */
         Get(1)->Apply(_psi); /* x*(T*psi) */
         double xT = (*sourcePsi * _psi).imag();

         _ve = - 2 * xT;
         cout << "<v> :" << _ve << endl;
      }

      Get(1)->Apply(destPsi, sourcePsi);
      MultElementsCopy((cVec*) _psi, (cVec*) sourcePsi, -_xe);
      AddElementsEq ((cVec*) destPsi, (cVec*) _psi, (cVec*) destPsi);

      *destPsi *= _eta * _ve;
   }

   void OGridStokesAlbrecht::Apply(WaveFunction *Psi)
   {
      *_buf = Psi;

      Apply(Psi, _buf);
   }
   Operator* OGridStokesAlbrecht::operator=(Operator* O)
   {
      return Copy(O);
   }

   Operator* OGridStokesAlbrecht::Copy(Operator* O)
   {
      OGridStokesAlbrecht *o = dynamic_cast<OGridStokesAlbrecht*>(O);

      if (o == NULL )
         throw (EIncompatible("Copy: not of type OGridStokesAlbrecht"));

      _eta = o->_eta;
      _buf = o->_buf->NewInstance();
      _psi = o->_psi->NewInstance();

      OList::Copy(O);

      return this;
   }

   bool OGridStokesAlbrecht::Valid(WaveFunction *Psi)
   {
      return OList::Valid(Psi);
   }
}
