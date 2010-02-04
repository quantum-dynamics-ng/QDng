#include "OSIL.h"

#include "linalg/LapackDiag.h"

namespace QDLIB {

    OSIL::OSIL()
            : OPropagator(), _name("OSIL"), _order(0), _hamilton(NULL), buf0(NULL), buf1(NULL), buf2(NULL)
    {
       for (int i=0; i < SIL_MAX_ORDER; i++)
          _Lzb[i] = NULL;
       
       _alpha.newsize(SIL_MAX_ORDER);
       _beta.newsize(SIL_MAX_ORDER);
       _vect.newsize(SIL_MAX_ORDER);
       _expHD.newsize(SIL_MAX_ORDER);
       _needs.SetValue("hamiltonian",0);
    }


    OSIL::~OSIL()
    {
       for (int i=0; i < SIL_MAX_ORDER; i++)
          if (_Lzb[i] != NULL) delete _Lzb[i];
       
       if (buf0 != NULL) delete buf0;
       if (buf1 != NULL) delete buf1;
       if (buf2 != NULL) delete buf2;
    }


    Operator * OSIL::NewInstance()
    {
       OSIL* r = new OSIL();
       return r;
    }
 
    void OSIL::Init(ParamContainer & params)
    {
       _params = params;
       
       _params.GetValue("order", _order);
       
       if (_order == 1)
          throw( EParamProblem("SIL: order doesn't make sense", _order) );
       
    }

    void OSIL::Init(WaveFunction * Psi)
    {
       if (Psi == NULL)
          throw (EParamProblem("SIL: Invalid WaveFunction"));
       
       int max;
       
       if (_order < 1) max = SIL_MAX_ORDER;
       else max = _order;
       
       for (int i=0; i < max; i++)
         _Lzb[i] = Psi->NewInstance();
       
       buf0=Psi->NewInstance();
       buf1=Psi->NewInstance();
       buf2=Psi->NewInstance();
       
       _hamilton->Init(Psi);
    }

    
    dcomplex OSIL::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
    {
       return dcomplex(0);
    }

    double OSIL::Expec(WaveFunction * Psi)
    {
       return 0;
    }

