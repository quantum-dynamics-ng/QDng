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
      int n;
   
      _params = params;
      _params.GetValue("dims", n);
   
      if (n < 1)
         throw(EParamProblem("Kick operator needs at least one dimension"));
   
      GridSystem::Dim(n);
   
      char c[256];
   
      string s;
   
      for (int i = 0; i < n; i++) {
         sprintf(c, "k%d", i);
         s = c;
   
         if (_params.isPresent(s)) 
            _params.GetValue(string(c), _kn[i]);
         else _kn[i] = 0; /* Mark as 0 => don't build k-space */
      }
   
   }
   
   void OGridKick::Init(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;

      opPsi = dynamic_cast<WFGridSystem*> (Psi);
      if (opPsi == NULL)
         throw(EIncompatible("Psi is not of type WFGridSystem", Psi->Name()));
      
      /* re-Init exponential ?*/
      if (*this != *((GridSystem*) opPsi) || _exp== NULL) {
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
	       x1.newsize( GridSystem::DimSizes(n) );
	       view.ActiveDim(n);
	       /* init spatial coordinate*/
	       for (int i=0; i < GridSystem::DimSizes(n); i++){
		  x1[i] = GridSystem::Xmin(n) + double(i)*GridSystem::Dx(n);
		  x1[i] *= _kn[n];
	       }
            view += x1;
	    }	 
         }
         ExpElements(_exp, &exp, 1*I);
      }
   }
   
   
   dcomplex OGridKick::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *opPsi;

      opPsi = PsiBra->NewInstance();
      Apply(opPsi, PsiKet);
      c = *PsiBra * opPsi;
      delete opPsi;

      return c;
   }
         
   double OGridKick::Expec(WaveFunction * Psi)
   {
      dcomplex c;

      c = MatrixElement(Psi, Psi);

      return c.real();
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
   
   WaveFunction * OGridKick::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      MultElementsCopy((cVec*) destPsi, (cVec*) sourcePsi, _exp, scaling);
      return destPsi;
   }

   WaveFunction * OGridKick::Apply(WaveFunction * Psi)
   {
      MultElements( (cVec*) Psi, _exp, scaling);
      return Psi;
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
   
   Operator * OGridKick::operator *(Operator * O)
   {
      throw(EIncompatible("OGridKick apply to operator not implementet", O->Name()));
   }
}

















