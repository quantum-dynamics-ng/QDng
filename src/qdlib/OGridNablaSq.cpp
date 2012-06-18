#include "OGridNablaSq.h"
#include "WFGridSystem.h"
#include "tools/helpers.h"

namespace QDLIB
{
   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridNablaSq)
   
   OGridNablaSq::OGridNablaSq() :
      ODSpace(), _name("OGridNablaSq"), _mass(MAX_DIMS)
   {
      _mass = -1;
      Diff::Collective(true);
      Diff::Single(false);
      Diff::Mixed(false);
      Diff::Derivative(2);
      Diff::Method(FFT);
   }

   OGridNablaSq::~OGridNablaSq()
   {
   }

   void OGridNablaSq::Init(ParamContainer & params)
   {
      _params = params;

      /* Get masses */
      vector<double> vec;
      _params.GetArray("mass", vec);
      ConvertArray(vec, _mass);

      if (vec.size() < 1)
         throw(EParamProblem("Nabla operator needs at least one dimension"));

      GridSystem::Dim(vec.size());

      /* Check if mass definitions make sense */
      bool eff = false;  /* Indicate at least 1 non-zero dim */

      dVec masses(vec.size());
      for (int i = 0; i < GridSystem::Dim(); i++) {
         if (_mass[i] == 0)
            throw(EParamProblem("Zero mass defined"));
         if (_mass[i] > 0) eff = true;

         masses[i] = -1./2./_mass[i];
      }

      if (!eff)
         throw(EParamProblem("Nabla^2 Operator is empty (no masses defined)"));

      Diff::Factors(masses);
   }

   void OGridNablaSq::Init(WaveFunction *Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*> (Psi);
      if (opPsi == NULL)
         throw(EIncompatible("Psi is not of type WFGridSystem", Psi->Name()));

      
      /* re-Init k-space ?*/
      if (_dspace != NULL) return;  // Avoid init twice

      if ( GridSystem::Dim() == 0){ /* Take Dims from WF*/
         GridSystem::Dim(opPsi->Dim());
      } else if ( GridSystem::Dim() !=  opPsi->Dim() ) { /* dims has been given in input (check it)*/
         throw ( EIncompatible("OGridNablaSq: Number of dims doesn't match WF") );
      }

      *((GridSystem*) this) = *((GridSystem*) opPsi);

      Diff::SetGrid(*this);
      Diff::InitParams(_params);
      InitDspace();
   }

   void OGridNablaSq::UpdateTime()
   {/* No need for */
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
      DnDxn(destPsi, sourcePsi);
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

   void OGridNablaSq::InitDspace()
   {

      if (GridSystem::Dim() == 0)
         throw(EParamProblem("Missing GridSystem parameters"));

      _InitKspace(); /* Maintained by Diff*/
      _dspace = &_kspace; /* hosted in Diff class */
   }

   void OGridNablaSq::InitExponential(cVec *exp, dcomplex c)
   {
      if (_dspace == NULL) InitDspace();
      ExpElements(exp, _dspace, c);
   }
} /* namespace QDLIB */

