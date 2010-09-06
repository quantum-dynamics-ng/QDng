#include "tools/Exception.h"
#include "OCheby.h"
#include "math/math_functions.h"
#include "linalg/Bessel.h"

#ifdef HAVE_SSE2
 #include "math/cplx_sse2.h"
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OCheby)
	 
   OCheby::OCheby()
      : OPropagator(), _name("OCheby"),
      _order(0), _coeff(0), _scaling(1.), _offset(0), ket0(NULL), ket1(NULL), ket2(NULL), buf(NULL)
   {
      _needs.SetValue("hamiltonian", 0);
   }


   OCheby::~OCheby()
   {
      DELETE_WF(ket0);
      DELETE_WF(ket1);
      DELETE_WF(ket2);
      DELETE_WF(buf);
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
   void OCheby::Hamiltonian(Operator *Op)
   {
      H = Op;
   }
   
   void OCheby::Init( ParamContainer & params )
   {
      _params = params;
      if (_params.isPresent(string("order")) ){
	 _params.GetValue("order", _order);
	 if (_order < 1) throw ( EParamProblem("Chebychev recursion order invalid") );
      } else _order = 0;
   }
   
   dcomplex OCheby::MatrixElement(WaveFunction *PsiBra, WaveFunction *PsiKet)
   {
      return dcomplex(0,0); /* Doesn't make sense */
   }
   
   double OCheby::Expec( WaveFunction * Psi )
   {
      return 0; /* Doesn't make sense */
   }

   
   void OCheby::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
     
      destPsi->FastCopy(*sourcePsi);
      Apply(destPsi);
   }

         
   void OCheby::Apply( WaveFunction * Psi )
   {
      WaveFunction *swap;
      
      if (ket0 == NULL) ket0 = Psi->NewInstance();
      if (ket1 == NULL) ket1 = Psi->NewInstance();
      if (ket2 == NULL) ket2 = Psi->NewInstance();
      if (buf == NULL) buf =  Psi->NewInstance();
      
      _exp  = OPropagator::Exponent()/clock->Dt();

      ket0->FastCopy(*Psi);   /* phi_0 */
      
      ket1->FastCopy(*Psi);
      H->Apply(ket1);
      
      AddElements((cVec*) ket1, (cVec*) Psi, -1*_offset ); /* Offset*/
      MultElements( (cVec*) ket1, _exp / _scaling);
      
      MultElements( (cVec*) Psi, _coeff[0] * cexp(OPropagator::Exponent()*_offset));
      MultElementsCopy( (cVec*) buf, (cVec*) ket1, _coeff[1] * cexp(OPropagator::Exponent()*_offset));
      
      *Psi += buf;
      
      dcomplex *k2, *bf, *k0, *k1, *psi;

      int strides = Psi->strides();
      int size = Psi->lsize();
      
      dcomplex exp2 = 2*_exp;
	    
      for (int i=2; i < _order; i++){
         dcomplex coeff = _coeff[i] * cexp(OPropagator::Exponent()*_offset);
 	 H->Apply( buf, ket1);
         int s,j;

	 for (s=0; s < strides; s++){
	    k2 = ket2->begin(s);
	    bf = buf->begin(s);
	    k0 = ket0->begin(s);
	    k1 = ket1->begin(s);
	    psi = Psi->begin(s);
#ifdef HAVE_SSE2
       QDSSE::cplx_sse_packed cbf, ck1, ck2, cpsi, cexp2, ccoeff;
       QDSSE::cplx_sse_packed coffset;
       __m128d r1, r2, mscaling;

       LoadPacked(cexp2, exp2, exp2);
       LoadPacked(ccoeff, coeff, coeff);
       LoadPacked(coffset, _offset, _offset);
       mscaling = _mm_set_pd(1/_scaling, 1/_scaling);
#ifdef _OPENMP    
#pragma omp parallel for default(shared) private(j, cbf, ck2, cpsi, r1, r2)
#endif
            int size2 = size - size % 2;  /* multiples of two */
	    for (j = 0; j < size2; j += 2){
	        QDSSE::LoadPacked(cbf, bf[j], bf[j+1]);
		QDSSE::LoadPacked(ck1, k1[j], k1[j+1]);
	        QDSSE::LoadPacked(ck2, k0[j], k0[j+1]);
	        QDSSE::LoadPacked(cpsi, psi[j], psi[j+1]);
	    
		cbf = QDSSE::MulPacked(QDSSE::SubPacked(cbf, QDSSE::MulPacked(coffset, ck1)), mscaling);
		      
	        cbf = QDSSE::MulPacked(cbf, cexp2);
	        //bf[j] *= exp2;
	    
	        //k2[j] = k0[j];
	        ck2 = QDSSE::AddPacked(ck2, cbf);
	        //k2[j] += bf[j];
	    
	        //k0[j] = k2[j];
	        cpsi = QDSSE::AddPacked(cpsi, QDSSE::MulPacked(ck2, ccoeff));
	        //psi[j] += k2[j] *  _coeff[i];
	    
	        QDSSE::UnPack(r1, r2, cbf);
	        QDSSE::Store(bf[j], bf[j+1], r1, r2);
	    
	        QDSSE::UnPack(r1, r2, ck2);
	        QDSSE::Store(k2[j], k2[j+1], r1, r2);
	        QDSSE::Store(k0[j], k0[j+1], r1, r2);
	    
	        QDSSE::UnPack(r1, r2, cpsi);
	        QDSSE::Store(psi[j], psi[j+1], r1, r2);
	    }
            if (size % 2 == 1){  /* Non-vectorized version for single element */
               j=size-1;
               bf[j] = (bf[j] - _offset * k1[j]) / _scaling; /* Offset + Scaling */
               bf[j] *= exp2;
               k2[j] = k0[j];
               k2[j] += bf[j];
               k0[j] = k2[j];
               psi[j] += k2[j] * coeff;
            }
	 }

#else
#ifdef _OPENMP    
#pragma omp parallel for default(shared) private(j)
#endif
	    for(j=0; j< size; j++){
	       bf[j] = (bf[j] - _offset * k1[j]) / _scaling; /* Offset + Scaling */
	       bf[j] *= exp2;
	       k2[j] = k0[j];
	       k2[j] += bf[j];
	       k0[j] = k2[j];
	       psi[j] += k2[j] * coeff;
	    }
	 }
#endif
	 	 
	 swap = ket1;
	 ket1 = ket0;
	 ket0 = swap;
      }
   }

   Operator * OCheby::operator =( Operator * O )
   {
      Copy(O);
      return this;
   }

   Operator * QDLIB::OCheby::Copy(Operator * O)
   {
      OCheby *P;
      
      P = dynamic_cast<OCheby*>(O);
      if (P == NULL)
	 throw ( EIncompatible("Incompat in copy", this->Name(), O->Name()) );
      
      /* Copy parents */
      OPropagator::Copy(O);
      
      _isTimedependent = P->_isTimedependent;
      _params = P->_params;
      clock = P->clock;
      
      /* Copy own stuff */
      if (H != NULL) DELETE_OP(H);
      H = P->H->NewInstance();
      *H = P->H;
      _order = P->_order;
      _coeff = P->_coeff;
      _scaling = P->_scaling;
      _offset = P->_offset;
      
      return this;
   }
   
   ParamContainer & OCheby::TellNeeds( )
   {      
      return _needs;
   }

   
   void OCheby::AddNeeds( string & Key, Operator * O )
   {
      if (Key == "hamiltonian") H = O;
      else throw ( EParamProblem("Unknown operator key") );
   }

   
   void OCheby::Init( WaveFunction *Psi )
   {
      
      if (H == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a hamiltonian") );
      if (clock == NULL)
	 throw ( EParamProblem("Chebychev Progagator is missing a clock") );
      if (clock->Dt() == 0)
	 throw ( EParamProblem("No time step defined") );
      
      /* Energy range & offset */
      _offset._real =  (H->Emax() + H->Emin()).real()/2; /* [-i:i] */
      _offset._imag =  (H->Emax() + H->Emin()).imag();   /* [-1:0] */
      if (_offset.imag() != 0)
	_scaling = cabs(H->Emax() - H->Emin());
      else
	_scaling = cabs(H->Emax() - H->Emin())/2;
      
      
      /* This is an estimate for the recursion depth */
      if (_order <= 0)
	 _order =  abs(int(5 * _scaling * clock->Dt()));
      
      if (_order < 10) _order=10;

      /* Prepare the coefficients */
      dVec bessel;
      int zeroes=0;
      
      /* Manual scaling, very dangerous */
      if (_params.isPresent("scaling")) _params.GetValue("scaling", _scaling);
      /* Manual choice of recursion depth */
      if (_params.isPresent("order")) _params.GetValue("order", _order);
      
      if (_order < 3)
	 throw (EParamProblem("Chebychev recursion must be at least 3") );
	 
      /* Check for Real/imag time */
      if (fabs(OPropagator::Exponent().imag()) != 0 && fabs(OPropagator::Exponent().real()) == 0){
	 if ( BesselJ0(_order, _scaling * clock->Dt(), bessel, zeroes) != 0)
	    throw ( EOverflow("Error while calculating Bessel coefficients") );
      } else if (fabs(OPropagator::Exponent().imag()) == 0 && fabs(OPropagator::Exponent().real()) != 0){
	 if ( BesselI0(_order, _scaling * clock->Dt(), bessel, zeroes) != 0)
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
      if (_order > 0 && _order <= int(_scaling * clock->Dt()) )
	 throw ( EParamProblem("Chebychev recursion order is to small") );
      
      _params.SetValue("order", _order);
      _params.SetValue("scaling", _scaling);
      _params.SetValue("offset.real", _offset.real());
      _params.SetValue("offset.imag", _offset.imag());
      
      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = bessel[0];
      for (int i=1; i < _coeff.size(); i++){
         _coeff[i] = 2.0 * bessel[i];
      }
      
      _exp  = OPropagator::Exponent()/clock->Dt();

      _params.SetValue("exponent Re", _exp.real());
      _params.SetValue("exponent Im", _exp.imag());
      
      /* Remove WF buffers => new operator type  needs new WF type */
      DELETE_WF(ket0);
      DELETE_WF(ket1);
      DELETE_WF(ket2);

   }

   
   bool OCheby::Valid(WaveFunction * Psi)
   {
      return H->Valid(Psi);
   }
   
} /* namespace QDLIB */







