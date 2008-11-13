#include "OGobbler.h"
#include "WaveFunction.h"
#include "Butterworth.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGobbler::OGobbler()
      : OGridSystem(), _name("OGobbler"), _order(0)
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
   void OGobbler::_Init(GridSystem * grid)
   {
      dVec filter;
      double x;
      
      *((GridSystem*) this) = *grid;
      
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
	    Butterworth<dVec>::Lowpass(filter, x, _order);
	    
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
      sprintf (c, "%d", i);
      s = string("N") + string(c);
      for (i=0; i < GridSystem::Dim(); i++){
	 /* check the left pass */
	 if (_params.isPresent(string("lp") + string(c))) {
	    _params.GetValue(string("lp") + string(c), _lpx[i]);
	 } else _lp[i] = false;
	    
	 /* check the right pass */
	 if (_params.isPresent(string("lp") + string(c))) {
	    _params.GetValue(string("lp") + string(c), _rpx[i]);
	 } else _rp[i] = false;
	 
	 i++;
	 sprintf (c, "%d", i);
      }
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
      
      ket = PsiKet->NewInstance();
      *ket = PsiKet;
      
      return *PsiBra * ket;
   }
   
   double OGobbler::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      c = MatrixElement(Psi,Psi);
      
      return c.real();
   }
   
   WaveFunction * OGobbler::operator *(WaveFunction * Psi)
   {
      WFGridSystem *ket;
      
      *ket = dynamic_cast<WFGridSystem*>(Psi);
      
      if ( *((GridSystem*) this) != *((GridSystem*) ket) ) _Init(ket);
      
      MultElements( (cVec*) ket, (dVec*) this);
      
      return ket;
   }
   
   WaveFunction * OGobbler::operator *=(WaveFunction * Psi)
   {
      WFGridSystem *ket;
      
      ket = dynamic_cast<WFGridSystem*>(Psi);
      
      if ( *((GridSystem*) this) != *((GridSystem*) ket) ) _Init((GridSystem*) ket);
      
      MultElements( (cVec*) ket, (dVec*) this);
      
      return ket;
      
   }
   
   Operator * OGobbler::operator =(Operator * O)
   {
      return O;
   }
   
   /**
    * We can gobble other OGridSystem operators.
    * 
    * No idea wether this makes sense.
    */
   Operator * OGobbler::operator *(Operator * O)
   {
      OGridSystem *op;
      
      op = dynamic_cast<OGridSystem*>(O);
      
      if ( *((GridSystem*) this) != *((GridSystem*) op) ) _Init( (GridSystem*) op);
      
      MultElements( (dVec*) op, (dVec*) this);
      
      return op;
   }

}



