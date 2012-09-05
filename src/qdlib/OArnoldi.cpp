#include "OArnoldi.h"

#include "linalg/LapackDiag.h"

namespace QDLIB
{

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OArnoldi)

   OArnoldi::OArnoldi() :
            OPropagator(), _name("OArnoldi"), _maxorder(ARNOLDI_DEF_ORDER), _order(ARNOLDI_DEF_ORDER), _conv(ARNOLDI_CONV),
            buf0(NULL), buf1(NULL)
   {
   }

   OArnoldi::~OArnoldi()
   {
      DELETE_WF(buf0);
      DELETE_WF(buf1);
   }

   /**
    * Initialize vectors etc. with right size.
    */
   void OArnoldi::InitBuffers()
   {
      _evals.newsize(_maxorder);
      _HA.newsize(_maxorder, _maxorder);
      _tHA.newsize(_maxorder, _maxorder);
      _vect.newsize(_maxorder);
      _vec0.newsize(_maxorder);
      _expHD.newsize(_maxorder);
   }

   void OArnoldi::Init(ParamContainer & params)
   {
      _params = params;

      if (_params.isPresent("order"))
         _params.GetValue("order", _maxorder);

      if (_params.isPresent("conv"))
         _params.GetValue("conv", _conv);


      if (_maxorder < 3)
         throw(EParamProblem("Arnoldi: order doesn't make sense", _maxorder));

      _order = _maxorder;

//      if (_params.isPresent("err"))
//         _params.GetValue("err", _err);

      InitBuffers();
   }

   void OArnoldi::Init(WaveFunction * Psi)
   {
      OPropagator::Init(Psi);

      if (buf0 != NULL)
         return; //Avoid init twice

      H = Get("hamiltonian");

      buf0 = Psi->NewInstance();
      buf1 = Psi->NewInstance();

      buf0->Retire();
      buf1->Retire();

      _Lzb.Init(Psi);
      _Lzb.ResizeBuffer(_maxorder);
      _Lzb.AutoLock(1);

      H->Clock(Operator::Clock());
   }

   /**
    * Create Arnoldi vectors + Hamiltonian.
    */
   void OArnoldi::BuildLZB(WaveFunction* Psi)
   {
      buf0->Reaquire();
      buf1->Reaquire();

      double PsiNorm = Psi->Norm();

      *buf1 = Psi;

      _Lzb.Set(0, buf1);

      _tHA = dcomplex(0);

      for (int it = 1; it < _maxorder; it++) {
         H->Apply(buf0, _Lzb[it-1]);
         if (it == 1)
            H->RecalcInternals(false); /* Non-linear operator must not re-calculate internal WF specific values until end of recursion */

         /* Orthogonalize */
         for (int j=0; j <= it-1; j++){
            WaveFunction* wfj = _Lzb[j];
            _tHA(j, it-1) = *(_Lzb[j]) * buf0;
            AddElements(buf0, wfj, -1 * _tHA(j, it-1));
         }

         double norm = buf0->Norm();

         if (norm < ARNOLDI_UNDERRUN){
            break;  /* Zero vector reached */
         }

         _tHA(it, it-1) = norm;
         *buf0 *= 1./norm;

         _Lzb.Set(it, buf0);
         _order = it+1;

         /* Estimate the error of the next L-vector */
         dcomplex err = pow(clock->Dt(), it-2) * _tHA(1,0) * PsiNorm;
         for (int i=1; i < it - 1; i++){
            err *=  _tHA(i+1,i) / double(i);
         }

         if (cabs(err) < _conv) break;
      }

      /* Last row */
      H->Apply(buf0, _Lzb[_order-1]);
      for (int j=0; j < _order; j++){
         _tHA(j, _order-1) = *(_Lzb[j]) * buf0;
      }

      /* Copy hamiltonian to new matrix */
      _HA.newsize(_order, _order);
      for (int i=0; i < _order; i++){
         for (int j=0; j < _order; j++){
            _HA(i,j) = _tHA(i,j);
         }
      }

      H->RecalcInternals(true); /* turn it on again */

      buf0->Retire();
      buf1->Retire();
   }

   void OArnoldi::DiagLZB()
   {
      /* Diag Arnoldi Hamiltonian */
      _evals.newsize(_order);
      _ZL.newsize(_order,_order);
      _ZR.newsize(_order,_order);
      LAPACK::DiagHessenberg(&_HA, &_evals, &_ZL, &_ZR);
   }

   /**
    * Propagate one step with given Arnoldi-Hamiltonian.
    *
    * Diagonalized Hamiltonian must be present
    */
   void OArnoldi::Propagate(WaveFunction* Psi)
   {
      _vect.newsize(_order);
      _vec0.newsize(_order);
      _expHD.newsize(_order);

      _vect = dcomplex(0);
      _vect[0] = 1;

      _ZL = _ZR;
      LAPACK::InvertGeneral(&_ZL);

      ExpElements(&_expHD, &_evals, OPropagator::Exponent());
      /* Z * e^(-i*Hd*dt) * Z+ */
      MatVecMult(&_vec0, &_ZL, &_vect);
      MultElements(&_vec0, &_expHD);
      MatVecMult(&_vect, &_ZR, &_vec0);

      /* Build linear combination from Lanczos vectors */
      *Psi *= _vect[0];

      for (int i = 1; i < _order; i++) {
         AddElements((cVec*) Psi, (cVec*) _Lzb[i], _vect[i]);
      }
   }

   void  OArnoldi::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      *destPsi = sourcePsi;
      BuildLZB(destPsi);
      DiagLZB();
      Propagate(destPsi);
   }

   void   OArnoldi::Apply(WaveFunction * Psi)
   {
      BuildLZB(Psi);
      DiagLZB();
      Propagate(Psi);
   }

   Operator* OArnoldi::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator* OArnoldi::Copy(Operator * O)
   {
      OArnoldi *o = dynamic_cast<OArnoldi*>(O);

      if (o == NULL)
         throw(EIncompatible("OArnoldi: unable to copy: ", O->Name()));

      OPropagator::Copy(O);

      _maxorder = o->_maxorder;

      InitBuffers();
      buf0 = o->buf0->NewInstance();
      buf1 = o->buf0->NewInstance();

      H = o->H->NewInstance();
      H->Copy(o->H);

      OPropagator::Copy(o);
      return this;
   }

   bool OArnoldi::Valid(WaveFunction * Psi)
   {
      if (H == NULL)
         return false;

      return H->Valid(Psi);
   }
}

