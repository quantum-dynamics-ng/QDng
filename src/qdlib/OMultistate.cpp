#include "OMultistate.h"
#include "WFMultistate.h"

namespace QDLIB
{

   OMultistate::OMultistate() : Operator(), _name("OMultistate"), _hermitian(true),
				 _nstates(0), _buf1(NULL), _buf2(NULL)
   {
      for(int i=0; i< QD_MAX_STATES; i++){
	 for(int j=0; j< QD_MAX_STATES; j++){
            _matrix[i][j] = NULL;
	 }
      }
   }


   OMultistate::~OMultistate()
   {
      if (_buf1 != NULL) delete _buf1;
      if (_buf2 != NULL) delete _buf2;
      for(int i=0; i< _nstates; i++){
	 for(int j=0; j< _nstates; j++){
	    if (_matrix[i][j] != NULL && i <= j)
	       delete _matrix[i][j];
	    if (_matrix[i][j] != NULL && i > j && !_hermitian)
	       delete _matrix[i][j];
	 }
      }
   }

   /**
    * Add an operator to the matrix.
    */
   void OMultistate::Add(Operator *O, int row, int col)
   {
      if (row >= QD_MAX_STATES || col >= QD_MAX_STATES)
	 throw (EOverflow("More than QD_MAX_STATES not possible"));
      
      if (row+1 > _nstates)
	 _nstates = row+1;
      if (col+1 > _nstates)
	 _nstates = col+1;
      
      _matrix[row][col] = O;
      
      if (O->isTimeDep()) _isTimedependent = true;
      
      if ( (_matrix[row][col] != NULL && _matrix[col][row] != NULL) && row != col )
	_hermitian = false;
   }
   
   /**
    * Direct access to the containing operators.
    * 
    */
   Operator * QDLIB::OMultistate::State(int i, int j)
   {
      if (_nstates > i && _nstates > j)
	 return _matrix[i][j];
      else
	 return NULL;
   }
   
   void OMultistate::Clock( QDClock * cl )
   {
      clock = cl;
      for(int i=0; i < _nstates; i++){
	 for(int j=0;  j < _nstates; j++){
	    if (_matrix[i][j] != NULL)
	       _matrix[i][j]->Clock(clock);
	 }
      }
   } 
   
   Operator * OMultistate::NewInstance( )
   {
      OMultistate *r = new OMultistate();
   
      return r;
   }

   void OMultistate::Init( ParamContainer & params )
   {
      _params = params;
      
      _params.GetValue( "nonhermitian", _hermitian);
   }

   

