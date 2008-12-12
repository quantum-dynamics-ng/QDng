#include "OGridGMat.h"

#include "WFGridSystem.h"
#include "Kspace.h"
#include "sys/FileSingleDefs.h"


namespace QDLIB {

   OGridGMat::OGridGMat(): _name("OGridGmat"), _size(0), _Gmat(NULL), _kspace(NULL), _wfbuf(NULL)
   {
   }
   
   
   OGridGMat::~OGridGMat()
   {
      for(lint i=0; i < GridSystem::Dim(); i++){
	 if (_wfbuf[i] != NULL) delete _wfbuf[i];
      }
      if (_kspace != NULL) delete[] _kspace;
      if (buf != NULL) delete buf;
   }

   
   void OGridGMat::_InitKspace (WFGridSystem *Psi)
   {
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Missing GridSystem parameters") );
   
      dVec *kspace1;
      dVecView *view;
           
      
      *((GridSystem*) this) = *((GridSystem*) Psi);
  
      /* Setup single derivative for every coordinate */
      for (lint i=0; i < _size; i++)
      {
	 
	 if ( *((GridSystem*) (_Gmat[i][i])) != *((GridSystem*) Psi) )
	    throw ( EParamProblem("Gmatrix does not match Wavefunction") );
   	 
	 
	 /* derivatives */
         if (_kspace[i].size() == 0){
	    _kspace[i].newsize(Psi->lsize());
	    _kspace[i] = 0.0;
	 }
	 
	 kspace1 = Kspace::Init1Dddx(GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i));
	 view = new dVecView(_kspace[i], GridSystem::Dim(), GridSystem::DimSizes());
	 
	 view->ActiveDim(i);
	 *view += *kspace1;
	 
	 delete view;
	 delete kspace1;
      }
   }
   
   void OGridGMat::Init(WaveFunction *Psi)
   {
      buf = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
      if (buf == NULL)
	 throw (EIncompatible ("Psi not a WFGridSystem"), Psi->Name() );
      for (int i=0; i < _size; i++){
	 _wfbuf[i] = dynamic_cast<WFGridSystem*> (Psi->NewInstance());
      }
      
      _InitKspace(dynamic_cast<WFGridSystem*>(Psi->NewInstance()));
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
      _size=n;
      
      int i;
      _Gmat = new OGridPotential**[n];
      _kspace = new dVec[n];
      _wfbuf = new WFGridSystem*[n];
      
      for (i=0; i < n; i++){
	 _Gmat[i] = new OGridPotential*[n];
	 for (int j=0; j < n; j++){
	    _Gmat[i][j] = new OGridPotential();
	   
	 }
      }
     
      
      
      _params.GetValue( "gmat", name);
      if (name.empty())
	 throw( EParamProblem ("No G-matrix elements given"));
      
      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);
      
      /* Read Matrix elements */
      char si[32], sj[32];
      string s;
      for (i=0; i < n; i++){
	 for(int j=0; j < n; j++){
	    snprintf (si, 32, "%d", i);
	    snprintf (sj, 32, "%d", j);
	    s = name + string("_") + string(si) + string(sj);
	    cout << "Name " << s << endl;
	    file.Name(s);
	    file >> ((OGridSystem*) _Gmat[i][j]);
	    cout << _Gmat[i][j]->size()  << endl;
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
      Apply(opKet, PsiKet);
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
	 T += 1/ ( VecMin(*_Gmat[i][i]) *  GridSystem::Dx(i) * GridSystem::Dx(i));
      
      T *= ( M_PI*M_PI / 2 );
      return T;
   }
	 
   double OGridGMat::Emin()
   {
      return 0; /* Minimum kinetic energy is zero */
   }
   
   WaveFunction * OGridGMat::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      WFGridSystem *psi;
      
      psi = dynamic_cast<WFGridSystem*>(sourcePsi->NewInstance());
      if (psi == NULL) throw (EIncompatible("Psi is damaged") );
      
      /* Make a copy from Psi */
      for (int i=0; i < GridSystem::Dim(); i++)
	 _wfbuf[i]->FastCopy (*sourcePsi);
      
      *destPsi = dcomplex(0,0);
      
      lint i;
      for (i=0; i < _size; i++){ /* Loop over dims*/
	 /* d/dx from WF */ 
// 	 cout << *(_wfbuf[i]) * _wfbuf[i] <<" ";
	 _wfbuf[i]->ToKspace();
	 MultElementsComplex( (cVec*) _wfbuf[i], (dVec*) &(_kspace[i]), 1/double(_wfbuf[i]->size()) );
	 _wfbuf[i]->ToXspace();
	 
// 	 cout << *(_wfbuf[i]) * _wfbuf[i] <<endl;
	 
	 for (lint j=0; j < _size; j++){
	    *((cVec*) buf) = *((cVec*) _wfbuf[i]);
	    /* Multiply Gmatrix element */
// 	    cout << i <<" " << j<< " "<< (*(_Gmat[i][j]))[0] << endl;
	    MultElements( (cVec*) buf, (dVec*) _Gmat[i][j]);
	    /* d/dx from G* d/dx WF */
	    buf->ToKspace();
	    MultElementsComplex( (cVec*) buf, (dVec*) &(_kspace[j]), 0.5/double(_wfbuf[i]->size()) );
	    buf->ToXspace();
	    *destPsi -= buf;
	    
	 }
      }
      
      return destPsi;
   }

   WaveFunction * OGridGMat::Apply( WaveFunction * Psi )
   {
     
      WaveFunction *ket;
      
      ket=Psi->NewInstance();
      
      Apply(ket, Psi);
      
      *Psi = ket;

      delete ket;
      return Psi;
   }
   
   Operator * OGridGMat::operator =( Operator * O )
   {
      OGridGMat *o = dynamic_cast<OGridGMat*>(O);
      if (o == NULL)
	 throw ( EIncompatible("Error in Assignment", Name(), O->Name()) );
      
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) o);
      _params = o->_params;
      _size = o->_size;
      if(_kspace == NULL) _kspace = new dVec[_size];
      if (_Gmat == NULL) _Gmat = new OGridPotential**[_size];
      if (_wfbuf == NULL) _wfbuf = new WFGridSystem*[_size];
      for (int i=0; i < _size; i++){
	 _kspace[i] = o->_kspace[i];
	 _wfbuf[i] = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 buf = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 _Gmat[i] = new OGridPotential*[_size];
	 for(int j=0; j < _size; j++){
	    _Gmat[i][j] = new OGridPotential();
	    *(_Gmat[i][j]) = *(o->_Gmat[i][j]);
	 }
      }
      return this;
   }
   
   
   Operator * OGridGMat::operator *( Operator * O )
   {
      throw ( EIncompatible("Can't apply", Name(), O->Name()) );
   }
   
   
   Operator* OGridGMat::Scale(const double d)
   {
      for (lint i=0; i < GridSystem::Dim(); i++)
         MultElements(&_kspace[i], d);
      return this;
   }

}





