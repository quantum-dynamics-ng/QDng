#include "OSIL.h"

#include "linalg/LapackDiag.h"

namespace QDLIB {

    OSIL::OSIL()
            : OPropagator(), _name("OSIL"), _order(0), _Lzb(NULL), buf0(NULL), buf1(NULL), buf2(NULL)
    {
        _needs.SetValue("hamiltonian", 0);
    }


    OSIL::~OSIL()
    {
        if (_Lzb != NULL) {
           for (int i=0; i < _order; i++)
	      DELETE_WF(_Lzb[i]);
           delete[] _Lzb;
        }
        DELETE_WF(buf0);
	DELETE_WF(buf1);
	DELETE_WF(buf2);
    }


    Operator * OSIL::NewInstance()
    {
        OSIL* r = new OSIL();
        return r;
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
       
       _Lzb = new WaveFunction*[_order];

       for (int i=0; i<_order; i++){
	  _Lzb[i] = NULL;
       }
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
        if (Psi == NULL)
            throw(EParamProblem("SIL: Invalid WaveFunction"));

        for (int i = 0; i < _order; i++)
            _Lzb[i] = Psi->NewInstance();

        buf0 = Psi->NewInstance();
        buf1 = Psi->NewInstance();
        buf2 = Psi->NewInstance();
    }


    dcomplex OSIL::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
    {
        Apply(buf2, PsiKet);
        return (*PsiBra) * buf2;
    }

    double OSIL::Expec(WaveFunction * Psi)
    {
       return MatrixElement(Psi, Psi).real();
    }

    /**
     * Create Lanczos vectors + Hamiltonian.
     */
    void OSIL::BuildLZB(WaveFunction* Psi)
    {
        int it;

        *(_Lzb[0]) = Psi;
        H->Apply(buf0,  _Lzb[0]);           /* H*q_0 */
        _alpha[0] = (*(_Lzb[0]) * buf0).real();     /* a_0 = <q_0 | H | _q0 > */
        
        *(_Lzb[1]) = buf0;
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

    ParamContainer & QDLIB::OSIL::TellNeeds()
    {
        return _needs;
    }

    void QDLIB::OSIL::AddNeeds(string & Key, Operator * O)
    {
        if (Key == "hamiltonian"){
            if (O == NULL)
                throw(EParamProblem("SIL: Invalid hamiltonian"));
            
            H = O;
        } else
            throw(EParamProblem("SIL: Unknown operator key"));
    }
}










