#include "OGobbler.h"
#include "WaveFunction.h"
#include "Butterworth.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGobbler::OGobbler()
      : OGridSystem(), _name("OGobbler"), _order(0), _nip(false)
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
	    view *= filter;
	 }
	    
      }
	 
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
      if (_params.isPresent("nip")) _nip = true;
      
      /* Gain value. */
      if (_params.isPresent("gain")){
	 double gain;
	 _params.GetValue("gain", gain);
	 MultElements((cVec*) this, gain);
      }
      
      /* Behavior of Expec() */
      _params.GetValue("residue", _residue);
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
      
      delete ket;
      
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
         delete ket;
      } else 
         c = MatrixElement(Psi, Psi);
      
      return c.real();
   }
   
   WaveFunction * OGobbler::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      WFGridSystem *ket;
      WaveFunction *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(destPsi);
      if (opPsi == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", sourcePsi->Name()) );
      
      ket = dynamic_cast<WFGridSystem*>(sourcePsi);
      if (ket == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", destPsi->Name()) );
      
      if (_nip) 
         MultElementsComplexEq((cVec*) opPsi, (cVec*) ket, (dVec*) this, -1);
      else 
	 MultElements((cVec*) opPsi, (cVec*) ket, (dVec*) this);
      
      return ket;
   }
   
   WaveFunction * OGobbler::Apply(WaveFunction * Psi)
   {
      WFGridSystem *ket;
      
      ket = dynamic_cast<WFGridSystem*>(Psi);
      if (ket == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );
      
      if (_nip)
         MultElements( (cVec*) ket, (dVec*) this, dcomplex(0,-1));
      else
         MultElements( (cVec*) ket, (dVec*) this);
      
      return ket;
      
   }
   
   Operator * OGobbler::operator =(Operator * O)
   {
      return O;
   }
   
   Operator * QDLIB::OGobbler::Copy(Operator * O)
   {
      string _name;
      bool _lp[MAX_DIMS];    /* build left pass for dim */
      double _lpx[MAX_DIMS];  /* cut-off center */
      bool _rp[MAX_DIMS];    /* build right pass for dim */
      double _rpx[MAX_DIMS];  /* cut-off center */
      int _order;           /* filter order */
      bool _nip;            /* Negative imaginary potential */
      
      OGobbler *o;
      
      o = dynamic_cast<OGobbler*>(O);
      if (o == NULL)
	 throw(EIncompatible("Not a Gobbler", O->Name()));
      
      _nip = o->_nip;
      _order = o->_order;
      _residue = o->_residue;
	    
      
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
   
   /**
    * We can gobble other OGridSystem operators.
    * 
    * No idea wether this makes sense.
    */
   Operator * OGobbler::operator *(Operator * O)
   {
      
      throw ( EIncompatible("Gobbler can't be applied to other operators") );
      
/*      OGridSystem *op;
      op = dynamic_cast<OGridSystem*>(O);
      if (op == NULL)
	 throw ( EIncompatible("O is not of type OGridSystem", O->Name()) );
      
      
      if ( *((GridSystem*) this) != *((GridSystem*) op) ) _Init();
      
      MultElements( (dVec*) op, (dVec*) this);*/
      
//       return op;
      return NULL;
   }

}
