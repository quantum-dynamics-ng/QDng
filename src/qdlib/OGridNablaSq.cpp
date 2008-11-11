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
   
      double dp = (2*PI) / length;    // Setup dp for kspace
      dp *= dp;   /* dp^2 */
   
      /* We include all the factors in the k-space function => Do it only once */
      /* The minus cancels with minus from -kx^2. */
      dp *= 0.5 / mass ;
     
	 
      for (int i=0; i < Nx / 2; i++){ //run from [-p..+p]
	 (*kspace)[i] = (double(i) * double(i)) * dp;
	 (*kspace)[Nx - i - 1] = (double(i+1) * double(i+1)) * dp;
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

   WaveFunction * OGridNablaSq::operator *(WaveFunction *Psi)
   {
      WFGridSystem *opPsi, *ket;
      WaveFunction *psi;
      
      
      /* Copy */
      ket = dynamic_cast<WFGridSystem*>(Psi);
      if (*this != *((GridSystem*) ket) || _kspace == NULL) {  /* re-Init k-space ?*/
	 *((GridSystem*) this) = *((GridSystem*) ket);
	 InitKspace();
      }
      
      psi =  Psi->NewInstance();
      *psi = Psi;
      
      opPsi = dynamic_cast<WFGridSystem*>(psi);
   
      opPsi->ToKspace();
      MultElements((cVec*) opPsi, _kspace, 1/double(GridSystem::Size()));
      opPsi->ToXspace();
      
      
      return opPsi;
   }

   WaveFunction * OGridNablaSq::operator *=(WaveFunction * Psi)
   {
      WFGridSystem *opPsi;
      
      opPsi = dynamic_cast<WFGridSystem*>(Psi);
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
    * \bug dimensions are inverse!?
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
	    view.ActiveDim(GridSystem::Dim()-i-1);
	    view += *kspace1;
	    delete kspace1;
	 }
      }
//       for (int i=0; i < 256; i+=2){
// 	 for (int j=0; j < 256; j+=2){
// 	    cout <<  i << " " << j << " " << (*_kspace)[256*i+j] << endl;
// 	 }
//       }
      
      
   }

} /* namespace QDLIB */