   void OMultistate::Init( WaveFunction * Psi )
   {
      WFMultistate *psi;
      
      if (_nstates < 1)
	 throw (EParamProblem ("Multistate operator is empty"));
      
      psi = dynamic_cast<WFMultistate*>(Psi);
      
      if (psi == NULL)
	 throw (EIncompatible("Multistate operator can only act on WFMultistate", Psi->Name()));
      
      if (psi->States() != _nstates)
	 throw (EParamProblem ("Multistate operator can only use WFs with same number of states"));
      
      _buf1 = dynamic_cast<WFMultistate*>(Psi->NewInstance());
      _buf2 = dynamic_cast<WFMultistate*>(Psi->NewInstance());
      
      
      
      if (_hermitian){
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= i; j++){
	       if (i != j && _matrix[j][i] != NULL) _matrix[i][j] = _matrix[j][i]; /* Copy pointer only */
	       if (_matrix[i][j] != NULL){
		  _matrix[i][j]->Init(psi->State(j));
		  if (i != j) _matrix[j][i] = _matrix[i][j]; /* Copy pointer only */
	       }
	    }
	 }
      } else {
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= _nstates; j++){
	       if (_matrix[i][j] != NULL)
		  _matrix[i][j]->Init(psi->State(j));
	    }
	 }
      }
   } 
   
   const string & OMultistate::Name( )
   {
      return _name;
   }

   dcomplex OMultistate::MatrixElement( WaveFunction * PsiBra, WaveFunction * PsiKet )
   {
      WaveFunction* opPsi;
      dcomplex c;
      
      opPsi=PsiKet->NewInstance();
      
      Apply( opPsi, PsiKet);
      c = *PsiBra * opPsi;
      delete opPsi;
      
      return c;
   }

   double OMultistate::Expec( WaveFunction * Psi )
   {
      dcomplex c;
      WaveFunction *opPsi;
      
          
      opPsi = Psi->NewInstance();
      Apply( opPsi, Psi);
      
      c = *opPsi * Psi;

      delete opPsi;
      
      return c.real();
   }

   void OMultistate::UpdateTime()
   {
      if (_hermitian){
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= i; j++){
	       if (_matrix[i][j] != NULL)
		  _matrix[i][j]->UpdateTime();
	    }
	 }
      } else {
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= _nstates; j++){
	       if (_matrix[i][j] != NULL)
		  _matrix[i][j]->UpdateTime();
	    }
	 }
      }
   }
   
   /** \todo Diagonalize Matrix => allow large couplings with Cheby */
   double OMultistate::Emax()
   {
      double d=0;
      double max;
      
      for (lint i=0; i < _nstates; i++){
	 if (_matrix[i][i] != NULL){
	    max = _matrix[i][i]->Emax();
	    if (max  > d) d = max;
	 }
      }
      
      return d;
   }
	 
   double OMultistate::Emin()
   {
      double d;
      double min;
      
      d = _matrix[0][0]->Emin();
      
      for (lint i=1; i < _nstates; i++){
	 if (_matrix[i][i] != NULL){
	    min = _matrix[i][i]->Emin();
	    if ( min < d) d = min;
	 }
      }
      
      return d;
   }
   
   WaveFunction * OMultistate::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      WFMultistate *psi, *dPsi;
      psi = dynamic_cast<WFMultistate*>(sourcePsi);
      dPsi = dynamic_cast<WFMultistate*>(destPsi);
      
      *((cVec*) dPsi) = dcomplex(0,0);
      
      for(int i=0; i< _nstates; i++){ 
	 for(int j=0; j< _nstates; j++){
	    if (_matrix[i][j] != NULL){
	       _matrix[i][j]->Apply(_buf1->State(i), psi->State(j));
	       AddElements((cVec*) (dPsi->State(i)), (cVec*) (_buf1->State(i)));
	    }
	 }
      }
     
      return dPsi;
   }


   WaveFunction * OMultistate::Apply( WaveFunction *Psi )
   {
      Apply( _buf2, Psi);
      *Psi = _buf2;
      
      return Psi;
   }
   
   Operator * OMultistate::operator =( Operator * O )
   {
      Copy(O);
      return this;
   }

   
   Operator * QDLIB::OMultistate::Copy(Operator * O)
   {
      OMultistate *o;
      
      o = dynamic_cast<OMultistate*>(O);
      
      if (o == NULL)
	 throw (EIncompatible("Multistate operator copy not possible", O->Name()));
      
      _hermitian = o->_hermitian;
      _nstates = o->_nstates;

      _buf1 =  dynamic_cast<WFMultistate*>(o->_buf1->NewInstance());
      _buf2 =  dynamic_cast<WFMultistate*>(o->_buf2->NewInstance());
      
      *_buf1 = o->_buf1;
      *_buf2 = o->_buf2;
      
      if (_hermitian){
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= i; j++){
	       if (o->_matrix[i][j] != NULL){
		  _matrix[i][j] = o->_matrix[i][j]->NewInstance();
		  *(_matrix[i][j]) = o->_matrix[i][j];
		  if (i !=j)
		     _matrix[j][i] = _matrix[i][j];
	       }
	    }
	 }
      } else {
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j< _nstates; j++){
	       if (o->_matrix[i][j] != NULL){
		  _matrix[i][j] = o->_matrix[i][j]->NewInstance();
		  *(_matrix[i][j]) = o->_matrix[i][j];
	       }
	    }
	 }
      }
      return this;
   }
   
   Operator * OMultistate::operator *( Operator * O )
   {
      throw (EIncompatible("Multistate operator can't be applied to other operators"));
      return O;
   }

   
   Operator* OMultistate::Offset(const double d)
   {
      
      for(int i=0; i< _nstates; i++){
	 if (_matrix[i][i] != NULL)
	    _matrix[i][i]->Offset(d);
      }
      
//       if (_hermitian){
// 	 for(int i=0; i< _nstates; i++){
// 	    for(int j=0; j<= i; j++){
// 	       if (_matrix[i][j] != NULL)
// 		  _matrix[i][j]->Offset(d);
// 	    }
// 	 }
//       } else {
// 	 for(int i=0; i< _nstates; i++){
// 	    for(int j=0; j< _nstates; j++){
// 	       if (_matrix[i][j] != NULL)
// 		  _matrix[i][j]->Offset(d);
// 	    }
// 	 }
//       }
       return this;
   }

   Operator* OMultistate::Scale(const double d)
   {
      if (_hermitian){
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j<= i; j++){
	       if (_matrix[i][j] != NULL)
		  _matrix[i][j]->Scale(d);
	    }
	 }
      } else {
	 for(int i=0; i< _nstates; i++){
	    for(int j=0; j< _nstates; j++){
	       if (_matrix[i][j] != NULL)
		  _matrix[i][j]->Scale(d);
	    }
	 }
      }
      scaling=d;
      return this;
   }

}
/* namespace QDLIB */