    /**
     * Propagate one step with given Lanczos-Hamiltonian.
     * 
     * Lzb, alpha and beta must present.
     */
    void OSIL::PropaLCB(WaveFunction* Psi, int basisLength)
    {
       cout << "Do step with lzb_len " << basisLength << endl;
       cout << "alpha\n"<<_alpha;
       cout << "beta\n"<<_beta;
       /* Diag Lanczos Hamiltonian */
       LAPACK::TriDiagHermitian(&_alpha, &_beta, &_Z, basisLength);
       
       _vect.newsize(basisLength);
       _vec0.newsize(basisLength);
       _vect = dcomplex(0);
       _vect[0] = 1;
       _expHD.newsize(basisLength);
       _HD.newsize(basisLength);
       _HD.CopySub(_alpha, 0, basisLength-1);
       cout << "HD\n"<<_HD;
       cout << "Z\n"<<_Z;
       for (int i=0; i < basisLength; i++)
          _HD.newsize(basisLength);
       
       ExpElements(&_expHD, &_HD, OPropagator::Exponent());
       cout << "e^HD\n"<<_expHD;
//        cout << "vect\n"<<_vec0;
       /* Z * e^(-i*Hd*dt) * Z+ */
       MatVecMult(&_vec0, &_Z, &_vect, true);
//        cout << "vect\n"<<_vec0;
       MultElements(&_vec0, &_expHD);
//        cout << "vect\n"<<_vec0;
       MatVecMult(&_vect, &_Z, &_vec0);
       cout << "vect\n"<<_vect;
       
       dcomplex norm=conj(_vect[0]) * _vect[0];
       /* Build linear combination from Lzb vector */
       *Psi *= _vect[0];
//        cout << "lsb_0"<<"\n"<< *(_Lzb[0]);
       for (int i=1; i < basisLength; i++){
//            cout << "lsb_"<<i <<"\n"<< *(_Lzb[i]);
           norm+=conj(_vect[i]) * _vect[i];
          AddElements((cVec*) Psi, (cVec*) _Lzb[i], _vect[i]);
       }
       cout << "lsbnorm  " << cabs(norm)<< endl;
    }

    
    WaveFunction * OSIL::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       return sourcePsi;
    }

    WaveFunction * OSIL::Apply(WaveFunction * Psi)
    {
       int it;
       
       *(_Lzb[0]) = Psi;
       _hamilton->Apply(buf0,  _Lzb[0]);           /* H*q_0 */
       _alpha[0] = (*(_Lzb[0]) * buf0).real();     /* a_0 = <q_0 | H | _q0 > */
       cout << "a_0 = "<<_alpha[0] << endl;
       *(_Lzb[1]) = buf0;
       MultElementsCopy( (cVec*) buf1, (cVec*) _Lzb[0], _alpha[0]);
       *(_Lzb[1]) -= buf1;
       
       _beta[0] = sqrt(_Lzb[1]->Norm());
//        if ( fabs(_beta[0]) > SIL_CUTOFF_BETA) {// || fabs(_alpha[0]) < SIL_CUTOFF_ALPHA ) {
//           PropaLCB(Psi, 1);
//           return Psi;
//        }
          
       *(_Lzb[1]) *= 1/_beta[0];       /* b0 = Norm(q1) = <q1 | H | q0 > */
       
       it = 2;
       while(it < _order && it < SIL_MAX_ORDER)
       {
          /* buf0 = H * q_i-1 */
          _hamilton->Apply(_Lzb[it], _Lzb[it-1]);
          _alpha[it-1] = (*(_Lzb[it-1]) * _Lzb[it]).real();
          /* - a_i-1 * q_i-1 */
          MultElementsCopy( (cVec*) buf0, (cVec*) _Lzb[it-1], _alpha[it-1]);
          *(_Lzb[it]) -= buf0;
          /* - b_i-2 * q_i-2 */
          MultElementsCopy( (cVec*) buf0, (cVec*) _Lzb[it-2], _beta[it-2]);
          *(_Lzb[it]) -= buf0;
          
          _beta[it-1] = sqrt(_Lzb[it]->Norm());
          
          *(_Lzb[it]) *= 1/_beta[it-1];
          
//           if ( fabs(_beta[it-1]) > SIL_CUTOFF_BETA)// || fabs(_alpha[it-1]) < SIL_CUTOFF_ALPHA )
//           {
//              _alpha[it] = _hamilton->Expec(_Lzb[it]);
//              PropaLCB(Psi, it+1);
//              return Psi;
//           }
          it++;
       }
       it--;
       _alpha[it] = _hamilton->Expec(_Lzb[it]);
       PropaLCB(Psi, it+1);
       
       return Psi;
    }

    Operator * OSIL::operator =(Operator * O)
    {
       Copy(O);
       return this;
    }

    Operator * OSIL::Copy(Operator * O)
    {
       OSIL *o = dynamic_cast<OSIL*>(O);
       
       if ( o == NULL )
          throw (EIncompatible ("OSIL: unable to copy: ", O->Name()));
       
       OPropagator::Copy(O);
       
       _hamilton = o->_hamilton->NewInstance();
       _hamilton->Copy(o->_hamilton);
       
       return this;
    }

    Operator * QDLIB::OSIL::operator *(Operator * O)
    {
       throw (EIncompatible ("OSIL: Apply to operator not implement: "));
       return NULL;
    }
    
    bool QDLIB::OSIL::Valid(WaveFunction * Psi)
    {
       if (_hamilton == NULL) return false;
       return _hamilton->Valid(Psi);
    }

    ParamContainer & QDLIB::OSIL::TellNeeds()
    {
       return _needs;
    }

    void QDLIB::OSIL::AddNeeds(string & Key, Operator * O)
    {
       if (Key == "hamiltonian"){
          _hamilton = O;
          if (O == NULL)
             throw ( EParamProblem("SIL: Invalid hamiltonian") );
       } else 
          throw ( EParamProblem("SIL: Unknown operator key") );
    }
}










