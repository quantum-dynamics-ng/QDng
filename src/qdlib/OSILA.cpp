#include "OSILA.h"

#include "linalg/LapackDiag.h"

namespace QDLIB
{

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OSILA)

   OSILA::OSILA() :
            OPropagator(), _name("OSILA"), _order(SIL_DEF_ORDER), _convorder(SIL_DEF_ORDER), _err(SIL_DEF_ERR),
            buf0(NULL), buf1(NULL), buf2(NULL)
   {
   }

   OSILA::~OSILA()
   {
      DELETE_WF(buf0);
      DELETE_WF(buf1);
      DELETE_WF(buf2);
   }

   /**
    * Initialize vectors etc. with right size.
    */
   void OSILA::InitBuffers()
   {
      _alpha.newsize(_order);
      _beta.newsize(_order);
      _HA.newsize(_order, _order);
      _vect.newsize(_order);
      _vec0.newsize(_order);
      _expHD.newsize(_order);
   }

   void OSILA::Init(ParamContainer & params)
   {
      _params = params;

      if (_params.isPresent("order"))
         _params.GetValue("order", _order);

      if (_order < 3)
         throw(EParamProblem("SIL: order doesn't make sense", _order));

      if (_params.isPresent("err"))
         _params.GetValue("err", _err);

      InitBuffers();
   }

   void OSILA::Init(WaveFunction * Psi)
   {
      OPropagator::Init(Psi);

      if (buf0 != NULL)
         return; //Avoid init twice

      H = Get("hamiltonian");

      buf0 = Psi->NewInstance();
      buf1 = Psi->NewInstance();
      buf2 = Psi->NewInstance();

      buf0->Retire();
      buf1->Retire();
      buf2->Retire();

      _Lzb.Size(_order);
      _Lzb.Init(Psi);
      _Lzb.AutoLock(3);

      H->Clock(Operator::Clock());
   }

   /**
    * Create Lanczos vectors + Hamiltonian.
    */
   void OSILA::BuildLZB(WaveFunction* Psi)
   {
      int it;

      buf0->Reaquire();
      buf1->Reaquire();

//      _alpha = 0;
//      _beta = 0;

//      double psinorm = Psi->Norm();

      _Lzb.Set(0, Psi);
      H->Apply(buf0, _Lzb[0]); /* q1 = H*q_0 */
      _Lzb.Set(1, buf0);
      H->RecalcInternals(false); /* Non-linear operator must not re-calculate internal WF specific values until end of recursion */

      _HA = dcomplex(0);
      _HA(0,0) = H->MatrixElement(_Lzb[0], _Lzb[0]);
      for (it = 1; it < _order; it++) {
         H->Apply(buf0, _Lzb[it-1]);
         _Lzb.Set(it, buf0);

         for (int j=0; j < it-1; j++){
            cout << j << ", " << it-1 <<endl;
            _HA(j, it-1) = *(_Lzb[j]) * _Lzb[it];
            AddElements(_Lzb[it], _Lzb[j], -1 * _HA(j, it-1));
         }

         _HA(it, it-1) = _Lzb[it]->Norm();
         *(_Lzb[it]) *= 1./_Lzb[it]->Norm();

         _HA(it,it) = H->MatrixElement(_Lzb[it], _Lzb[it]);
      }

      cout << _HA;
//      ///////
//      _alpha[0] = *(_Lzb[0]) * buf0; /* a_0 = <q_0 | H | _q0 > */
//
//      _Lzb.Set(1, buf0);
//      MultElementsCopy((cVec*) buf1, (cVec*) _Lzb[0], _alpha[0]);
//      *(_Lzb[1]) -= buf1;
//
//      _beta[0] = _Lzb[1]->Norm();
//
//      *(_Lzb[1]) *= 1 / _beta[0]; /* b0 = Norm(q1) = <q1 | H | q0 > */
//
//      _convorder = 2;
//      for (it = 2; it < _order; it++) {
//         /* Estimate the error the next L-vector */
//         dcomplex err = pow(clock->Dt(), it - 2) * _beta[0];
//         for (int i = 1; i < it - 2; i++) {
//            err *= _beta[i] / double(i);
//         }
//         err = cabs(err);
//
//         if (err.real() > psinorm) {
//            cout << "Warning: Lanczos expansion becomes unstable: " << err.real() << endl;
//            //break;
//         }
//         _convorder++;
//
//         if (it == _order - 1)
//            cout << "Warning: Lanczos expansion is not converged: " << err.real() << " > " << _err * psinorm << endl;
//
//         /* buf0 = H * q_i-1 */
//         H->Apply(_Lzb[it], _Lzb[it - 1]);
//         _alpha[it - 1] = (*(_Lzb[it - 1]) * _Lzb[it]).real();
//         /* - a_i-1 * q_i-1 */
//         MultElementsCopy((cVec*) buf0, (cVec*) _Lzb[it - 1], _alpha[it - 1]);
//         *(_Lzb[it]) -= buf0;
//         /* - b_i-2 * q_i-2 */
//         MultElementsCopy((cVec*) buf0, (cVec*) _Lzb[it - 2], _beta[it - 2]);
//         *(_Lzb[it]) -= buf0;
//
//         _beta[it - 1] = _Lzb[it]->Norm();
//
//         *(_Lzb[it]) *= 1 / _beta[it - 1];
//
//         /* Check orthogonality of LZB */
//         if (it > 10) {
//            double d = cabs(*(_Lzb[0]) * _Lzb[it]);
//            if (d > 1000 * _err)
//               cout << "Warning: Lanczos basis becomes non orthogonal: " << it << " " << d << endl;
//         }
//
//         /* convergence reached */
//         if (err.real() < _err * psinorm)
//            break;
//      }
//
//      _alpha[it - 1] = H->MatrixElement(_Lzb[it - 1], _Lzb[it - 1]);
      H->RecalcInternals(true); /* turn it on again */

      buf0->Retire();
      buf1->Retire();
   }

   /**
    * Propagate one step with given Lanczos-Hamiltonian.
    *
    * Lzb, alpha and beta must present.
    */
   void   OSILA::DiagLZB(WaveFunction* Psi)
   {
      /* Diag Lanczos Hamiltonian */
//      LAPACK::
//      LAPACK::TriDiagHermitian(&_alpha, &_beta, &_Z, _order);
      LAPACK::DiagHessenberg(&_HA, &_alpha, &_Z);

      _vect = dcomplex(0);
      _vect[0] = 1;

      ExpElements(&_expHD, &_alpha, OPropagator::Exponent());
      /* Z * e^(-i*Hd*dt) * Z+ */
      MatVecMult(&_vec0, &_Z, &_vect, true);
      MultElements(&_vec0, &_expHD);
      MatVecMult(&_vect, &_Z, &_vec0);

      /* Build linear combination from Lanczos vectors */
      *Psi *= _vect[0];

      for (int i = 1; i < _convorder; i++) {
         AddElements((cVec*) Psi, (cVec*) _Lzb[i], _vect[i]);
      }
   }

   void  OSILA::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      *destPsi = sourcePsi;
      BuildLZB(destPsi);
//      DiagLZB(destPsi);
   }

   void   OSILA::Apply(WaveFunction * Psi)
   {
      BuildLZB(Psi);
      DiagLZB(Psi);
   }

   Operator* OSILA::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator* OSILA::Copy(Operator * O)
   {
      OSILA *o = dynamic_cast<OSILA*>(O);

      if (o == NULL)
         throw(EIncompatible("OSIL: unable to copy: ", O->Name()));

      OPropagator::Copy(O);

      _order = o->_order;

      InitBuffers();
      buf0 = o->buf0->NewInstance();
      buf1 = o->buf0->NewInstance();
      buf2 = o->buf0->NewInstance();

      H = o->H->NewInstance();
      H->Copy(o->H);

      return this;
   }

   bool OSILA::Valid(WaveFunction * Psi)
   {
      if (H == NULL)
         return false;

      return H->Valid(Psi);
   }
}

