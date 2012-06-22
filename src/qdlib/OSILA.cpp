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

      _convorder = _order;

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

      _beta = 1.;

      _Lzb.Set(0, Psi);
      H->Apply(buf0, _Lzb[0]); /* q1 = H*q_0 */
      _Lzb.Set(1, buf0);
      H->RecalcInternals(false); /* Non-linear operator must not re-calculate internal WF specific values until end of recursion */

      _HA = dcomplex(0);

      for (it = 1; it < _order; it++) {
         H->Apply(buf0, _Lzb[it-1]);
         _Lzb.Set(it, buf0);

         for (int j=0; j <= it-1; j++){
            _HA(j, it-1) = *(_Lzb[j]) * _Lzb[it];
            AddElements(_Lzb[it], _Lzb[j], -1 * _HA(j, it-1));
         }


         double norm = _Lzb[it]->Norm();
         _HA(it, it-1) = norm;
         *(_Lzb[it]) *= 1./norm;
      }

      /* Last row */
      H->Apply(buf0, _Lzb[_order-1]);
      for (int j=0; j < _order; j++){
         _HA(j, _order-1) = *(_Lzb[j]) * buf0;
      }

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
      LAPACK::DiagHessenberg(&_HA, &_alpha, &_ZL, &_ZR);

      _vect = dcomplex(0);
      _vect[0] = 1;

      _ZL = _ZR;
      LAPACK::InvertGeneral(&_ZL);

      ExpElements(&_expHD, &_alpha, OPropagator::Exponent());
      /* Z * e^(-i*Hd*dt) * Z+ */
      MatVecMult(&_vec0, &_ZL, &_vect);
      MultElements(&_vec0, &_expHD);
      MatVecMult(&_vect, &_ZR, &_vec0);

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
      DiagLZB(destPsi);
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

