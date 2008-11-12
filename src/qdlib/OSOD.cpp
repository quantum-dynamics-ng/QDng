#include "OSOD.h"

namespace QDLIB
{

   OSOD::OSOD()
      : OPropagator(), _name("OSOD"), _psi_last(NULL), _ham(NULL), _exp(0,0), _isUpToDate(false)
   {}


   OSOD::~OSOD()
   {
      if (_psi_last != NULL) delete _psi_last;
   }

   /**
    * Initializes the Propagator.
    */
   void OSOD::_Init( WaveFunction *Psi )
   {
//       WaveFunction *psi_so;
      
      if (_ham == NULL) throw ( EParamProblem("Hamiltonian for SOD not initialized") );
      if (_psi_last == NULL) _psi_last = Psi->NewInstance();
      
      *_psi_last = Psi;
      
      
      _exp = OPropagator::Exponent();
      /* First order back step */
      
      *_ham *= Psi;
      *Psi *= _exp;
      *Psi += _psi_last;
      
      _psi_last->Normalize();
      _isUpToDate = true;
   }


   /**
    * Give the SDO a Hamiltonian for propagation
    */
   void QDLIB::OSOD::AddHamiltonian( Operator *H )
   {
      _ham = H;
   }
   
   Operator * OSOD::NewInstance( )
   {
      OSOD *O = new OSOD();
      
      O->Init(_params);
      
      return O;
   }

   void OSOD::Init( ParamContainer & params )
   {
   }

   const string & OSOD::Name( )
   {
      return _name;
   }

   dcomplex OSOD::MatrixElement( WaveFunction * PsiBra, WaveFunction * PsiKet )
   {
      return dcomplex(0,0);
   }

   double OSOD::Expec( WaveFunction * Psi )
   {
      return 0;
   }

   WaveFunction * OSOD::operator *( WaveFunction * Psi )
   {
      return Psi;
   }

   WaveFunction * OSOD::operator *=( WaveFunction * Psi )
   {
      if ( !_isUpToDate ) _Init(Psi);
      
      *_ham *= Psi;
      *Psi *= 2*_exp;
      *Psi += _psi_last;
      
      *_psi_last = Psi;
      
      return Psi;
   }

   Operator * OSOD::operator =( Operator * O )
   {
      return O;
   }

   Operator * OSOD::operator *( Operator * O )
   {
      return O;
   }

   ParamContainer & OSOD::TellNeeds( )
   {
      ParamContainer *p = new ParamContainer();
      return *p;
   }

   void OSOD::AddNeeds( string & Key, Operator * O )
   {
   }

   void OSOD::ReInit( )
   {
      _isUpToDate=false;
   }


}




