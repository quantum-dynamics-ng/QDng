#include "OGridPosition.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGridPosition::OGridPosition()
   : OGridSystem(), _name("OGridPosition")
   {
   }
   
   
   OGridPosition::~OGridPosition()
   {
   }


   Operator * OGridPosition::NewInstance()
   {
      OGridPosition *r;
      
      r = new OGridPosition();
      
      return r;
   }



   void OGridPosition::Init(WaveFunction * Psi)
   {
      WFGridSystem *wf;
      dVec xspace1; /* Single dimension k-space */
      
      wf = dynamic_cast<WFGridSystem*>(Psi);
      
      if (wf == NULL)
         throw ( EIncompatible("OGridPosition needs a gridsystem wavefunction: ", Psi->Name()) );
      
      /* Copy grid parameters */
      *((GridSystem*) this) = *((GridSystem*) wf);
      dVec::newsize(GridSystem::Size());
      
      *((dVec*) this) = 0; /* Init with zeros */
      
      /* Init k-space for every dimension */
      dVecView view(*this, GridSystem::Dim(), GridSystem::DimSizes());
      for (int i=0; i < GridSystem::Dim(); i++){
         xspace1.newsize(GridSystem::DimSizes(i));
         for (int n=0; n < GridSystem::DimSizes(i); n++){
              xspace1[n] = GridSystem::Xmin(i) + GridSystem::Dx(i) * n;
         }
         
         view.ActiveDim(i);
         view += xspace1;
      }
   }
   
   dcomplex OGridPosition::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *OpPsi = PsiKet->NewInstance();
      
      Apply(OpPsi, PsiKet);
      
      c = *PsiBra * OpPsi;
      
      delete OpPsi;
      
      return c;
   }

   double OGridPosition::Expec(WaveFunction * Psi)
   {
      dcomplex c;
      
      c = MatrixElement(Psi, Psi);
      
      return c.real();
   }


   WaveFunction* OGridPosition::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      MultElementsCopy( (cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);
      
      return destPsi;
   }

   WaveFunction* OGridPosition::Apply(WaveFunction * Psi)
   {
      MultElements( (cVec*) Psi, (dVec*) this);
      
      return Psi;
   }
   
   Operator * OGridPosition::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator * OGridPosition::Copy(Operator * O)
   {
      OGridSystem::Copy(O);
      return this;
   }

   Operator * OGridPosition::operator *(Operator * O)
   {
      throw ( EIncompatible("OGridPosition can't operate on other operators"));
      return this;
   }

} /* namespace QDLIB */





