// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "tools/Exception.h"
#include "OCheby.h"
#include "math/math_functions.h"
#include "linalg/Bessel.h"

#ifdef HAVE_SSE2
#include "math/m128dc.h"
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OCheby)

   OCheby::OCheby() :
      OPropagator(), _name("OCheby"), _order(0), _coeff(0), _scaling(1.), _prec(DEFAULT_PREC),
                     _offset(0), ket0(NULL),
                      ket1(NULL), ket2(NULL), buf(NULL)
   {
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
   int OCheby::Recursion()
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

   void OCheby::Init(ParamContainer & params)
   {
      _params = params;
      if (_params.isPresent(string("order"))) {
         _params.GetValue("order", _order);
         if (_order < 1)
            throw(EParamProblem("Chebychev recursion order invalid"));
      } else _order = 0;
   }


   double OCheby::raw_norm(WaveFunction *Psi)
   {
      int rank = Psi->MPIrank();
      int msize = Psi->MPIsize();

      int strides = Psi->strides();
      int size = Psi->lsize();

      double norm(0);

#ifdef _OPENMP
#pragma omp parallel for shared(norm)
#endif
      for (int s = rank; s < strides; s += msize) {
         double lnorm(0);
         dcomplex* wf = Psi->begin(s);
#ifdef _OPENMP
#pragma omp parallel for reduction(+:lnorm)
#endif
         for (int j = 0; j < size; j++) {
            lnorm += (conj(wf[j]) * wf[j]).real();
         }
         norm += lnorm;
      }

      return norm;
   }

   void OCheby::real_time_step(WaveFunction *Psi)
   {
      WaveFunction *swap;
      double norm = raw_norm(Psi);

      _exp = OPropagator::Exponent() / clock->Dt();

      ket0->FastCopy(*Psi); /* phi_0 */

      H->Apply(ket1, Psi);
      H->RecalcInternals(false); /* Non-linear operator should not re-calculate internal WF specific values until end of recursion */

      AddElements((cVec*) ket1, (cVec*) Psi, -1 * _offset); /* Offset*/
      MultElements((cVec*) ket1, _exp / _scaling);

      MultElements((cVec*) Psi, _coeff[0] * cexp(OPropagator::Exponent() * _offset));
      MultElementsCopy((cVec*) buf, (cVec*) ket1, _coeff[1] * cexp(OPropagator::Exponent()
               * _offset));

      *Psi += buf;
      dcomplex *k2, *bf, *k0, *k1, *psi;

      int strides = Psi->strides();
      int size = Psi->lsize();

      dcomplex exp2 = 2 * _exp;


      for (int i = 2; i < _order; i++) {
	 double accnorm(0);
         dcomplex coeff = _coeff[i] * cexp(OPropagator::Exponent() * _offset);
         H->Apply(buf, ket1);
         int s;
         int rank = Psi->MPIrank();
         int msize = Psi->MPIsize();
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,k0,k1,k2,bf,psi) reduction(+:accnorm)
#endif
         for (s = rank; s < strides; s += msize) {
            int j=0;
            bf = buf->begin(s);
            k0 = ket0->begin(s);
            k1 = ket1->begin(s);
            k2 = ket2->begin(s);
            psi = Psi->begin(s);
#ifdef HAVE_SSE2
            m128dc vk2, vbf, vk0, vk1, vpsi, v_o(_offset), v_exp2(exp2), v_coeff(coeff);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(j, vk0, vk1, vk2, vbf, vpsi) reduction(+:accnorm)
#endif
            for(j=0; j< size; j++) {
               vbf = bf[j];
               vk0 = k0[j];
               vk1 = k1[j];
               vpsi = psi[j];

               vbf = vbf - vk1 * v_o;
               vbf *= (1./_scaling);
               vbf *= v_exp2;
               vk2 = vk0 + vbf;
               vk0 = vk2;
               vpsi += vk2 * v_coeff;
               accnorm = accnorm + dcomplex(vpsi.conj() * vpsi).real();

               vbf.Store(bf[j]);
               vk0.Store(k0[j]);
               vk1.Store(k1[j]);
               vk2.Store(k2[j]);
               vpsi.Store(psi[j]);
            }
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(j) reduction(+:accnorm)
#endif
            for (j = 0; j < size; j++) {
               bf[j] = (bf[j] - _offset * k1[j]) / _scaling; /* Offset + Scaling */
               bf[j] *= exp2;
               k2[j] = bf[j] + k0[j];
               k0[j] = k2[j];
               psi[j] += k2[j] * coeff;
               accnorm = accnorm + (conj(psi[j]) * psi[j]).real();
            }
         }
#endif

         /* Check for convergence */
         double nconv = abs(norm - accnorm) / norm;

         if (nconv  < _prec && _offset.imag() == 0)
            break;

         /* Convergence only make sense for norm preserving propagations */
         if (i == _order -1 && nconv  > _prec && H->Hermitian()){
            cout << "Warning: Chebychev series not converged: " << scientific << nconv << " > " << _prec  << endl;
            cout << Psi->Norm() << " " << norm << " " << accnorm << endl;
         }


         swap = ket1;
         ket1 = ket0;
         ket0 = swap;
      }
      H->RecalcInternals(true); /* turn it on again */


   }

   void OCheby::imag_time_step(WaveFunction *Psi)
   {
      WaveFunction *swap;
      double norm_last = 0;

      _exp = OPropagator::Exponent() / clock->Dt();

      ket0->FastCopy(*Psi); /* phi_0 */

      H->Apply(ket1, Psi);
      H->RecalcInternals(false); /* Non-linear operator should not re-calculate internal WF specific values until end of recursion */

      AddElements((cVec*) ket1, (cVec*) Psi, -1 * _offset); /* Offset*/
      MultElements((cVec*) ket1, _exp / _scaling);

      MultElements((cVec*) Psi, _coeff[0] * cexp(OPropagator::Exponent() * _offset));
      MultElementsCopy((cVec*) buf, (cVec*) ket1, _coeff[1] * cexp(OPropagator::Exponent() * _offset));

      *Psi += buf;
      dcomplex *k2, *bf, *k0, *k1, *psi;

      int strides = Psi->strides();
      int size = Psi->lsize();

      dcomplex exp2 = 2 * _exp;


      for (int i = 2; i < _order; i++) {
         double accnorm(0);
         dcomplex coeff = _coeff[i] * cexp(OPropagator::Exponent() * _offset);
         H->Apply(buf, ket1);
         int s, j;
         int rank = Psi->MPIrank();
         int msize = Psi->MPIsize();
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(s,j,k0,k1,k2,bf,psi) reduction(+:accnorm)
#endif
         for (s = rank; s < strides; s += msize) {
            k2 = ket2->begin(s);
            bf = buf->begin(s);
            k0 = ket0->begin(s);
            k1 = ket1->begin(s);
            psi = Psi->begin(s);
#ifdef HAVE_SSE2
            m128dc vk2, vbf, vk0, vk1, vpsi, v_o(_offset), v_exp2(exp2), v_coeff(coeff);
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(j,vk2, vbf, vk0, vk1, vpsi) reduction(+:accnorm)
#endif
            for(j=0; j< size; j++) {
               vbf = bf[j];
               vk0 = k0[j];
               vk1 = k1[j];
               vpsi = psi[j];

               vbf = vbf - vk1 * v_o;
               vbf *= (1/_scaling);
               vbf *= v_exp2;
               vk2 = vbf - vk0;
               vk0 = vk2;
               vpsi += vk2 * v_coeff;
               accnorm = accnorm + dcomplex(vpsi.conj() * vpsi).real();

               vbf.Store(bf[j]);
               vk0.Store(k0[j]);
               vk1.Store(k1[j]);
               vk2.Store(k2[j]);
               vpsi.Store(psi[j]);
            }
         }
#else
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(j) reduction(+:accnorm)
#endif
            for (j = 0; j < size; j++) {
               bf[j] = (bf[j] - _offset * k1[j]) / _scaling; /* Offset + Scaling */
               bf[j] *= exp2;
               k2[j] = bf[j] - k0[j]; // Here must be a minus for the imag_time polynomials
               k0[j] = k2[j];
               psi[j] += k2[j] * coeff;
               accnorm = accnorm + (conj(psi[j]) * psi[j]).real();
            }

         }
#endif

         /* Check for convergence */
         if (abs(accnorm - norm_last)/norm_last < _prec)
            break;

         norm_last = accnorm;

         swap = ket1;
         ket1 = ket0;
         ket0 = swap;
      }
      H->RecalcInternals(true); /* turn it on again */

   }

   void OCheby::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {

      destPsi->FastCopy(*sourcePsi);
      Apply(destPsi);
   }

   void OCheby::Apply(WaveFunction * Psi)
   {
      ket0->Reaquire();
      ket1->Reaquire();
      ket2->Reaquire();
      buf->Reaquire();

      if (imaginary_time)
         imag_time_step(Psi);
      else
         real_time_step(Psi);

      ket0->Retire();
      ket1->Retire();
      ket2->Retire();
      buf->Retire();
   }

   Operator * OCheby::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator * QDLIB::OCheby::Copy(Operator * O)
   {
      OCheby *P;

      P = dynamic_cast<OCheby*> (O);
      if (P == NULL)
         throw(EIncompatible("Incompat in copy", this->Name(), O->Name()));

      /* Copy parents */
      OPropagator::Copy(O);

      _isTimedependent = P->_isTimedependent;
      _params = P->_params;
      clock = P->clock;

      /* Copy own stuff */
      if (H != NULL)
         DELETE_OP(H);
      H = P->H->NewInstance();
      *H = P->H;
      _order = P->_order;
      _coeff = P->_coeff;
      _scaling = P->_scaling;
      _offset = P->_offset;

      return this;
   }
   
   void OCheby::Init( WaveFunction *Psi )
   {
      H = Get("hamiltonian");
      H->Clock(Operator::Clock());
      H->Init(Psi);

      if (H == NULL)
         throw ( EParamProblem("Chebychev Progagator is missing a hamiltonian") );

      /* Parent Initalization */
      OPropagator::Init(Psi);


      if (ket0 != NULL) return;  // Avoid init twice
      ket0 = Psi->NewInstance();
      ket1 = Psi->NewInstance();
      ket2 = Psi->NewInstance();
      buf = Psi->NewInstance();

      ket0->Retire();
      ket1->Retire();
      ket2->Retire();
      buf->Retire();

      dcomplex Emax(H->Emax());
      dcomplex Emin(H->Emin());
      double v;

      /* Manual overide for Emax/Emin values */
      if (_params.isPresent("Emax")){
         _params.GetValue("Emax", v);
         Emax.real(v);
      }
      if (_params.isPresent("Emax_im")){
         _params.GetValue("Emax_im", v);
         Emax.imag(v);
      }
      if (_params.isPresent("Emin")){
         _params.GetValue("Emin", v);
         Emin.real(v);
      }
      if (_params.isPresent("Emin_im")){
         _params.GetValue("Emin_im", v);
         Emin.imag(v);
      }


      /* Energy range & offset */
      if (!imaginary_time){
         _offset._real = (Emax + Emin).real() / 2; /* [-i:i] */
         _offset._imag = (Emax + Emin).imag(); /* [-1:0] */
      } else {
         _offset = Emin - cabs(Emin)*0.05; /* */
      }

      if (_offset.imag() != 0 || fabs(OPropagator::Exponent().imag()) == 0)
         _scaling = cabs(Emax - Emin)*1.05;
      else
         _scaling = cabs(Emax - Emin) / 2;

      /* This is an estimate for the recursion depth */
      if (_order <= 0)
         _order = abs(int(20 * _scaling * clock->Dt()));

      if (_order < 10)
         _order = 10;

      /* Prepare the coefficients */
      dVec bessel;
      int zeroes = 0;

      /* Manual scaling, very dangerous */
      if (_params.isPresent("scaling"))
         _params.GetValue("scaling", _scaling);
      /* Manual choice of recursion depth */
      if (_params.isPresent("order"))
         _params.GetValue("order", _order);

      if (_order < 3)
         throw(EParamProblem("Chebychev recursion must be at least 3"));

      /* Check for Real/imag time */
      if (fabs(OPropagator::Exponent().imag()) != 0 && fabs(OPropagator::Exponent().real()) == 0) {
         if (BesselJ0(_order, _scaling * clock->Dt(), bessel, zeroes) != 0)
            throw(EOverflow("Error while calculating Bessel coefficients"));
      } else if (fabs(OPropagator::Exponent().imag()) == 0 && fabs(OPropagator::Exponent().real()) != 0) {
         if (BesselI0(_order, _scaling * clock->Dt(), bessel, zeroes) != 0)
            throw(EOverflow("Error while calculating Bessel coefficients"));
      } else {
         throw(EParamProblem("Chebychev propagator doesn't support mixed real/complex exponents"));
      }

      /* Remove tailing zeroes (from underflow) */
      _order -= zeroes;

      /* Check the order which is really needed */
      if (!_params.isPresent("order")) {
         double precission = BESSEL_DELTA;
         int i = 2;
         /* Check how much is needed for series convergence */
         while (i < _order && abs(bessel[i - 1] - bessel[i - 2]) > precission) {
            i++;
         }
         _order = i;
         _params.SetValue("Bessel prec", precission);
         _params.SetValue("BesselDelta", abs(bessel[_order - 1] - bessel[_order - 2]));
      }

      /* Check Recursion order */
      if (_order > 0 && !imaginary_time && _order <= int(_scaling * clock->Dt()))
         throw(EParamProblem("Chebychev recursion order is to small"));

      _params.SetValue("order", _order);
      _params.SetValue("scaling", _scaling);
      _params.SetValue("offset.real", _offset.real());
      _params.SetValue("offset.imag", _offset.imag());

      /* Setup coefficients */
      _coeff.newsize(_order);
      _coeff[0] = bessel[0];
      for (int i = 1; i < _coeff.size(); i++) {
         _coeff[i] = 2.0 * bessel[i];
      }

      /* Check for precision on norm */
      if (_params.isPresent("prec"))
         _params.GetValue("prec", _prec);

      _params.SetValue("prec", _prec);

      _exp = OPropagator::Exponent() / clock->Dt();

      _params.SetValue("exponent Re", _exp.real());
      _params.SetValue("exponent Im", _exp.imag());

   }

   bool OCheby::Valid(WaveFunction * Psi)
   {
      return H->Valid(Psi);
   }

} /* namespace QDLIB */

