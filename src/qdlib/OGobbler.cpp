#include "OGobbler.h"
#include "WaveFunction.h"
#include "Butterworth.h"
#include "WFGridSystem.h"

namespace QDLIB {

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
	    if (int(x) >  GridSystem::DimSizes(i) || int(x) < 0)
	       throw ( EParamProblem ("Center of filter edge out of the grid") );
	    
	    filter.newsize(GridSystem::DimSizes(i));
	    Butterworth<dVec>::Lowpass(filter, x, _order);
	
	    view.ActiveDim(i);
	    view *= filter;
	 }
	 if (_rp[i]){
	    x = (_rpx[i] - GridSystem::Xmin(i)) / GridSystem::Dx(i);
	    if (int(x) >  GridSystem::DimSizes(i) || int(x) < 0)
	       throw ( EParamProblem ("Center of filter edge out of the grid") );
	    
	    filter.newsize(GridSystem::DimSizes(i));
	    Butterworth<dVec>::Highpass(filter, x, _order);
	    
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
      if (_params.isPresent("order") ){
	 _params.GetValue("order", _order);
	 if (_order < 1)
	    throw ( EParamProblem ("Filter order to small") );
      } else {
	 _order = 20;      /* default value */
	 _params.SetValue("order", _order);
      }
      
      /* Get all the params from the ParamContainer */
      _params.GetValue( "dims", n );
      if ( n > MAX_DIMS ) throw ( EOverflow("More than MAX_DIMS for gobbler defined") );
      if ( n <= 0 ) throw ( EParamProblem("Zero number of dimension defined") );
      GridSystem::Dim(n);
   
   
      int i=0;
      char c[256];

      for (i=0; i < n; i++){
	 sprintf (c, "%d", i);
	 /* check the left pass */
	 if (_params.isPresent(string("lp") + string(c))) {
	    _params.GetValue(string("lp") + string(c), _lpx[i]);
	    _lp[i] = true;
	 } else _lp[i] = false;
	    
	 /* check the right pass */
	 if (_params.isPresent(string("rp") + string(c))) {
	    _params.GetValue(string("rp") + string(c), _rpx[i]);
	    _rp[i] = true;
	 } else _rp[i] = false;
	 
      }
      
      /* negative imaginary potential */
      _params.GetValue("nip", _nip, false);
      
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
      
      *((GridSystem*) this) = *((GridSystem*) psi);
      _Init();
   }
   
   Operator * OGobbler::NewInstance()
   {
      OGobbler *r;
      
      r = new OGobbler();
      return r;
   }
   
   const string & OGobbler::Name()
   {
      return _name;
   }
   
   dcomplex OGobbler::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      WaveFunction *ket;
      dcomplex c;
	    
      
      ket = PsiKet->NewInstance();
      Apply(ket, PsiKet);
      c = *PsiBra * ket;
      
      DELETE_WF(ket);
      
      return c;
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
      if (_nip) 
	 MultElementsComplexEq((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this, -1);
      else 
	 MultElements((cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);

   }
   
   void OGobbler::Apply(WaveFunction * Psi)
   {
      if (_nip)
	 MultElements( (cVec*) Psi, (dVec*) this, dcomplex(0,-1));
      else
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

