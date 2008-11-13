#include "sys/Exception.h"
#include "OCheby.h"


namespace QDLIB
{

   OCheby::OCheby()
      : OPropagator(), _name("OCheby"), _hamilton(NULL),
      _order(0), _coeff(0), Rdelta(0.0), Gmin(0), ket0(NULL), ket1(NULL), ket2(NULL)
   {}


   OCheby::~OCheby()
   {
      if (ket0 != NULL) delete ket0;
      if (ket1 != NULL) delete ket1;
      if (ket2 != NULL) delete ket2;
   }


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
      WaveFunction *r;
      
      if (ket0 == NULL) ket0 = Psi->NewInstance();
      if (ket1 == NULL) ket1 = Psi->NewInstance();
      if (ket2 == NULL) ket2 = Psi->NewInstance();
      
      r = Psi->NewInstance();
      *((cVec*) r) = dcomplex(0,0);
      
      /* \phi_0 and \phi_1 are fixed */
      *ket0 = Psi;
      *ket0 *= _exp;
      
      *ket1 = *_hamilton * Psi;
      *ket1 *= _exp;
      
      /* Operator recursion loop */
      for(int i=2; i < _order; i++) {
	 /* 2 X \phi_i-1 + \phi_i-2 */
	 *ket2 = *_hamilton * ket1;
	 *ket2 *= 2 * _exp;
	 *ket2 += *ket0;
	 
	 /* multiply by coefficients of the series expansion and add up to the result*/
	 *ket0 *= _coeff[i-2];
	 *r += ket0;
	 
	 /* shift back by one */
	 *ket0 = ket1;
	 *ket1 = ket2;
      } 
      /* multiply the last two coefficients of the series expansion */
      *ket0 *= _coeff[_order-2];
      *r += ket0;
      *ket1 *= _coeff[_order-1];
      *r += ket1;

      return r;
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
      if (_order > 0 && _order < 10 * int(clock->Dt() * Rdelta) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      if (_order == 0)
	 _order = 10 * int(clock->Dt() * Rdelta);
      
      _params.SetValue("order", _order);
      
      /* Check for convergence of the Bessel series */
      dVec bessel;
      int k = 10;
      BesselJ0(_order, (Rdelta * clock->Dt()), bessel);
      while ( abs(bessel[_order - 1] - bessel[_order - 2] ) > BESSEL_DELTA && _order < BESSEL_MAX_ORDER) {
	 k++;
	 _order = k * int(clock->Dt() * Rdelta);
	 BesselJ0(_order, (Rdelta * clock->Dt()), bessel);
      }
	 
      if (_order >= BESSEL_MAX_ORDER)
	 throw ( EParamProblem ("Maximum recursion order reached. Choose a smaller time step or set the order manually") );
      
      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = cexpI(Rdelta + Gmin) * bessel[0];
      for (int i=1; i < _coeff.size(); i++){
	 _coeff[i] = 2.0 * cexpI(Rdelta + Gmin) * bessel[0];
      }
      
      _exp  = OPropagator::Exponent() / Rdelta;
      
      /* Remove WF buffers => new operator type  needs new WF type */
      if (ket0 != NULL) delete ket0;
      if (ket1 != NULL) delete ket1;
      if (ket2 != NULL) delete ket2;

   }

} /* namespace QDLIB */



