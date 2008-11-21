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


   /**
    * Ask for the actual order of the recursion.
    */
   int OCheby::Recursion( )
   {
      return _order;
   } 
   
   /**
    * Add a hamiltonian.
    */
   void OCheby::Hamiltonian(Operator *H)
   {
      _hamilton = H;
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
      WaveFunction *swap;
      
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
	 /* shift back by one - use pointers instead of copy */
	 swap = ket0;
	 ket0 = ket1;
	 ket1 = ket2;
	 ket2 = swap;
      } 
      /* multiply the last two coefficients of the series expansion */
      *ket0 *= _coeff[_order-2];
      *r += ket0;
      *ket1 *= _coeff[_order-1];
      *r += ket1;

      return r;
   }

   

   void OCheby::_Recursion( WaveFunction * psi0, WaveFunction * psi1, WaveFunction * Hpsi1, WaveFunction *Psi, int n )
   {
      *Hpsi1 = psi1;       /* 1/R * H * Psi */
      *Hpsi1 *= 1/Rdelta;
      *_hamilton *= Hpsi1;
      
      *ket2 = psi0;
      *Hpsi1 *= 2*_exp;
      *ket2 += Hpsi1;
      
      *psi0 = ket2;
      
      *ket2 *= _coeff[n];
      *Psi += ket2;
      
      cout << "\tNorm ket2, psi: " << ket2->Norm() << " " <<  Psi->Norm() << endl; 
      
   }
   
   WaveFunction * OCheby::operator *=( WaveFunction * Psi )
   {
      WaveFunction *buf;
      
      if (ket0 == NULL) ket0 = Psi->NewInstance();
      if (ket1 == NULL) ket1 = Psi->NewInstance();
      if (ket2 == NULL) ket2 = Psi->NewInstance();
    
      buf = Psi->NewInstance();
      
     
      *ket0 = Psi;   /* phi_0 */
      
      *ket1 = Psi;
      *ket1 *= 1/Rdelta;
      *_hamilton *= ket1;
      *ket1 *=  _exp;
      
      *Psi *= _coeff[0];
     
      *buf = ket1;
      *buf *= _coeff[1];
      
      *Psi += buf;
      
      int i=2;
      while (i < _order){
	 _Recursion(ket0, ket1, buf, Psi, i);
	 i++;
	 if(!(i < _order)) break;
	 _Recursion(ket1, ket0, buf, Psi, i);
	 i++;
      }
      
      delete buf;
      /* multiply the last two coefficients of the series expansion */
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
      Rdelta = (_hamilton->Emax() - _hamilton->Emin())/ 2;
      Gmin =  _hamilton->Emin();
      
      /* This is an estimate for the recursion depth */
      if (_order <= 0)
	 _order =  int(5 * Rdelta * clock->Dt());
      
      if (_order < 10) _order=10;

      /* Check for convergence of the Bessel series */
      dVec bessel;
      int zeroes=0;
      
      /* Manual scaling, very dangerous*/
      if (_params.isPresent("scaling")) _params.GetValue("scaling", Rdelta);
      /* Manual choice of recursion depth */
      if (_params.isPresent("order")) _params.GetValue("order", Rdelta);
      
      if ( BesselJ0(_order, Rdelta * clock->Dt(), bessel, zeroes) != 0)
	 throw ( EOverflow("Error while calculating Bessel coefficients") );
      
      _order -= zeroes;
      int i=2;
      /* Check how much is needed for series convergence */
      while ( i < _order && abs(bessel[i - 1] - bessel[i - 2] ) > BESSEL_DELTA) {
	 i++;
      }
      _order = i;
      
	
      /* Recursion order */
      if (_order > 0 && _order <= int(Rdelta * clock->Dt()) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      if (_order >= BESSEL_MAX_ORDER)
	 throw ( EParamProblem ("Maximum recursion order reached. Choose a smaller time step or set the order manually") );
      
     
      _params.SetValue("order", _order);
      _params.SetValue("scaling", Rdelta);
      _params.SetValue("offset", Gmin);
      
      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = cexpI(OPropagator::Exponent().imag()*(Rdelta + Gmin)) * bessel[0];
      cout << "exponentReal" << cexpI(OPropagator::Exponent().imag()*(Rdelta + Gmin));
      for (int i=1; i < _coeff.size(); i++){
	 _coeff[i] = 2.0 * cexpI(OPropagator::Exponent().imag()*(Rdelta + Gmin)) * bessel[i];
      }
      
//       cout << "Bessel :\n" << bessel;
//       cout << "exp_clock: "<<OPropagator::Exponent().imag() << endl;
      
      _exp  = OPropagator::Exponent()/clock->Dt();
      _params.SetValue("exponent Re", _exp.real());
      _params.SetValue("exponent Im", _exp.imag());
      
      /* Remove WF buffers => new operator type  needs new WF type */
      if (ket0 != NULL) delete ket0;
      if (ket1 != NULL) delete ket1;
      if (ket2 != NULL) delete ket2;

   }

} /* namespace QDLIB */



