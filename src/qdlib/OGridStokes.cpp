#include "OGridStokes.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridStokes)
   
    OGridStokes::OGridStokes() : OGridNabla(true),
       _name("OGridStokes"), _buf(NULL), _mass(0), _eta(0), _R(0), _c(1)
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
       
       /* Let our parents get their parameters */
       OGridNabla::Init(params);
       OGridPosition::Init(params);
    }
    
    void OGridStokes::Init(WaveFunction *Psi)
    {
       if (_buf != NULL) return;

       _buf = Psi->NewInstance();
       OGridNabla::Init(Psi);
       OGridPosition::Init(Psi);
    }
    
    
    dcomplex OGridStokes::Emax()
    {
       /* 6 pi eta R kmax / m * xmax */
       return dcomplex( 6.0 * M_PI * _eta * _R * OGridNabla::Emax() * OGridPosition::Emax() /_mass);
    }
    
    void OGridStokes::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       
       /* 1/2 {x,p} */
       OGridNabla::Apply(_buf ,sourcePsi);
       double p = (*_buf * sourcePsi).real();
       OGridPosition::Apply(_buf);
       OGridPosition::Apply(destPsi ,sourcePsi);
       double x = (*destPsi * sourcePsi).real();
       OGridNabla::Apply(destPsi);
       
       *destPsi += _buf;
       *destPsi *= 0.5;
       
       OGridPosition::Apply(_buf ,sourcePsi);
       OGridPosition::Apply(_buf);
       *destPsi += _buf;
       
       /* - <x>p */
       OGridNabla::Apply(_buf ,sourcePsi);
       *_buf *= x;
       *destPsi -= _buf;

       *destPsi *= _c;
       
       /* <p>x */
        OGridPosition::Apply(_buf ,sourcePsi);
        *_buf *= p * (1 - _c);
        *destPsi += _buf;

        *_buf = sourcePsi;
        *_buf *= x * p * (1-_c);

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
