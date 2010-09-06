#include "OFlux.h"

namespace QDLIB {
   
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OFlux)
   
   
   OFlux::OFlux()
         : OGridNabla(), _name("OFlux"), _mass(0), _buf(NULL),
   {
      for (int i=0; i < MAX_DIMS; i++)
         _Gm[i] = NULL;
   }
   
   
   OFlux::~OFlux()
   {
      DELETE_WF(_buf);
   }


   
   void OFlux::Init(ParamContainer & params)
   {
      OGridNabla::Init(params);
   }

   void OFlux::Init(WaveFunction * Psi)
   {
      OGridNabla::Init(Psi);
         
      DELETE_WF(_buf);
      _buf = Psi->NewInstance();
   }

   const string & OFlux::Name()
   {
      return _name;
   }

   dcomplex OFlux::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      
   }

   double OFlux::Expec(WaveFunction * Psi)
   {
   }

   void OFlux::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      OGridNabla::Apply(_buf, sourcePsi);
      
      MultElementsConugate((cVec*) destPsi, (cVec*) sourcePsi, (cVec*)_buf, 1.0);
      
      destPsi->Imag();
      
      if 
   }

   void OFlux::Apply(WaveFunction * Psi)
   {
   }

   Operator * OFlux::operator =(Operator * O)
   {
   }

   Operator * OFlux::Copy(Operator * O)
   {
   }
}

