#include "OGridKick.h"
#include "WFGridSystem.h"

namespace QDLIB {

   OGridKick::OGridKick()
   : Operator(), GridSystem(), _name("OGridKick"), _exp(NULL)
   {
   }
   
   
   OGridKick::~OGridKick()
   {
      if (_exp != NULL) delete _exp;
   }

   Operator * OGridKick::NewInstance()
   {
      OGridKick *r;
   
      r = new OGridKick();

      return r;
   }

   const string & QDLIB::OGridKick::Name()
   {
      return _name;
   }
   
   void OGridKick::Init(ParamContainer & params)
   {
      _params = params;

      if ( _params.isPresent("dims") ) {
         int n;
         _params.GetValue("dims", n);
         if (n < 1)
            throw(EParamProblem("Kick operator needs at least one dimension"));

         GridSystem::Dim(n);
      }
   
   }
   
   void OGridKick::Init(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*> (Psi);
      if (opPsi == NULL)
         throw(EIncompatible("Psi is not of type WFGridSystem", Psi->Name()));
      
      if (_exp != NULL) return;  // Avoid init twice
      
      if ( GridSystem::Dim() == 0){ /* Take Dims from WF*/
         Dim(opPsi->Dim());
      } else if ( GridSystem::Dim() !=  opPsi->Dim() ) { /* dims has been given in input (check it)*/
         throw ( EIncompatible("OGridNablaSq: Number of dims doesn't match WF") );
      }
      
      double scale=1;
      if (_params.isPresent("scale"))
         _params.GetValue("scale", scale);

      /* Get k-values from config */
      char c[256];

      for (int i = 0; i < Dim(); i++) {
         sprintf(c, "k%d", i);

         if (_params.isPresent(c))
            _params.GetValue(string(c), _kn[i]);
         else _kn[i] = 0; /* Mark as 0 => don't build k-space */

         _kn[i] *= scale;
      }
      
      *((GridSystem*) this) = *((GridSystem*) opPsi);
      
      
      dVec x1;
      dVec exp;

      if (_exp == NULL)
         _exp = new cVec(GridSystem::Size(), true);
      else _exp->newsize(GridSystem::Size());
      
      exp.newsize(GridSystem::Size());
      exp = 0;
      dVecView view(exp, GridSystem::Dim(), GridSystem::DimSizes());
      
      /* Run over dimension - create exp(i * k_n * x_n) */
      for (int n=0; n < GridSystem::Dim(); n++){
         if (_kn[n] != 0){
            x1.newsize( GridSystem::DimSize(n) );
            view.ActiveDim(n);
            /* init spatial coordinate*/
            for (int i=0; i < GridSystem::DimSize(n); i++){
               x1[i] = GridSystem::Xmin(n) + double(i)*GridSystem::Dx(n);
               x1[i] *= _kn[n];
            }
         view += x1;
         }	 
      }
      ExpElements(_exp, &exp, 1*I);
      
   }

   Operator * OGridKick::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }

   Operator * OGridKick::Copy(Operator * O)
   {
      OGridKick *org = dynamic_cast<OGridKick*> (O);

      if (org == NULL)
         throw(EIncompatible("O is not of type OGridKick", O->Name()));

      _params = org->_params;
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) org);

      if (_exp == NULL && org->_exp != NULL) {
         _exp = new cVec();
         *_exp = *(org->_exp);
      }

      for (int i = 0; i < GridSystem::Dim(); i++) {
         _kn[i] = org->_kn[i];
      }

      return this;
   }
   
   void OGridKick::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      MultElementsCopy((cVec*) destPsi, (cVec*) sourcePsi, _exp);
   }

   void OGridKick::Apply(WaveFunction * Psi)
   {
      MultElements( (cVec*) Psi, _exp);
   }
   
   bool OGridKick::Valid(WaveFunction * Psi)
   {
      WFGridSystem *PsiGS;

      PsiGS = dynamic_cast<WFGridSystem*> (Psi);
      if (PsiGS == NULL)
         return false;

      if (*((GridSystem*) this) != *((GridSystem*) PsiGS))
         return false;

      return true;
   }   
}

















