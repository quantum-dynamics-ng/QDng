#include "OGridGMat.h"

#include "WFGridSystem.h"
#include "Kspace.h"
#include "sys/FileSingleDefs.h"


namespace QDLIB {

   OGridGMat::OGridGMat(): _name("OGridGmat"), _size(0), _Gmat(0,0), _kspace(0,0), _wfbuf(0)
   {
   }
   
   
   OGridGMat::~OGridGMat()
   {
      for(lint i=0; i < GridSystem::Dim(); i++){
	 if (_wfbuf[i] != NULL) delete _wfbuf[i];
      }
   }

   
   void OGridGMat::_InitKspace (WaveFunction *Psi)
   {
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Missing GridSystem parameters") );
   
      dVec *kspace1;
      dVecView *view;
      
      WFGridSystem* psi;
      psi = dynamic_cast<WFGridSystem*>(Psi);
      
      if (psi == NULL)
	 throw ( EIncompatible("Psi not of type WFGridSystem", Psi->Name()) );
      
      *((GridSystem*) this) = *((GridSystem*) psi);
  
      /* Setup single derivative for every coordinate */
      for (lint i=0; i < GridSystem::Dim(); i++)
      {
	 if ( ((GridSystem) (_Gmat[i][i])) != *((GridSystem*) psi) )
	    throw ( EParamProblem("Gmatrix does not match Wavefunction") );
   
	 /* buffers */
	 if (_wfbuf[i] == NULL) _wfbuf[i] = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
	 
	 /* derivatives */
         if (_kspace[i].size() == 0){
	    _kspace[i].newsize(GridSystem::Size(),0);
	 }
	 kspace1 = Kspace::Init1Dddx(GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i));
	 view = new dVecView(_kspace[i], GridSystem::Dim(), GridSystem::DimSizes());
	 
	 view->ActiveDim(i);
	 *view += *kspace1;
	 
	 delete view;
	 delete kspace1;
      }
   }
   
   Operator* OGridGMat::NewInstance()
   {
      OGridGMat* r = new OGridGMat();
      return r;
   }
   
   /**
    * \todo Clean memory if already initialized.
    */
   void OGridGMat::Init(ParamContainer &params)
   {
      int n;
      string name;
      
      _params = params;
      _params.GetValue( "dims", n);
   
      if (n < 1)
	 throw ( EParamProblem ("G-matrix operator needs at least one dimension") );
      GridSystem::Dim(n);
      
      _kspace.newsize(n,0);   /* Setup k-space, init every kspace Vector with size zero */
      _wfbuf.newsize(n);      /* We need dim buffers */
      
      for (lint i=0; i < n; i++)
      {
	_wfbuf[i] = NULL;
      }
      
      _params.GetValue( "gmat", name);
      
      if (name.empty())
	 throw( EParamProblem ("No G-matrix elements given"));
      
      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);
      
      /* Read Matrix elements */
      int i;
      char si[32], sj[32];
      string s;
      for (i=0; i < n; i++){
	 for(int j=0; j < (i + 1); j++){
	    snprintf (si, 32, "%d", i);
	    snprintf (sj, 32, "%d", j);
	    s = name + string("_") + string(si) + string("_") + string(sj);
	    file.Name(s);
	    file >> ((OGridSystem*) &_Gmat[i][j]);
	 }
      }
      
   }
   
   const string & OGridGMat::Name( )
   {
      return _name;
   }

   dcomplex OGridGMat::MatrixElement( WaveFunction * PsiBra, WaveFunction * PsiKet )
   {
      dcomplex d;
      WaveFunction *opKet;
      
      opKet = PsiKet->NewInstance();
      d = *PsiBra * opKet;
      delete opKet;
      
      return d;
   }

   double OGridGMat::Expec( WaveFunction * Psi )
   {
      dcomplex d;
      
      d = MatrixElement(Psi,Psi);
      
      return d.real();
   }

   double OGridGMat::Emax()
   {
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Nabla operator not initalized") );
      
      /* Calc Tmax on the Grid */
      double T=0;
      
      for (int i=0; i < GridSystem::Dim(); i++)
	 T += 1/ ( VecMin(_Gmat[i][i]) *  GridSystem::Dx(i) * GridSystem::Dx(i));
      
      T *= ( M_PI*M_PI / 2 );
      return T;
   }
	 
   double OGridGMat::Emin()
   {
      return 0; /* Minimum kinetic energy is zero */
   }
   
   WaveFunction * OGridGMat::operator *( WaveFunction * Psi )
   {
      WaveFunction *psi;
      if (_kspace.size() == 0) _InitKspace(Psi);
      
      psi = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
      if (psi == NULL)
	 throw ( EIncompatible ("Can't apply to ", Psi->Name()) );
      
      /* Make a copy from Psi */
      for (int i=0; i < GridSystem::Dim(); i++)
	 *((WaveFunction*) _wfbuf[i]) = Psi;
      
      *Psi = dcomplex(0,0);
      
      lint i;
      for (i=0; i < GridSystem::Dim(); i++){
	 /* d/dx from WF */
	 _wfbuf[i]->ToKspace();
	 MultElementsComplex( (cVec*) _wfbuf[i], &_kspace[i], 1/double(GridSystem::Size()) );
	 _wfbuf[i]->ToXspace();
	 
	 for (lint j=0; j < (i+1); j++){
	    /* Multiply Gmatrix element */
	    MultElements( (cVec*) _wfbuf[i], (dVec*) &_Gmat[i][j]);
	    _wfbuf[i]->ToKspace();
	    MultElementsComplex( (cVec*) _wfbuf[i], &_kspace[j], 1/double(GridSystem::Size()) );
	    _wfbuf[i]->ToXspace();
	    *psi += _wfbuf[i];
	 }
      }
      
      return psi;
   }

   WaveFunction * OGridGMat::operator *=( WaveFunction * Psi )
   {
      WaveFunction *psi;
      if (_kspace.size() == 0) _InitKspace(Psi);
      
      psi = dynamic_cast<WFGridSystem*>(Psi);
      if (psi == NULL)
	 throw ( EIncompatible ("Can't apply to ", Psi->Name()) );
      
      /* Make a copy from Psi */
      for (int i=0; i < GridSystem::Dim(); i++)
         *((WaveFunction*) _wfbuf[i]) = Psi;
      
      *Psi = dcomplex(0,0);
      
      lint i;
      for (i=0; i < GridSystem::Dim(); i++){
	 /* d/dx from WF */
	 _wfbuf[i]->ToKspace();
	 MultElementsComplex( (cVec*) _wfbuf[i], &_kspace[i], 1/double(GridSystem::Size()) );
	 _wfbuf[i]->ToXspace();
	 
	 for (lint j=0; j < (i+1); j++){
	    /* Multiply Gmatrix element */
	    MultElements( (cVec*) _wfbuf[i], (dVec*) &_Gmat[i][j]);
	    _wfbuf[i]->ToKspace();
	    MultElementsComplex( (cVec*) _wfbuf[i], &_kspace[j], 1/double(GridSystem::Size()) );
	    _wfbuf[i]->ToXspace();
	    *psi += _wfbuf[i];
	 }
      }
      
      return psi;
   }
   
   Operator * OGridGMat::operator =( Operator * O )
   {
      OGridGMat *o = dynamic_cast<OGridGMat*>(O);
      if (o == NULL)
	 throw ( EIncompatible("Error in Assignment", Name(), O->Name()) );
      
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) o);
      
      _size = o->_size;
      _Gmat = o->_Gmat;
      _kspace = o->_kspace;
      
      for (int i=0; i < _size; i++)
	 _wfbuf[i] = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
      
      return this;
   }
   
   
   Operator * OGridGMat::operator *( Operator * O )
   {
      throw ( EIncompatible("Can't apply", Name(), O->Name()) );
   }
   
   
   Operator* OGridGMat::Scale(const double d)
   {
      if (_kspace.size() == 0)
	 throw ( EParamProblem("k-space not initialized", Name()) );
      
      for (lint i=0; i < GridSystem::Dim(); i++)
         MultElements(&_kspace[i], d);
      
      return this;
   }

}





