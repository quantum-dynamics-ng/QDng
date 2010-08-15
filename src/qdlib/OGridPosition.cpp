#include "OGridPosition.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGridPosition::OGridPosition()
   : OGridSystem(), _name("OGridPosition"), _dim(-1)
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

   void OGridPosition::Init(ParamContainer & params)
   {
      _params = params;
      
      if (_params.isPresent("dim")){
         _params.GetValue("dim", _dim);
         
         if (_dim < 0)
            throw ( EParamProblem("Invalid dimension chosen for position operator: ", _dim)  );
      }
   }

   /** Init a single dimension */
   void OGridPosition::_InitDim(dVecView &view, const int dim)
   {
      dVec xspace1;
      
      xspace1.newsize(GridSystem::DimSizes(dim));
      
      /* setup x */
      for (int n=0; n < GridSystem::DimSizes(dim); n++){
         xspace1[n] = GridSystem::Xmin(dim) + GridSystem::Dx(dim) * n;
      }
         
      view.ActiveDim(dim);
      view += xspace1;
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
      
      if (GridSystem::Dim() < _dim + 1)
         throw (EParamProblem ("Dimension for position operator exceeds dimensions of Wavefunction", _dim) );
      
      dVecView view(*this, GridSystem::Dim(), GridSystem::DimSizes());
      
      if (_dim < 0 ) { /* Init k-space for every dimension */
         for (int i=0; i < GridSystem::Dim(); i++)
            _InitDim(view, i);
      } else {
         _InitDim(view, _dim);
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

   dcomplex OGridPosition::Emax()
   {
      return(dcomplex(GridSystem::Xmax(_dim)));
   }
   
   dcomplex OGridPosition::Emin()
   {
      return(dcomplex(GridSystem::Xmin(_dim)));
   }

   
   void OGridPosition::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      MultElementsCopy( (cVec*) destPsi, (cVec*) sourcePsi, (dVec*) this);
   }

   void OGridPosition::Apply(WaveFunction * Psi)
   {
      MultElements( (cVec*) Psi, (dVec*) this);
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
} /* namespace QDLIB */

