#include "sys/Exception.h"
#include "OCheby.h"

namespace QDLIB
{

   OCheby::OCheby()
      : OPropagator(), _name("OCheby"), _hamilton(NULL),
      _order(0), _coeff(0), Rdelta(0.0), Gmin(0)
   {}


   OCheby::~OCheby()
   {}


   Operator * OCheby::NewInstance( )
   {
      OCheby *r;
      r = new OCheby();
      return r;
   }
   
   void OCheby::Init( ParamContainer & params )
   {
      _params = params;
      if (_params.isPresent(string("order")) ){
	 _params.GetValue("order", _order);
	 if (_order < 1) throw ( EParamProblem("Chebychev recursion order invalid") );
      } else _order = 0;
   }

   const string & OCheby::Name( )
   {
      return _name;
   }
   
   dcomplex OCheby::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      return dcomplex(0,0); /* Doesn't make sense */
   }
   
   
   double OCheby::Expec( WaveFunction * Psi )
   {
      return 0; /* Doesn't make sense */
   }

   WaveFunction * OCheby::operator *( WaveFunction * Psi )
   {
      WaveFunction *ket0, *ket1, *ket2;
      
      /* \phi_0 and \phi_1 are fixed */
      ket0 = Psi->NewInstance();
      *((cVec*) ket0) = dcomplex(0,0);
      ket1 = *_hamilton * Psi;
      ket2 = Psi->NewInstance();
      
      /* Recursion */
      for(int i=0; i <= _order; i++) {
	 *ket2 = *_hamilton * ket1;
	 *ket2 *= 2; /* x coeff  */
	 *ket2 += *ket0;
	 
	 *ket0 = ket1;
	 *ket1 = ket2;
      } 
      return ket2;
   }

   WaveFunction * OCheby::operator *=( WaveFunction * Psi )
   {
      return Psi;
   }

   Operator * OCheby::operator =( Operator * O )
   {

      OCheby *P;
      
      P = dynamic_cast<OCheby*>(O);
      
      /* Copy parents */
      *((OPropagator*) this) = (OPropagator*) P;
      _isTimedependent = P->_isTimedependent;
      _params = P->_params;
      clock = P->clock;
      
      /* Copy own stuff */
      _hamilton = P->_hamilton;
      _order = P->_order;
      _coeff = P->_coeff;
      Rdelta = P->Rdelta;
      Gmin = P->Gmin;
      
      return this;
   }

   Operator * OCheby::operator *( Operator * O )
   {
      throw ( EIncompatible ("Can't apply the Chebychev propagator to another operator") );
      return O;
   }

   ParamContainer & OCheby::TellNeeds( )
   {      
      ParamContainer *p = new ParamContainer();
      
      p->SetValue("hamiltonian", 0);
      
      return *p;
   }

   
   void OCheby::AddNeeds( string & Key, Operator * O )
   {
      if (Key == "hamiltonian") _hamilton = O;
      else throw ( EParamProblem("Unknown operator key") );
   }

   
   void OCheby::ReInit( )
   {
      
      if (_hamilton == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a hamiltonian") );
      
      /* Energy range & offset */
      Rdelta = _hamilton->Emax() - _hamilton->Emin();
      Gmin =  _hamilton->Emin();
      
      /* Recursion order */
      if (_order > 0 && _order < int(10 * clock->Dt() * Rdelta) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      if (_order == 0)
	 _order = int(10 * clock->Dt() * Rdelta);
      
      _params.SetValue("order", _order);
      
      /* Setup coefficients */
      dVec bessel;
      BesselJ0(_order, (Rdelta * clock->Dt()), bessel);
      
      _coeff.newsize(_order);
      _coeff[0] = cexpI(Rdelta + Gmin) * bessel[0];
      for (int i=1; i < _coeff.size(); i++){
	 _coeff[i] = 2.0 * cexpI(Rdelta + Gmin) * bessel[0];
      }

   }

} /* namespace QDLIB */



