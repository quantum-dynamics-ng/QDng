#include "OGridNablaSq.h"
#include "WFGridSystem.h"


namespace QDLIB {

   OGridNablaSq::OGridNablaSq()
   : OKSpace(), _name("OGridNablaSq")
   {
      for (int i=0; i < MAX_DIMS; i++){
	 _mass[i] = -1;
      }
   }
   
   
   OGridNablaSq::~OGridNablaSq()
   {
   }

   /**
    * Init K-Space in one dimension.
    * All factors are included in the kspace representation except FFT normalization.
    * 
    * \param mass reduced mass
    */
   dVec * OGridNablaSq::InitKspace1D(const double mass, const double length, const int Nx)
   {

      dVec *kspace;
   
      kspace = new dVec(Nx);
   
      double dp = (2*M_PI) / length;    // Setup dp for kspace
      dp *= dp;   /* dp^2 */
   
      /* We include all the factors in the k-space function => Do it only once */
      /* The minus cancels with minus from -kx^2. */
      dp *= 0.5 / mass ;
     
      if (Nx % 2 == 0){ /* even + odd grid points */
	 for (int i=0; i < Nx / 2; i++){ //run from [-p..+p]
	    (*kspace)[i] = (double(i) * double(i)) * dp;
	    (*kspace)[Nx - i - 1] = (double(i+1) * double(i+1)) * dp;
	 }
      } else {
	 (*kspace)[0] = 0; 
	 for (int i=1; i < (Nx+1) / 2; i++){ //run from [-p..+p]
	    (*kspace)[i] = (double(i) * double(i)) * dp;
	    (*kspace)[Nx - i] = (double(i+1) * double(i+1)) * dp;
	 }
      }
      
      return kspace;
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
      
      int i=0;
      char c[256];
      string s;
   
      sprintf (c, "%d", i);
      s = string("mass") + string(c);
      while (i < n){
	 if ( _params.isPresent(s) ) {
	    _params.GetValue( string("mass") + string(c), _mass[i]);
	    if (_mass[i] == 0) throw ( EParamProblem ("Zero mass defined") );
	 } else _mass[i] = -1;    /* Mark as -1 => don't build k-space */
	 i++;
	 sprintf (c, "%d", i);
	 s = string("mass") + string(c);
      }
   }

   const string & OGridNablaSq::Name()
   {
      return _name;
   }

   void OGridNablaSq::UpdateTime(){}

   dcomplex OGridNablaSq::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
   {
      dcomplex c;
      WaveFunction *opPsi;
      
      opPsi = *this * PsiKet;
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
	 T += 1/ ( _mass[i] *  GridSystem::Dx(i) * GridSystem::Dx(i));
      T *= ( M_PI*M_PI / 2 );
      return T;
   }
	 
   double OGridNablaSq::Emin()
   {
      return 0; /* Minimum kinetic energy is zero */
   }
   
   WaveFunction * OGridNablaSq::operator *(WaveFunction *Psi)
   {
      WFGridSystem *opPsi, *ket;
      WaveFunction *psi;
      
      
      /* Copy */
     ket = dynamic_cast<WFGridSystem*>(Psi);
     if (ket == NULL)
	throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

     
      if (*this != *((GridSystem*) ket) || _kspace == NULL) {  /* re-Init k-space ?*/
	 *((GridSystem*) this) = *((GridSystem*) ket);
	 InitKspace();
      }
      
      psi =  Psi->NewInstance();
      *psi = Psi;
      
      opPsi = dynamic_cast<WFGridSystem*>(psi);
      if (opPsi == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      
      opPsi->ToKspace();
      MultElements((cVec*) opPsi, _kspace, 1/double(GridSystem::Size()));
//       MultElements((cVec*) opPsi, _kspace);
      opPsi->ToXspace();

      
      return opPsi;
   }

   WaveFunction * OGridNablaSq::operator *=(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(Psi);
      if (opPsi == NULL)
	 throw ( EIncompatible("Psi is not of type WFGridSystem", Psi->Name()) );

      
      if (*this != *((GridSystem*) opPsi)  || _kspace == NULL) {  /* re-Init k-space ?*/
	 *((GridSystem*) this) = *((GridSystem*) opPsi);
	 InitKspace();
      }    
      
      opPsi->ToKspace();
      MultElements((cVec*) opPsi, _kspace, 1/double(GridSystem::Size()));
      opPsi->ToXspace();
      
      return opPsi;
   }

   Operator * OGridNablaSq::operator =(Operator * O)
   {
      OGridNablaSq *org = dynamic_cast<OGridNablaSq*>(O);
      
      if (org == NULL)
	 throw ( EIncompatible("O is not of type OGridNablaSq", O->Name()) );

      
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) org);
      _kspace = org->_kspace;
      
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
    * \bug dimensions are inverse => x is the fastest!
    */
   void OGridNablaSq::InitKspace()
   {
   
      
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Missing GridSystem parameters") );
   
      dVec *kspace1;
   
      if (_kspace == NULL ) _kspace = new dVec(GridSystem::Size());
      else _kspace->newsize(GridSystem::Size());
      
      
      *_kspace = 0;
      dVecView view(*_kspace, GridSystem::Dim(), GridSystem::DimSizes());
   
      /* Init k-space for every dimension */
      for (int i=0; i < GridSystem::Dim(); i++){ 
	 if (_mass[i] > 0) {
	    kspace1 = InitKspace1D(_mass[i], GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i) );
	    view.ActiveDim(i);
	    view += *kspace1;
	    delete kspace1;
	 }
      }
      
      
   }

} /* namespace QDLIB */
