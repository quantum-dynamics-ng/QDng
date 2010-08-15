#include "OScalar.h"

namespace QDLIB {

   OScalar::OScalar()
   : Operator(), _name("Scalar"), _buf(NULL), _value(1)
   {
   }
   
   
   OScalar::~OScalar()
   {
      if (_buf != NULL) delete _buf;
   }
   
   
   Operator * OScalar::NewInstance()
   {
      OScalar* r;
      r = new OScalar();
      return r;
   }

   void OScalar::Init(ParamContainer & params)
   {
      _params = params;
      
      _params.GetValue("value",_value);
   }
   
   void OScalar::Init(WaveFunction * Psi)
   {
      if (Psi == NULL)
         throw( EIncompatible("Scalar got empty wavefunction") );
      
      _buf = Psi->NewInstance();
   }

   dcomplex OScalar::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      *_buf = PsiKet;
      MultElements((cVec*) _buf, _value);
      return *PsiBra * _buf;
   }
   
   
   double OScalar::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      c = MatrixElement(Psi, Psi);
      return c.real();
   }
   
   void OScalar::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      MultElementsCopy((cVec*) destPsi, (cVec*) sourcePsi, _value);
   }

   
   void OScalar::Apply(WaveFunction * Psi)
   {
      MultElements((cVec*) Psi, _value);
   }

   Operator * OScalar::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }
   
   Operator * OScalar::Copy(Operator * O)
   {
      OScalar* o;
      
      o = dynamic_cast<OScalar*>(O);
      if (o == NULL)
         throw ( EIncompatible("Not of type scalar", O->Name() ) );
      
      _value = o->_value;
      if (o->_buf != NULL)
         _buf = o->_buf->NewInstance();
      
      return this;
   }
   
   bool OScalar::Valid(WaveFunction * Psi)
   {
      if ( Psi == NULL ) return false;
      if ( Psi->size() != _buf->size() ) return false;
      
      return true;
   }
   
} /* namespace QDLIB */


















