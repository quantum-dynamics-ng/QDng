#include "tools/Exception.h"
#include "OCheby.h"
#include "math/math_functions.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace QDLIB
{

   OCheby::OCheby()
      : OPropagator(), _name("OCheby"), _hamilton(NULL),
      _order(0), _coeff(0), Rdelta(0.0), Gmin(0), ket0(NULL), ket1(NULL), ket2(NULL), buf(NULL)
   {
      _needs.SetValue("hamiltonian", 0);
   }


   OCheby::~OCheby()
   {
      if (ket0 != NULL) delete ket0;
      if (ket1 != NULL) delete ket1;
      if (ket2 != NULL) delete ket2;
      if (buf != NULL) delete buf;
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

   
   WaveFunction * OCheby::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
     
      destPsi->FastCopy(*sourcePsi);
      Apply(destPsi);
      return destPsi;
   }

         
   WaveFunction * OCheby::Apply( WaveFunction * Psi )
   {
      WaveFunction *swap;
      
      if (ket0 == NULL) ket0 = Psi->NewInstance();
      if (ket1 == NULL) ket1 = Psi->NewInstance();
      if (ket2 == NULL) ket2 = Psi->NewInstance();
      if (buf == NULL) buf =  Psi->NewInstance();
      
      ket0->FastCopy(*Psi);   /* phi_0 */
      
      ket1->FastCopy(*Psi);
      _hamilton->Apply(ket1);
      MultElements( (cVec*) ket1, _exp);
      
      MultElements( (cVec*) Psi, _coeff[0]);
      
      MultElementsCopy( (cVec*) buf, (cVec*) ket1, _coeff[1]);
      
      *Psi += buf;
      
      dcomplex *k2, *bf, *k0, *psi;

      int strides = Psi->strides();
      int size = Psi->lsize();
      
      dcomplex exp2 = 2*_exp;
	    
      for (int i=2; i < _order; i++){
 	 _hamilton->Apply( buf, ket1);
	 
	 for (int s=0; s < strides; s++){
	    k2 = ket2->begin(s);
	    bf = buf->begin(s);
	    k0 = ket0->begin(s);
	    psi = Psi->begin(s);
	    for(int j=0; j< size; j++){
	       bf[j] *= exp2;
	       k2[j] = k0[j];
	       k2[j] += bf[j];
	       k0[j] = k2[j];
	       psi[j] += k2[j] *  _coeff[i];
	    }
	 }
	 	 
	 swap = ket1;
	 ket1 = ket0;
	 ket0 = swap;
      }

      return Psi;
   }

   Operator * OCheby::operator =( Operator * O )
   {

      OCheby *P;
      
      P = dynamic_cast<OCheby*>(O);
      if (P == NULL)
	 throw ( EIncompatible("Incompat in copy", this->Name(), O->Name()) );
      
      /* Copy parents */
      *((OPropagator*) this) = (OPropagator*) P;
      _isTimedependent = P->_isTimedependent;
      _params = P->_params;
      clock = P->clock;
      
      /* Copy own stuff */
      *_hamilton = P->_hamilton;
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
      return _needs;
   }

   
   void OCheby::AddNeeds( string & Key, Operator * O )
   {
      if (Key == "hamiltonian") _hamilton = O;
      else throw ( EParamProblem("Unknown operator key") );
   }

   
   void OCheby::Init( WaveFunction *Psi )
   {
      
      if (_hamilton == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a hamiltonian") );
      if (clock == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a clock") );
      if (clock->Dt() == 0)
	 throw ( EParamProblem("No time step defined") );
      
      /* Initalize H*/
      _hamilton->Init(Psi);
      
      /* Energy range & offset */
      Rdelta = (_hamilton->Emax() - _hamilton->Emin())/ 2;
      Gmin =  _hamilton->Emin();
      
      /* This is an estimate for the recursion depth */
      if (_order <= 0)
	 _order =  abs(int(5 * Rdelta * clock->Dt()));
      
      if (_order < 10) _order=10;

      /* Prepare the coefficients */
      dVec bessel;
      int zeroes=0;
      
      /* Manual scaling, very dangerous */
      if (_params.isPresent("scaling")) _params.GetValue("scaling", Rdelta);
      /* Manual choice of recursion depth */
      if (_params.isPresent("order")) _params.GetValue("order", _order);
      
      /* Check for Real/imag time */
      if (fabs(OPropagator::Exponent().imag()) != 0 && fabs(OPropagator::Exponent().real()) == 0){
	 if ( BesselJ0(_order, Rdelta * clock->Dt(), bessel, zeroes) != 0)
	    throw ( EOverflow("Error while calculating Bessel coefficients") );
      } else if (fabs(OPropagator::Exponent().imag()) == 0 && fabs(OPropagator::Exponent().real()) != 0){
	 if ( BesselI0(_order, Rdelta * clock->Dt(), bessel, zeroes) != 0)
	    throw ( EOverflow("Error while calculating Bessel coefficients") );
      } else {
	 throw (EParamProblem("Chebychev propagator doesn't support mixed real/complex exponents") );
      }
      
      
      /* Remove tailing zeroes (from underflow) */
      _order -= zeroes;
      
      /* Check the order which is really needed */
      if (! _params.isPresent("order") ){
	 double precission = BESSEL_DELTA;
	 if (_params.isPresent("prec"))
	    _params.GetValue("prec", precission);
	 int i=2;
	 /* Check how much is needed for series convergence */
	 while ( i < _order && abs(bessel[i - 1] - bessel[i - 2] ) > precission) {
	    i++;
	 }
	 _order = i;
	 _params.SetValue("prec", precission);
      }

	
      /* Check Recursion order */
      if (_order > 0 && _order <= int(Rdelta * clock->Dt()) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      /* Scale the Hamiltonian */
      _hamilton->Offset( -1*(Rdelta+Gmin) );
      
      _hamilton->Scale( 1/Rdelta );
      
      _params.SetValue("order", _order);
      _params.SetValue("scaling", Rdelta);
      _params.SetValue("offset", Gmin);
      
      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = cexp(OPropagator::Exponent()*(Rdelta + Gmin)) * bessel[0];
      for (int i=1; i < _coeff.size(); i++){
	 _coeff[i] = 2.0 * cexp(OPropagator::Exponent()*(Rdelta + Gmin)) * bessel[i];
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



