#include "OGridNablaSq.h"
#include "WFGridSystem.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridNablaSq)
   
   OGridNablaSq::OGridNablaSq() :
      ODSpace(), _name("OGridNablaSq")
   {
      for (int i = 0; i < MAX_DIMS; i++) {
         _mass[i] = -1;
      }
   }

   OGridNablaSq::~OGridNablaSq()
   {
      if (_dspace != NULL)
         delete _dspace;
   }

   void OGridNablaSq::Init(ParamContainer & params)
   {
      int n;

      _params = params;
      _params.GetValue("dims", n);

      if (n < 1)
         throw(EParamProblem("Nabla operator needs at least one dimension"));
      GridSystem::Dim(n);

      char c[256];
      string s;
      bool eff = false;  /* Indicate at least 1 non-zero dim */
      
      for (int i = 0; i < n; i++) {
         sprintf(c, "mass%d", i);
         s = c;
         if (_params.isPresent(s)) {
            _params.GetValue(string(c), _mass[i]);
            if (_mass[i] == 0)
               throw(EParamProblem("Zero mass defined"));
            
            eff = true;
         } else _mass[i] = -1; /* Mark as -1 => don't build k-space */
      }
      
      if (!eff)
         throw(EParamProblem("Nabla^2 Operator is empty (no masses defined)"));
   }

   void OGridNablaSq::Init(WaveFunction *Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*> (Psi);
      if (opPsi == NULL)
         throw(EIncompatible("Psi is not of type WFGridSystem", Psi->Name()));

      /* re-Init k-space ?*/
      if (*this != *((GridSystem*) opPsi) || _dspace == NULL) {
         *((GridSystem*) this) = *((GridSystem*) opPsi);
         InitDspace();
      }

   }

   void OGridNablaSq::UpdateTime()
   {/* No need for */
   }

   dcomplex OGridNablaSq::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *opPsi;

      opPsi = PsiBra->NewInstance();
      Apply(opPsi, PsiKet);
      c = *PsiBra * opPsi;
      DELETE_WF(opPsi);

      return c;
   }

   double OGridNablaSq::Expec(WaveFunction * Psi)
   {
      dcomplex c;

      c = MatrixElement(Psi, Psi);

      return c.real();
   }

   dcomplex OGridNablaSq::Emax()
   {
      if (GridSystem::Dim() == 0)
         throw(EParamProblem("Nabla operator not initalized"));
      /* Calc Tmax on the Grid */
      double T = 0;
      for (int i = 0; i < GridSystem::Dim(); i++)
         if (_mass[i] > 0)
            T += 1 / (_mass[i] * GridSystem::Dx(i) * GridSystem::Dx(i));

      T *= (M_PI * M_PI / 2);
      return dcomplex(T);
   }

   dcomplex OGridNablaSq::Emin()
   {
      return dcomplex(0); /* Minimum kinetic energy is zero */
   }

   void OGridNablaSq::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      WFGridSystem *ket, *opPsi;

      ket = dynamic_cast<WFGridSystem*> (sourcePsi);
      opPsi = dynamic_cast<WFGridSystem*> (destPsi);

      _FFT.Forward(ket);
      opPsi->IsKspace(true); /* The result is in K-space */
      MultElements((cVec*) opPsi, (cVec*) ket, _dspace, 1 / double(GridSystem::Size()));
      ket->IsKspace(false); /* switch back to X-space -> we don't change sourcePsi */
      _FFT.Backward(opPsi);
   }

   void OGridNablaSq::Apply(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*> (Psi);

      _FFT.Forward(opPsi);
      MultElements((cVec*) opPsi, _dspace, 1 / double(GridSystem::Size()));
      _FFT.Backward(opPsi);
   }

   Operator * OGridNablaSq::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator * QDLIB::OGridNablaSq::Copy(Operator * O)
   {
      OGridNablaSq *org = dynamic_cast<OGridNablaSq*> (O);

      if (org == NULL)
         throw(EIncompatible("O is not of type OGridNablaSq", O->Name()));

      _params = org->_params;
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) org);

      if (_dspace == NULL && org->_dspace != NULL) {
         _dspace = new dVec();
         *_dspace = *(org->_dspace);
      }

      for (int i = 0; i < GridSystem::Dim(); i++) {
         _mass[i] = org->_mass[i];
      }

      return this;
   }

   bool QDLIB::OGridNablaSq::Valid(WaveFunction * Psi)
   {
      WFGridSystem *PsiGS;

      PsiGS = dynamic_cast<WFGridSystem*> (Psi);
      if (PsiGS == NULL)
         return false;

      if (*((GridSystem*) this) != *((GridSystem*) PsiGS))
         return false;

      return true;
   }

   /**
    * \todo clean Vector view.
    * \bug dimensions are inverse => x (lowest number ) is the fastest!
    */
   void OGridNablaSq::InitDspace()
   {

      if (GridSystem::Dim() == 0)
         throw(EParamProblem("Missing GridSystem parameters"));

      dVec *kspace1;

      if (_dspace == NULL)
         _dspace = new dVec(GridSystem::Size(), true);
      else _dspace->newsize(GridSystem::Size());

      *_dspace = 0;
      dVecView view(*_dspace, GridSystem::Dim(), GridSystem::DimSizes());

      /* Init k-space for every dimension */
      for (int i = 0; i < GridSystem::Dim(); i++) {
         if (_mass[i] > 0) {

            kspace1 = Kspace::Init1Dd2dx2(_mass[i], static_cast<GridSystem>(*this), i);

            view.ActiveDim(i);
            view += *kspace1;
            delete kspace1;

         }
      }
   }

   void OGridNablaSq::InitExponential(cVec *exp, dcomplex c)
   {
      if (_dspace == NULL) InitDspace();
      ExpElements(exp, _dspace, c);
   }
} /* namespace QDLIB */

