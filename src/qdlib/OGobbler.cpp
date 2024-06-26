// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OGobbler.h"
#include "WaveFunction.h"
#include "math/math_functions.h"
#include "WFGridSystem.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGobbler)
   
   OGobbler::OGobbler()
      : OGridSystem(), _name("OGobbler"), _order(0), _gain(1),_nip(false)
   {
      for (int i=0; i < MAX_DIMS; i++){
	 _lp[i] = false;
	 _rp[i] = false;
      }
   }
   
   
   OGobbler::~OGobbler()
   {
      
   }
   
   /**
    * Set up the filter grid.
    */
   void OGobbler::_Init()
   {
      dVec filter;
      double x;
      
      
      dVec::newsize(GridSystem::Size());
      
      if (_nip)
         *((dVec*) this) = 0;
      else
	 *((dVec*) this) = 1;
      
      
      dVecView view( *((dVec*) this), GridSystem::Dim(), GridSystem::DimSizes());
      
      /* Loop over dimension */
      for (int i=0; i < GridSystem::Dim(); i++){
	 if (_lp[i]){
	    x = (_lpx[i] - GridSystem::Xmin(i)) / GridSystem::Dx(i);
	    if (int(x) >  GridSystem::DimSize(i) || int(x) < 0)
	       throw ( EParamProblem ("Center of filter edge out of the grid") );
	    
	    filter.newsize(GridSystem::DimSize(i));
	    FunctionGenerator<dVec>::BwLowpass(filter, x, _order);
	
	    view.ActiveDim(i);
	    view *= filter;
	 }
	 if (_rp[i]){
	    x = (_rpx[i] - GridSystem::Xmin(i)) / GridSystem::Dx(i);
	    if (int(x) >  GridSystem::DimSize(i) || int(x) < 0)
	       throw ( EParamProblem ("Center of filter edge out of the grid") );
	    
	    filter.newsize(GridSystem::DimSize(i));
            FunctionGenerator<dVec>::BwHighpass(filter, x, _order);
	    
	    view.ActiveDim(i);
	    
	    if (_nip)
	       view += filter;
	    else
	       view *= filter;
	 }
	    
      }
	 
      if (_gain != 1)
	 MultElements((dVec*) this, _gain);

   }
   
   
   void OGobbler::Init(ParamContainer & params)
   {
      string s;
      int n;
      
      _params = params;
	 
      /* Check filter order */
      if (_params.isPresent("order"))
      {
         _params.GetValue("order", _order);
         if (_order < 1)
            throw(EParamProblem("Filter order to small"));
      }
      else {
         _order = 20; /* default value */
         _params.SetValue("order", _order);
      }
      
      /* Get all the params from the ParamContainer */
      if (_params.isPresent("dims")){
         _params.GetValue( "dims", n );
         GridSystem::Dim(n);
      }


      /* negative imaginary potential */
      _params.GetValue("nip", _nip, false);
      if (_nip) _hermitian = false;
      
      /* Gain value. */
      if (_params.isPresent("gain")){
	 _params.GetValue("gain", _gain);
      }
      
      /* Behavior of Expec() */
      _params.GetValue("residue", _residue, false);
   }
   
   void OGobbler::Init(WaveFunction *Psi)
   {
      WFGridSystem* psi;
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
         throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      if (GridSystem::Size() > 0) return;  // Avoid init twice

      if ( GridSystem::Dim() == 0){ /* Take Dims from WF*/
         Dim(psi->Dim());
      } else if ( GridSystem::Dim() !=  psi->Dim() ) { /* dims has been given in input (check it)*/
         throw ( EIncompatible("OGobbler: Number of dims doesn't match WF") );
      }

      char c[256];
      for (int i = 0; i < Dim(); i++)
      {
         sprintf(c, "%d", i);
         /* check the left pass */
         if (_params.isPresent(string("lp") + string(c)))
         {
            _params.GetValue(string("lp") + string(c), _lpx[i]);
            _lp[i] = true;
         }
         else _lp[i] = false;

         /* check the right pass */
         if (_params.isPresent(string("rp") + string(c)))
         {
            _params.GetValue(string("rp") + string(c), _rpx[i]);
            _rp[i] = true;
         }
         else _rp[i] = false;

      }

      *((GridSystem*) this) = *((GridSystem*) psi);
      _Init();
   }

   double OGobbler::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      if (_residue){
         WaveFunction *ket;
         ket = Psi->NewInstance();
         Apply(ket, Psi);
         
         c= *ket * ket;
	 DELETE_WF(ket);
      } else 
         c = MatrixElement(Psi, Psi);
      
      return c.real();
   }
   
   dcomplex OGobbler::Emax()
   {
      return (dcomplex(_gain));
   }

   dcomplex OGobbler::Emin()
   {
      if (_nip) return (dcomplex(0,-_gain));
      else return dcomplex(0);
   }

   
   void OGobbler::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      if (_nip){
         double sgn = -1;
         if (_conj) sgn = 1; /* Switch sign for complex conjugate */
         MultElementsComplexEq((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, sgn);
      } else
	 MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);

   }
   
   void OGobbler::Apply(WaveFunction * Psi)
   {
      if (_nip) {
         double sgn = -1;
         if (_conj) sgn = 1; /* Switch sign for complex conjugate */
         MultElements( (cVec*) Psi, (dVec*) this, dcomplex(0, sgn));
      } else
	 MultElements( (cVec*) Psi, (dVec*) this);
   }
   
   Operator * OGobbler::operator =(Operator * O)
   {
      return Copy(O);
   }
   
   Operator * OGobbler::Copy(Operator * O)
   {
      OGobbler *o;
      
      o = dynamic_cast<OGobbler*>(O);
      if (o == NULL)
	 throw(EIncompatible("Not a Gobbler", O->Name()));
      
      _nip = o->_nip;
      _order = o->_order;
      _residue = o->_residue;
      _gain = o->_gain;
	    
      
      for (int i=0; i < Dim(); i++){
	 _lp[i] = o->_lp[i];
	 _lpx[i] = o->_lpx[i];
	 _rp[i] = o->_rp[i];
	 _rpx[i] = o->_rpx[i];
	 
      }
      /* Copy Parents */
      OGridSystem::Copy(O);
      
      return this;
   }
   
}

