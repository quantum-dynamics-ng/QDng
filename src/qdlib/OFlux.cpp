#include "OFlux.h"

namespace QDLIB {
   
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OFlux)
   
   
   OFlux::OFlux()
   : _name("OFlux"), _buf(NULL), _indT(0), _indG(1)
   {
   }
   
   
   OFlux::~OFlux()
   {
      DELETE_WF(_buf);
   }


   
   void OFlux::Init(ParamContainer & params)
   {
      _params = params;
   }

   void OFlux::Init(WaveFunction * Psi)
   {
      OList::Init(Psi);
      
      DELETE_WF(_buf);
      DELETE_WF(_one);
      
      _buf = Psi->NewInstance();
      _one = Psi->NewInstance();
      
      *_one = -2;
      
      if (Size() != 2)
         throw ( EParamProblem("Flux: Misterious number of operators. Should be 2") );
      
   }

   dcomplex OFlux::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      /* Im( Psi* T Psi )*/
      Get(_indT)->Apply(_buf, PsiKet);
      _buf->DirectProductConjugate(PsiBra);
      _buf->Imag();
      
      /* Integrate over inner Volume */
      Get(_indG)->Apply(_buf);
      return dcomplex(*_one * _buf);
   }

   double OFlux::Expec(WaveFunction * Psi)
   {
      return MatrixElement(Psi,Psi).real();
   }

   void OFlux::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      throw ( EIncompatible("Flux Operator apply not implemented") );
   }

   void OFlux::Apply(WaveFunction * Psi)
   {
      throw ( EIncompatible("Flux Operator apply not implemented") );
   }

   Operator * OFlux::operator =(Operator * O)
   {
      return Copy(O);
   }

   Operator * OFlux::Copy(Operator * O)
   {
      OFlux* o;
      
      o = dynamic_cast<OFlux*>(O);
      if (o == NULL)
         throw (EIncompatible("Can't copy to Flux Operator", o->Name()));
      
      OList::Copy(O);
      
      DELETE_WF(_buf);
      if (_buf != NULL) _buf = dynamic_cast<WFGridSystem*>(o->_buf->NewInstance());
      
      _indT = o->_indT;
      _indG = o->_indG;
      
      return this;
   }
}

