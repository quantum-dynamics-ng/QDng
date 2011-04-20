#include "OGridStokes.h"
#include "math/math_functions.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridStokes)
   
    OGridStokes::OGridStokes() : OGridNabla(true),
    _name("OGridStokes"), _buf(NULL), _x0(0), _slope(0), _mass(0), _eta(0), _R(0), _c(0)
    {
    }


    OGridStokes::~OGridStokes()
    {
       DELETE_WF(_buf);
    }

    void OGridStokes::Init(ParamContainer &params)
    {  
       /* Check our own stuff */
       if ( !params.isPresent("mass") )
          throw(EParamProblem("Stokes operators needs a mass"));
       
       if ( !params.isPresent("eta") )
          throw(EParamProblem("Stokes operators needs a stokes parameters"));

       if ( !params.isPresent("R") )
          throw(EParamProblem("Stokes operators needs a stokes radius"));

       if ( params.isPresent("c") ){
          params.GetValue("c", _c);
          if (_c < 0 || _c > 1)
             throw(EParamProblem("c-parameter must be between 0 and 1"));
       }
       
       params.GetValue("mass", _mass);
       params.GetValue("eta", _eta);
       params.GetValue("R", _R);
       
       if ( params.isPresent("x0") )
          params.GetValue("x0", _x0);
       
       if ( params.isPresent("slope") )
          params.GetValue("slope", _slope);

       /* Let our parents get their parameters */
       OGridNabla::Init(params);
       OGridPosition::Init(params);
    }
    
    void OGridStokes::Init(WaveFunction *Psi)
    {
       if (_buf != NULL) return;
       
       OGridNabla::Init(Psi);
       OGridPosition::Init(Psi);
       
       _buf = Psi->NewInstance();
       _psi = Psi->NewInstance();
       
       /* Position dependent scaling */
       _sigmoid.newsize(Psi->size());
       _sigmoidInt.newsize(Psi->size());
       if (_slope != 0) {
         double x0 = ( _x0 - double( OGridPosition::Xmin(0) ) ) / double( OGridPosition::Dx(0) ); /* Convert x0 to gridpoint units */
         FunctionGenerator<dVec>::Sigmoid(_sigmoid, x0, _slope);
         FunctionGenerator<dVec>::SigmoidInt(_sigmoidInt, x0, _slope);
       }
       else
         _sigmoid = 1.0;
    }
    
    
    dcomplex OGridStokes::Emax()
    {
       /* 6 pi eta R kmax / m * xmax */
       return dcomplex( 6.0 * M_PI * _eta * _R * OGridNabla::Emax() * OGridPosition::Emax() /_mass);
    }
    
    void OGridStokes::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       
       /* Apply sigmoid shape */
       MultElementsCopy( (cVec*) _psi, (cVec*) sourcePsi, (dVec*) &_sigmoid);
       //*_psi = sourcePsi;
       /* Recalc expectation values */
       if ( RecalcInternals() ) {
          OGridPosition::Apply(_buf, _psi);
          _xe = (*_buf * _psi).real() / _psi->Norm();
          OGridNabla::Apply(_buf, _psi);
          _pe = (*_buf * _psi).real() / _psi->Norm();
       }
       
       
       /* <p>x */
       MultElementsCopy( (cVec*) destPsi, (cVec*) sourcePsi, (dVec*) &_sigmoidInt);
       //OGridPosition::Apply(destPsi);
       *destPsi *= _pe * (1 - _c);
       
       /* <p><x> */
       *_buf = sourcePsi;
       *_buf *= _xe * _pe * (1 - _c);
       
       *destPsi += _buf;

       
       /* c/2 (xp + px -2<x>p) */
       if ( _c != 0 ) {
         /* 1/2 xp*/
         OGridNabla::Apply(_buf, sourcePsi);
         MultElements( (cVec*) _buf, (dVec*) &_sigmoid);
         OGridPosition::Apply(_buf);
         *_buf *= 0.5 * _c;
         *destPsi += _buf;
         
         /* 1/2 px*/
         OGridPosition::Apply(_buf, _psi);
         OGridNabla::Apply(_buf);
         *_buf *= 0.5 * _c;
         *destPsi += _buf;

         /* - <x>p */
         OGridNabla::Apply(_buf, _psi);
         *_buf *= _xe * _c *.5;
         *destPsi -= _buf;
         
         OGridNabla::Apply(_buf, sourcePsi);
         MultElements( (cVec*) _buf, (dVec*) &_sigmoid);
         *_buf *= _xe * _c *.5;
         *destPsi -= _buf;
       }

       *destPsi *= _eta / _mass * _R * 6 * M_PI;
    }
    
    void OGridStokes::Apply(WaveFunction *Psi)
    {
       WaveFunction *buf;

       buf = Psi->NewInstance();
       *buf = Psi;

       Apply(Psi, buf);
    }
    
    
    Operator* OGridStokes::operator=(Operator* O)
    {
       return Copy(O);
    }
    
    Operator* OGridStokes::Copy(Operator* O)
    {
       OGridStokes *o = dynamic_cast<OGridStokes*>(O);
       
       if (o == NULL )
          throw (EIncompatible("Copy: not of type OGridStokes"));
       
       _mass = o->_mass;
       _eta = o->_eta;
       _R = o->_R;
       _buf = o->_buf->NewInstance();
       
       OGridNabla::Copy(O);
       OGridPosition::Copy(O);
       
       return this;
    }
    
    bool OGridStokes::Valid(WaveFunction *Psi)
    {
       return OGridNabla::Valid(Psi) & OGridPosition::Valid(Psi);
    }

}
