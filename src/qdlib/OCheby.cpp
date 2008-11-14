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


   int QDLIB::OCheby::Recursion( )
   {
      return _order;
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
      *ket0 =  Psi;
//       *ket0 *= _coeff[0];
      
      *ket1 = Psi;
      *ket1 *= _exp;
      *_hamilton *= ket1;
//       *ket1 *= _coeff[1];
      
      
      /* Operator recursion loop */
      for(int i=2; i < _order; i++) {
	 /* 2 X \phi_i-1 + \phi_i-2 */
	 *ket2 = ket1;
	 *ket2 *= 2 * _exp;
	 *_hamilton *= ket2;
	 *ket2 += ket0;
// 	 *ket2 *= _coeff[i];
	 
	 /* multiply by coefficients of the series expansion and add up to the result*/
	 *ket0 *= _coeff[i-2];
	 *r += ket0;
	 cout << r->Norm() << endl;
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
      if (clock == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a clock") );
      if (clock->Dt() == 0)
	 throw ( EParamProblem("No time step defined") );
      
      
      /* Energy range & offset */
      Rdelta = (_hamilton->Emax() - _hamilton->Emin()) *  clock->Dt() / 2;
      Gmin =  _hamilton->Emin() *  clock->Dt();
      
      /* Recursion order */
      if (_order > 0 && _order <= int(5 * Rdelta) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      
      if (_order <= 0)
	 _order =  int(2*Rdelta);
      
      if (_order < 10) _order=10;
      
      cout << endl << "Dt: " << clock->Dt() << endl;
      cout << endl << "Rdelta: " << Rdelta << endl;
      cout << endl << "initial order: " <<_order << endl;
      
      
      /* Check for convergence of the Bessel series */
      dVec bessel;
    
      BesselJ0(_order, Rdelta , bessel);
      while ( abs(bessel[_order - 1] - bessel[_order - 2] ) > BESSEL_DELTA && _order < BESSEL_MAX_ORDER) {
	 _order += 2;
	 BesselJ0(_order, Rdelta , bessel);
      }
	
      if (_order >= BESSEL_MAX_ORDER)
	 throw ( EParamProblem ("Maximum recursion order reached. Choose a smaller time step or set the order manually") );
      
      _params.SetValue("order", _order);
      
      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = cexpI((Rdelta + Gmin)) * bessel[0];
      for (int i=1; i < _coeff.size(); i++){
	 _coeff[i] = 2.0 * cexpI((Rdelta + Gmin)) * bessel[i];
      }
//       cout << _coeff; 
//       _exp = 1/Rdelta;
      _exp  = OPropagator::Exponent() / Rdelta;
      cout << OPropagator::Exponent();
      std::cout << _exp;
      /* Remove WF buffers => new operator type  needs new WF type */
      if (ket0 != NULL) delete ket0;
      if (ket1 != NULL) delete ket1;
      if (ket2 != NULL) delete ket2;

   }

}/* namespace QDLIB */



