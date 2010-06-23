#include "OGridGMat.h"

#include "WFGridSystem.h"
#include "Kspace.h"
#include "tools/FileSingleDefs.h"
#include "math/math_functions.h"

namespace QDLIB {

   OGridGMat::OGridGMat(): _name("OGridGmat"), _size(0), _Gmat(NULL), _kspace(NULL), _wfbuf(NULL), buf(NULL), _KinCoup(true)
   {
   }
   
   
   OGridGMat::~OGridGMat()
   {
      if (_wfbuf != NULL){
         for(lint i=0; i < GridSystem::Dim(); i++){
            if (_wfbuf[i] != NULL) delete _wfbuf[i];
         }
         delete[] _wfbuf;
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
	    _kspace[i].newsize(Psi->size());
	 }
	 
	 kspace1 = Kspace::Init1Dddx(GridSystem::Xmax(i) - GridSystem::Xmin(i), GridSystem::DimSizes(i));
	 view = new dVecView(_kspace[i], GridSystem::Dim(), GridSystem::DimSizes());
	 
	 view->ActiveDim(i);
	 *view = *kspace1;
	 
	 delete view;
	 delete kspace1;
      }
   }
   
   void OGridGMat::Init(WaveFunction *Psi)
   {
      buf = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
            
      if (buf == NULL)
	 throw (EIncompatible ("Psi not a WFGridSystem"), Psi->Name() );
      
     if ( *((GridSystem*) this) != *((GridSystem*) _Gmat[0][0]) )
	 throw (EIncompatible ("Gmatrix is incompatible with wave function!"));
      
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
         _wfbuf[i] = NULL;
	 _Gmat[i] = new OGridPotential*[n];
	 for (int j=0; j <= i; j++){
	    _Gmat[i][j] = new OGridPotential();
	   
	 }
      }
     
      _params.GetValue( "gmat", name);
      if (name.empty())
	 throw( EParamProblem ("No G-matrix elements given"));
      
      _params.GetValue("coup", _KinCoup, true);
      
      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);
      
      /* Read Matrix elements */
      char si[32], sj[32];
      string s;
      for (i=0; i < n; i++){ /* Loop over matrix elem. */
	 for(int j=0; j <= i; j++){
	    if (!(i != j && !_KinCoup)){ /* No off-diagonals if kinetic coupling is turned off*/
	       snprintf (si, 32, "%d", i);
	       snprintf (sj, 32, "%d", j);
	       s = name + string("_") + string(si) + string(sj);
	       file.Name(s);
	       file >> ((OGridSystem*) _Gmat[i][j]);
               
               if (i != 0 && j != 0) { /* Check grid compatibility of G-Mat elem. */
                  if ( *((GridSystem*)  _Gmat[i][j]) != *((GridSystem*) _Gmat[0][0]) )
                     throw (EIncompatible ("Gmatrix elements incompatible"));
               }
	    }
	 }
      }
      *((GridSystem*) this) = *((GridSystem*) _Gmat[0][0]);
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
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Gmatrix operator not initalized") );
      
      /* Calc Tmax on the Grid */
      double T=0;
      if (_size == 1 ){ // Simple case
	 T = VecMin(*(_Gmat[0][0])) / (GridSystem::Dx(0) * GridSystem::Dx(0));
      } else  if (_size == 2 ){ // Quick diag
	 double g0, g1, g10;
         double t[2];
	 g0 =  VecMin(*(_Gmat[0][0]));
	 g1 = VecMin(*(_Gmat[1][1]));
         if (_KinCoup){
            g10 = VecMin(*(_Gmat[1][0]));
	    diag22symm(g0, g1, g10, t);
            T = t[0]/ (GridSystem::Dx(0) * GridSystem::Dx(0));
            T += t[1]/ (GridSystem::Dx(1) * GridSystem::Dx(1));
         } else {
            T = g0/ (GridSystem::Dx(0) * GridSystem::Dx(0));
            T += g1/ (GridSystem::Dx(1) * GridSystem::Dx(1));
         }
	 
         
      } else {
	 for (int i=0; i < GridSystem::Dim(); i++)
	    T += 1/ ( VecMin(*(_Gmat[i][i])) *  GridSystem::Dx(i) * GridSystem::Dx(i));
      }
      T *= ( M_PI*M_PI / 2 );
      return T;
   }
	 
   double OGridGMat::Emin()
   {
      return 0; /* Minimum kinetic energy is zero */
   }
   
   WaveFunction * OGridGMat::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      /* Make a copy from Psi */
      for (int i=0; i < GridSystem::Dim(); i++)
	 _wfbuf[i]->FastCopy (*sourcePsi);
      
      *destPsi = dcomplex(0,0);
      
      lint i;
      for (i=0; i < _size; i++){ /* Loop over dims*/
	 /* d/dx from WF */ 
	 _FFT.Forward(_wfbuf[i]);
	 MultElementsComplex( (cVec*) _wfbuf[i], (dVec*) &(_kspace[i]), 1/double(buf->size()) );
         _FFT.Backward(_wfbuf[i]);
         
 	 for (lint j=0; j < _size; j++){
	    if (i == j || _KinCoup){ /* Kinetic coupling ?*/
	       *((cVec*) buf) = *((cVec*) _wfbuf[i]);
	       /* Multiply Gmatrix element */
	       if ( j>i) /* Gmatrix it self is symmetric - but not the mixed derivatives !!!*/
	         MultElements( (cVec*) buf, (dVec*) _Gmat[j][i]);
	       else
	         MultElements( (cVec*) buf, (dVec*) _Gmat[i][j]);
	       /* d/dx from G* d/dx WF */
               _FFT.Forward(buf);
	       MultElementsComplex( (cVec*) buf, (dVec*) &(_kspace[j]), -.5/double(buf->size()) );
               _FFT.Backward(buf);
	       
	       *destPsi += buf;
	    }
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
      Copy(O);
      return this;
   }
   
   
   Operator * OGridGMat::Copy(Operator * O)
   {
      OGridGMat *o = dynamic_cast<OGridGMat*>(O);
      if (o == NULL)
	 throw ( EIncompatible("Error in Assignment", Name(), O->Name()) );
      
      /* Copy parents */
      *((GridSystem*) this) = *((GridSystem*) o);
      _params = o->_params;
      _size = o->_size;
      
      _KinCoup = o->_KinCoup;
      
      if(_kspace == NULL) _kspace = new dVec[_size];
      if (_Gmat == NULL) _Gmat = new OGridPotential**[_size];
      if (_wfbuf == NULL) _wfbuf = new WFGridSystem*[_size];
      for (int i=0; i < _size; i++){
	 _kspace[i] = o->_kspace[i];
	 _wfbuf[i] = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 buf = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 _Gmat[i] = new OGridPotential*[_size];
	 for(int j=0; j <= i; j++){
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
      for (lint i=0; i < _size; i++)
         MultElements(&_kspace[i], sqrt(d));
      scaling=d;
      return this;
   }

   
   bool OGridGMat::Valid(WaveFunction * Psi)
   {
      WFGridSystem *PsiGS;
      
      PsiGS = dynamic_cast<WFGridSystem*>(Psi);
      
      if ( PsiGS == NULL) return false;
      
      if ( *((GridSystem*) this) != *((GridSystem*) PsiGS) ) return false;
      
      return true;
   }
}


