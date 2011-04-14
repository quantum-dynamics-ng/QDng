#include "OProduct.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OProduct)
         
    OProduct::OProduct()
            : OList(), _name("OProduct")
    {
    }


    OProduct::~OProduct()
    {
    }

    dcomplex OProduct::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
    {
       WaveFunction  *out;
       dcomplex d;
     
       out = PsiKet->NewInstance();
       Apply(out, PsiKet);
       d = *PsiBra * out;
       DELETE_WF(out);
      
       return d;
    }

    dcomplex OProduct::Emax()
    {
       dcomplex val(1);
       
       for (int i=Size()-1; i >= 0; i--)
          val *= Get(i)->Emax();
       
       return val;
    }

    dcomplex OProduct::Emin()
    {
       dcomplex val(1);
       
       for (int i=Size()-1; i >= 0; i--)
          val *= Get(i)->Emin();
       
       return val;
    }

    void OProduct::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       Get(Size()-1)->Apply(destPsi, sourcePsi);
       
       for (int i=Size()-2; i >= 0; i--)
          Get(i)->Apply(destPsi);

    }

    void OProduct::Apply(WaveFunction * Psi)
    {
       for (int i=Size()-1; i >= 0; i--)
          Get(i)->Apply(Psi);
    }

    void QDLIB::OProduct::ApplyParent(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
       Get(Size()-1)->ApplyParent(destPsi, sourcePsi);
       
       for (int i=Size()-2; i >= 0; i--)
          Get(i)->ApplyParent(destPsi, destPsi);

    }
    
    void OProduct::Apply(Operator * destOp, Operator * sourceOp)
    {
       Get(Size()-1)->Apply(destOp, sourceOp);
       
       for (int i=Size()-2; i >= 0; i--)
          Get(i)->Apply(destOp);
    }

    void OProduct::Apply(Operator * Op)
    {
       for (int i=Size()-1; i >= 0; i--)
          Get(i)->Apply(Op);
    }

    
}

