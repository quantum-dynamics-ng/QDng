#include "OSIL.h"

#include "linalg/LapackDiag.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OSIL)
	 
    OSIL::OSIL()
            : OPropagator(), _name("OSIL"), _order(0), buf0(NULL), buf1(NULL), buf2(NULL)
    {
    }


    OSIL::~OSIL()
    {
        DELETE_WF(buf0);
	DELETE_WF(buf1);
	DELETE_WF(buf2);
    }
    
    /**
     * Initialize vectors etc. with right size.
     */
    void OSIL::InitBuffers()
    {
       _alpha.newsize(_order);
       _beta.newsize(_order);
       _vect.newsize(_order);
       _vec0.newsize(_order);
       _expHD.newsize(_order);       
    }
    
    void OSIL::Init(ParamContainer & params)
    {
        _params = params;

        if (_params.isPresent("order"))
            _params.GetValue("order", _order);
        else {
            _order = SIL_DEF_ORDER;
            _params.SetValue("order", _order);
        }

        if (_order == 1)
            throw(EParamProblem("SIL: order doesn't make sense", _order));

        InitBuffers();
    }

    void OSIL::Init(WaveFunction * Psi)
    {
       OPropagator::Init(Psi);

        if (buf0 != NULL) return; //Avoid init twice

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
    void OSIL::BuildLZB(WaveFunction* Psi)
    {
        int it;

        buf0->Reaquire();
        buf1->Reaquire();

        _Lzb.Set(0, Psi);
        H->Apply(buf0,  _Lzb[0]);           /* H*q_0 */
        H->RecalcInternals(false);  /* Non-linear operator should not recalulate internal WF specfic values until end of recursion */
        _alpha[0] = (*(_Lzb[0]) * buf0).real();     /* a_0 = <q_0 | H | _q0 > */
        
        _Lzb.Set(1, buf0);
        MultElementsCopy((cVec*) buf1, (cVec*) _Lzb[0], _alpha[0]);
        *(_Lzb[1]) -= buf1;

        _beta[0] = _Lzb[1]->Norm();

        *(_Lzb[1]) *= 1 / _beta[0];     /* b0 = Norm(q1) = <q1 | H | q0 > */

        for (it = 2; it < _order; it++)
        {
            /* buf0 = H * q_i-1 */
            H->Apply(_Lzb[it], _Lzb[it-1]);
            _alpha[it-1] = (*(_Lzb[it-1]) * _Lzb[it]).real();
            /* - a_i-1 * q_i-1 */
            MultElementsCopy((cVec*) buf0, (cVec*) _Lzb[it-1], _alpha[it-1]);
            *(_Lzb[it]) -= buf0;
            /* - b_i-2 * q_i-2 */
            MultElementsCopy((cVec*) buf0, (cVec*) _Lzb[it-2], _beta[it-2]);
            *(_Lzb[it]) -= buf0;

            _beta[it-1] = _Lzb[it]->Norm();

            *(_Lzb[it]) *= 1 / _beta[it-1];

        }

        _alpha[it-1] = H->Expec(_Lzb[it-1]);
        H->RecalcInternals(true); /* turn it on again */

        buf0->Retire();
        buf1->Retire();
    }

    /**
     * Propagate one step with given Lanczos-Hamiltonian.
     *
     * Lzb, alpha and beta must present.
     */
    void OSIL::DiagLZB(WaveFunction* Psi)
    {
        /* Diag Lanczos Hamiltonian */
        LAPACK::TriDiagHermitian(&_alpha, &_beta, &_Z, _order);

        _vect = dcomplex(0);
        _vect[0] = 1;

        ExpElements(&_expHD, &_alpha, OPropagator::Exponent());
        /* Z * e^(-i*Hd*dt) * Z+ */
        MatVecMult(&_vec0, &_Z, &_vect, true);
        MultElements(&_vec0, &_expHD);
        MatVecMult(&_vect, &_Z, &_vec0);

        /* Build linear combination from Lanczos vectors */
        *Psi *= _vect[0];

        for (int i = 1; i < _order; i++){
            AddElements((cVec*) Psi, (cVec*) _Lzb[i], _vect[i]);
        }
    }


    void OSIL::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       *destPsi = sourcePsi;
       BuildLZB(destPsi);
       DiagLZB(destPsi);
    }

    void OSIL::Apply(WaveFunction * Psi)
    {
        BuildLZB(Psi);
        DiagLZB(Psi);
    }

    Operator * OSIL::operator =(Operator * O)
    {
        Copy(O);
        return this;
    }

    Operator * OSIL::Copy(Operator * O)
    {
        OSIL *o = dynamic_cast<OSIL*>(O);

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

    bool QDLIB::OSIL::Valid(WaveFunction * Psi)
    {
        if (H == NULL) return false;

        return H->Valid(Psi);
    }
}










