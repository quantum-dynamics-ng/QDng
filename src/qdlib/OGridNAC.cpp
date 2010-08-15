#include "OGridNAC.h"

namespace QDLIB {

   OGridNAC::OGridNAC(): OGridNabla(), _name("GridNAC"), _sign(1), _mass(1), _buf(NULL)
   {
      _pNAMCE = (dVec*) &_NACME;
   }
   
   
   OGridNAC::~OGridNAC()
   {
      if (_buf != NULL) delete _buf;
   }
   
   
   Operator * OGridNAC::NewInstance()
   {
      OGridNAC *r = new OGridNAC();
      return r;
   }

   void OGridNAC::Init(ParamContainer & params)
   {
      /* Init parent */
      OGridNabla::Init(params);
      
      if (!_params.isPresent("file"))
         throw (EParamProblem("NACME file is missing"));
      
      /* Init Grid with NAC file */
      ParamContainer pNAC;
      string s;
      _params.GetValue("file",s);
      pNAC.SetValue("name", "OGridPotential");
      pNAC.SetValue("file", s);
      
      _NACME.Init(pNAC);
      
      if (_params.isPresent("sign"))
         _params.GetValue("sign",_sign);
      
      if (_params.isPresent("mass"))
         _params.GetValue("mass",_mass);
   }

   
   void OGridNAC::Init(WaveFunction * Psi)
   {
      OGridNabla::Init(Psi);
      _NACME.Init(Psi);
      
      _buf = Psi->NewInstance();
   }

   const string & OGridNAC::Name()
   {
      return _name;
   }

   dcomplex OGridNAC::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c(0,0);
      
      WaveFunction *opPsi;
      
      opPsi = PsiKet->NewInstance();
      Apply(opPsi, PsiKet);
      
      c = *PsiBra * opPsi;
      
      delete opPsi;
      return c;
   }

   double OGridNAC::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      c = MatrixElement(Psi, Psi);
     
      return c.real();
   }

   void OGridNAC::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      /* 1/2 f del psi*/
      PreFactor(0.5 * _sign / _mass);
      OGridNabla::Apply(_buf, sourcePsi);
      _NACME.Apply(_buf);
      
      /* 1/2 del f psi*/
      PreFactor(0.5 * _sign / _mass);
      _NACME.Apply(destPsi, sourcePsi);
      OGridNabla::Apply(destPsi);

      *destPsi += _buf;
   }

   void OGridNAC::Apply(WaveFunction * Psi)
   {
      /* 1/2 f del psi*/
      PreFactor(0.5 * _sign / _mass);
      OGridNabla::Apply(_buf, Psi);
     _NACME.Apply(_buf);
      
      /* 1/2 del f psi*/
      PreFactor(.5 * _sign / _mass);
      _NACME.Apply(Psi);
      OGridNabla::Apply(Psi);

      *Psi += _buf;
   }

   Operator * OGridNAC::operator =(Operator * O)
   {
      return Copy(O);
   }

   Operator * OGridNAC::Copy(Operator * O)
   {
      OGridNAC* o = dynamic_cast<OGridNAC*>(O);
      
      if(o == NULL)
         throw (EIncompatible("Operator not of type OGridNAC", O->Name()) );
      
      OGridNabla::Copy(O); /* Copy Parent */
      
      _NACME = &o->_NACME;
      _pNAMCE = (dVec*) &_NACME;
      _sign = _sign;
      
      _buf = o->_buf->NewInstance();
      return this;
   }
}





