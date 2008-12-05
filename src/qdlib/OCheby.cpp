#include "sys/Exception.h"
#include "OCheby.h"

#ifdef _OPENMP
#include <omp.h>
#endif

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

   
   WaveFunction * OCheby::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      WaveFunction *buf, *r;
      
      if (ket0 == NULL) ket0 = sourcePsi->NewInstance();
      if (ket1 == NULL) ket1 = sourcePsi->NewInstance();
      if (ket2 == NULL) ket2 = sourcePsi->NewInstance();
    
      buf = sourcePsi->NewInstance();
      r = sourcePsi->NewInstance();
      
      *r  = sourcePsi;  /* Copy */
      
      *ket0 = sourcePsi;   /* phi_0 */
      
      *ket1 = sourcePsi;
//       *_hamilton *= ket1;
      _hamilton->Apply(ket1);
       *ket1 *=  _exp;

      *r *= _coeff[0];

      *buf = ket1;
      *buf *= _coeff[1];

      *r += buf;

      int i=2;
      while (i < _order){

	 i++;
	 if(!(i < _order)) break;
	
	 i++;
      }

      delete buf;
      /* multiply the last two coefficients of the series expansion */
      return r;
   }

      
   WaveFunction * OCheby::Apply( WaveFunction * Psi, const dcomplex d )
   {
      Apply( Psi );
      *Psi *= d;
      return Psi;
   }
   
   WaveFunction * OCheby::Apply( WaveFunction * Psi, const double d )
   {
      Apply( Psi );
      *Psi *= d;
      return Psi;
   }
   
   WaveFunction * OCheby::Apply( WaveFunction * Psi )
   {
      WaveFunction *buf, *swap;
      
      if (ket0 == NULL) ket0 = Psi->NewInstance();
      if (ket1 == NULL) ket1 = Psi->NewInstance();
      if (ket2 == NULL) ket2 = Psi->NewInstance();
    
      buf = Psi->NewInstance();
      
      ket0->FastCopy(*Psi);   /* phi_0 */
      
      ket1->FastCopy(*Psi);
      _hamilton->Apply(ket1);
      MultElements( (cVec*) ket1, _exp);
      
//       *ket1 *=  _exp;
      
      *Psi *= _coeff[0];
     
      *buf = ket1;
      *buf *= _coeff[1];
      
      *Psi += buf;
      
      int i=2;
      dcomplex *k2, *bf, *k0, *psi;
//       #pragma omp parallel private(i, k2, bf, k0, psi)
//       {
      while (i < _order){
 	 _hamilton->Apply( buf, ket1);

	 
	 int strides = Psi->strides();
	 int size = Psi->lsize();
	 int s;
	 for (s=0; s < strides; s++){
	    k2 = ket2->begin(s);
	    bf = buf->begin(s);
	    k0 = ket0->begin(s);
	    psi = Psi->begin(s);
	    #pragma omp for
	    for(int j=0; j< size; j++){
	       bf[j] *= 2*_exp;
	       k2[j] = k0[j];
	       k2[j] += bf[j];
	       k0[j] = k2[j];
	       psi[j] += k2[j] *  _coeff[i];
	    }
	 }
	 	 
	 swap = ket1;
	 ket1 = ket0;
	 ket0 = swap;
	 i++;
/*	 _Recursion(ket0, ket1, buf, Psi, i);
	 i++;
	 if(!(i < _order)) break;
	 _Recursion(ket1, ket0, buf, Psi, i);
	 i++;*/
      }
//       } /* parallel*/

      delete buf;
      /* multiply the last two coefficients of the series expansion */
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
      ParamContainer *p = new ParamContainer();
      
      p->SetValue("hamiltonian", 0);
      
      return *p;
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
      
      if ( BesselJ0(_order, Rdelta * clock->Dt(), bessel, zeroes) != 0)
	 throw ( EOverflow("Error while calculating Bessel coefficients") );
      
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
      _coeff[0] = cexpI(OPropagator::Exponent().imag()*(Rdelta + Gmin)) * bessel[0];
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



