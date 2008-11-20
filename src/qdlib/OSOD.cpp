#include "OSOD.h"

#include "OCheby.h"

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
     
      
      _exp = (-1)*OPropagator::Exponent();
      OCheby U;
      
      U.Clock( clock );
      U.Hamiltonian( _ham );
      U.ReInit();
      
      *_psi_last = U * Psi;
      
      _exp = OPropagator::Exponent();
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
      
      WaveFunction *psi = Psi->NewInstance();
     
      cout << _exp << endl;
      
      *_ham *= psi;
      *psi *= 2*_exp;
      *psi += _psi_last;
      
      *_psi_last = Psi;
      
      
      *Psi = psi;
      
      delete psi;
     
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
      
      p->SetValue( "hamiltonian", "");
      
      return *p;
   }

   void OSOD::AddNeeds( string & Key, Operator * O )
   {
      if (Key == "hamiltonian")
      {
	 _ham = O;
      } else {
	 throw ( EParamProblem ("Not a valid operator for the SOD", Key) );
      }
   }

   void OSOD::ReInit( )
   {
      _isUpToDate=false;
   }


}




