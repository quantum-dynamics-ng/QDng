#include "OGridStokes.h"

namespace QDLIB {
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridStokes)
   
    OGridStokes::OGridStokes(): _name("OGridStokes")
    {
    }


    OGridStokes::~OGridStokes()
    {
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

       params.GetValue("mass", _mass);
       params.GetValue("eta", _eta);
       params.GetValue("R", _R);
       
       /* Let our parents get their parameters */
       OGridNabla::Init(params);
       OGridPosition::Init(params);
    }
    
    void OGridStokes::Init(WaveFunction *Psi)
    {
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
       OGridPosition::Apply(destPsi);
       OGridNabla::Apply(destPsi,sourcePsi);
       
       *destPsi *= _eta / _mass * _R * 6 * M_PI;
    }
    
    void OGridStokes::Apply(WaveFunction *Psi)
    {
       OGridPosition::Apply(Psi);
       OGridNabla::Apply(Psi);
       
       *Psi *= _eta / _mass * _R * 6 * M_PI;
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
       
       OGridNabla::Copy(O);
       OGridPosition::Copy(O);
       
       return this;
    }
    
    bool OGridStokes::Valid(WaveFunction *Psi)
    {
       return OGridNabla::Valid(Psi) & OGridPosition::Valid(Psi);
    }

}
