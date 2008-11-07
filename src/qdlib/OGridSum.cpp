#include "OGridSum.h"

namespace QDLIB {

   OGridSum::OGridSum()
   : OGridSystem(), _name("OGridSum"), _size(0), _isUpTodate(false)
   {
      for (int i=0; i < MAX_OPS; i++)
	 _O[i] = NULL;
   }
   
   
   OGridSum::~OGridSum()
   {
      for (int i=0; i < MAX_OPS; i++)
	 if (_O[i] != NULL) delete _O[i];
   }

   /**
    * Update the sum representation.
    */
   void QDLIB::OGridSum::_Update( )
   {
      _O[0]->UpdateTime();
      for (int i=0; i < dVec::size(); i++)      /* Loop over grid points, first operator */
	 (*this)[i] = (*(_O[0]))[i];
      
      int k;
      for (k=1; k < _size; k++)             /* Loop over operators */
	 _O[k]->UpdateTime();
         for (int i=0; i < dVec::size(); i++)   /* Loop over grid points */
	    (*this)[i] += (*(_O[k]))[i];
      
      _isUpTodate = true;
   }

   Operator * OGridSum::NewInstance( )
   {
      OGridSum *r;
      r = new OGridSum();
      return r;
   }

   void OGridSum::Init( ParamContainer & params )
   {
   }

   const string & OGridSum::Name( )
   {
      return _name;
   }

   void OGridSum::UpdateTime( )
   {
      if (_size > 0) _Update();
   }

   dcomplex OGridSum::MatrixElement( WaveFunction *PsiBra, WaveFunction *PsiKet )
   {
      WaveFunction *ket;
      dcomplex d;
      
      ket = *this * PsiKet;
      d = *PsiBra * ket;
   
      return d;
   }

   double OGridSum::Expec( WaveFunction * Psi )
   {
      dcomplex d;
      
      d = OGridSum::MatrixElement(Psi, Psi);
      
      return d.real();
   }

   WaveFunction * OGridSum::operator *( WaveFunction * Psi )
   {
      if (_size == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
   
      if (dVec::size() != Psi->size())
	 throw ( EIncompatible("Operator and WF differ in size") );
   
      if(! _isUpTodate) _Update( );
      
      WaveFunction *psi;
   
      psi = Psi->NewInstance();  /* Copy */
      *psi = Psi;
   
      MultElements( (cVec*) psi, (dVec*) this);
   
      return psi;
   }

   WaveFunction * OGridSum::operator *=( WaveFunction * Psi )
   {
      if (_size == 0)
	 throw ( EParamProblem("Sum Operator is empty") );
   
      if (dVec::size() != Psi->size())
	 throw ( EIncompatible("Operator and WF differ in size") );
   
      if(! _isUpTodate) _Update( );
      
      MultElements( (cVec*) Psi, (dVec*) this);
   
      return Psi;
   }
   
   /**
    * Return an operator.
    */
   OGridSystem* OGridSum::Get( int i )
   {
      if (i >= _size)
	 throw( EIncompatible("Access to empty element") );
      return _O[i];
   }
   
   /**
    * Number of operators stored.
    */
   int OGridSum::Size()
   {
      return _size;
   }
   
   /**
    * Add a new operator.
    */
   void OGridSum::Add( OGridSystem * O )
   {
      if (_size >= MAX_OPS) {
	 throw ( EOverflow("Maximum numbers of operators reached") );
      }
      if (O == NULL){
	 throw ( EIncompatible("Operator not initialized") );
      }
      
      if (dVec::size() == 0) newsize(O->size());
      
      if ( O->size() != dVec::size() )
	 throw ( EParamProblem("Sum Operator can only take operators with same grid size") );
      
      if (O->isTimeDep()) _isTimedependent = true;
      O->Clock(clock);
      
      _O[_size] = O;
      _size++;
      _isUpTodate = false;
   }
   
}








