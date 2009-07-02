#include "OGridNablaSq.h"
#include "WFGridSystem.h"


namespace QDLIB {

   OGridNablaSq::OGridNablaSq()
   : ODSpace(), _name("OGridNablaSq")
   {
      for (int i=0; i < MAX_DIMS; i++){
	 _mass[i] = -1;
      }
   }
   
   
   OGridNablaSq::~OGridNablaSq()
   {
   }

   Operator * OGridNablaSq::NewInstance()
   {
      OGridNablaSq *r;
   
      r = new OGridNablaSq();
   
      return r;
   }

   void OGridNablaSq::Init(ParamContainer & params)
   {
      int n;

      
      _params = params;
      _params.GetValue( "dims", n);
   
      if (n < 1)
	 throw ( EParamProblem ("Nabla operator needs at least one dimension") );
      GridSystem::Dim(n);
      
      char c[256];
      string s;
   
      for (int i=0; i < n; i++){
	 sprintf (c, "mass%d", i);
	 s=c;
	 if ( _params.isPresent(s) ) {
	    _params.GetValue( string(c), _mass[i]);
	    if (_mass[i] == 0) throw ( EParamProblem ("Zero mass defined") );
	 } else
	    _mass[i] = -1;    /* Mark as -1 => don't build k-space */
      }
   }

   void OGridNablaSq::Init(WaveFunction *Psi)
   {
      WFGridSystem *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(Psi);
      if (opPsi == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      /* re-Init k-space ?*/
      if (*this != *((GridSystem*) opPsi)  || _dspace == NULL) {
	 *((GridSystem*) this) = *((GridSystem*) opPsi);
	 InitDspace();
      }
     
   }
   
   const string & OGridNablaSq::Name()
   {
      return _name;
   }

   void OGridNablaSq::UpdateTime(){/* No need for */}

   dcomplex OGridNablaSq::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *opPsi;
      
      opPsi = PsiBra->NewInstance();
      Apply(opPsi, PsiKet);
      c = *PsiBra * opPsi;
      delete opPsi;
      
      return c;
   }

   double OGridNablaSq::Expec(WaveFunction * Psi)
   {
      dcomplex c;
        
      c = MatrixElement(Psi, Psi);
      
      return c.real();
   }

   double OGridNablaSq::Emax()
   {
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Nabla operator not initalized") );
      /* Calc Tmax on the Grid */
      double T=0;
      for (int i=0; i < GridSystem::Dim(); i++)
	 if (_mass[i] > 0) 
	    T += 1/ ( _mass[i] *  GridSystem::Dx(i) * GridSystem::Dx(i));
      
      T *= ( M_PI*M_PI / 2 );
      return T;
   }
	 
   double OGridNablaSq::Emin()
   {
      return 0; /* Minimum kinetic energy is zero */
   }
   
   WaveFunction * OGridNablaSq::Apply(WaveFunction *destPsi, WaveFunction *sourcePsi)
   {
      WFGridSystem *ket, *opPsi;
      
      
      ket = dynamic_cast<WFGridSystem*>(sourcePsi);
      opPsi = dynamic_cast<WFGridSystem*>(destPsi);
      
      ket->ToKspace();
      opPsi->isKspace(true);
      MultElements((cVec*) opPsi, (cVec*) ket, _dspace, 1/double(GridSystem::Size()));
      ket->isKspace(false);   /* switch back to X-space -> we don't change sourcePsi*/
      opPsi->ToXspace();
      
      return destPsi;
   }

   WaveFunction * OGridNablaSq::Apply(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(Psi);

      opPsi->ToKspace();
      MultElements((cVec*) opPsi, _dspace, 1/double(GridSystem::Size()));
      opPsi->ToXspace();
      
      return opPsi;
   }

   
   Operator * OGridNablaSq::operator =(Operator * O)
   {
      Copy(O);
      return this;
   }
   
   Operator * QDLIB::OGridNablaSq::Copy(Operator * O)
   {
      OGridNablaSq *org = dynamic_cast<OGridNablaSq*>(O);
      
      if (org == NULL)
	 throw ( EIncompatible("O is not of type OGridNablaSq", O->Name()) );

      _params = org->_params;
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) org);
      
      if (_dspace == NULL && org->_dspace != NULL){
	 _dspace = new dVec();
	 *_dspace = *(org->_dspace);
      }
         
      
      for (int i=0; i < GridSystem::Dim(); i++){
	 _mass[i] = org->_mass[i];
      }
      
      return this;
   }

   Operator * OGridNablaSq::operator *(Operator * O)
   {
      throw ( EIncompatible("Can't apply OGridNablaSq to another operator") );
      return O;
   }

   /**
    * \todo clean Vector view.
    * \bug dimensions are inverse => x (lowest number ) is the fastest!
    */
   void OGridNablaSq::InitDspace()
   {
   
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Missing GridSystem parameters") );
   
      dVec *kspace1;
   
      if (_dspace == NULL ) _dspace = new dVec(GridSystem::Size(), true);
      else _dspace->newsize(GridSystem::Size());
      
      
      *_dspace = 0;
      dVecView view(*_dspace, GridSystem::Dim(), GridSystem::DimSizes());
   
      /* Init k-space for every dimension */
      for (int i=0; i < GridSystem::Dim(); i++){ 
	 if (_mass[i] > 0) {

	    kspace1 = Kspace::Init1Dd2dx2(_mass[i], GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i) );

	    view.ActiveDim(i);
	    view += *kspace1;
	    delete kspace1;

	 }
      }
   }

} /* namespace QDLIB */

