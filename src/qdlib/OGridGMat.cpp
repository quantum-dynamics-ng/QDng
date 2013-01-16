#include "OGridGMat.h"

#include "qdlib/WFGridSystem.h"
#include "tools/helpers.h"
#include "tools/FileSingleDefs.h"
#include "qdlib/Diff.h"
#include "math/math_functions.h"


namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridGMat)
   
   OGridGMat::OGridGMat(): _name("OGridGmat"), _size(0), _Gmat(NULL), _GmatDiff(NULL), _kspace(NULL),
                           _wfbuf(NULL), buf(NULL), buf1(NULL), _Abuf(NULL), _diff(1), _pml(NULL), _KinCoup(true), _ChainRule(false)
   {
   }
   
   
   OGridGMat::~OGridGMat()
   {
      if (_wfbuf != NULL) {
         for (lint i = 0; i < GridSystem::Dim(); i++) {
            DELETE_WF(_wfbuf[i]);
         }
         delete[] _wfbuf;
      }

      DELETE_WF(buf);
      DELETE_WF(buf1);

      if (_Gmat != NULL) {
         for (int i = 0; i < _size; i++) {
            for (int j = 0; j <= i; j++) {
               DELETE_OP(_Gmat[i][j]);
            }
            delete[] _Gmat[i];
         }
         delete[] _Gmat;
      }

      if (_GmatDiff != NULL) {
         for (int i = 0; i < _size; i++) {
            delete[] _GmatDiff[i];
         }
         delete[] _GmatDiff;
      }

      if (_pml != NULL) delete[] _pml;
   }

   void OGridGMat::Init(WaveFunction *Psi)
   {
      WFGridSystem *psi = dynamic_cast<WFGridSystem*>(Psi);

      if (psi == NULL)
         throw (EIncompatible ("Psi not a WFGridSystem"), Psi->Name() );

      if (buf != NULL) return;  // Avoid init twice

      *((GridSystem*) this) = *((GridSystem*) psi);

      /* Check compatibility of gridsystems */
      for (int i=0; i < Dim(); i++){
         for (int j=0; j <= i ; j++){
            if (_src(i,j) == grd)
               if (*((GridSystem*) _Gmat[i][j]) != *((GridSystem*) psi))
                  throw(EIncompatible("Gmatrix element is incompatible with wave function"));
         }
      }


      /* Check if PML is requested */
      bool pml;
      _params.GetValue("pml", pml, false);

      if (pml){
         if (_ChainRule)
            throw (EParamProblem("Gmat: Chainrule and PML is not supported"));

         _pml = new PML[Dim()];

         for (int i=0; i < Dim(); i++){
            _pml[i].SetGrid(*this);
            _pml[i].InitParams(_params, i);
         }
      }
      
     /* Setup buffers */
     buf = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
     buf1 = Psi->NewInstance();
     buf->Retire();
     buf1->Retire();
     
      for (int i=0; i < _size; i++){
         _wfbuf[i] = dynamic_cast<WFGridSystem*> (Psi->NewInstance());
         _wfbuf[i]->Retire();
      }
      
      /* Setup derivatives */
      _diff.Collective(false);
      _diff.Mixed(false);
      _diff.Single(true);

      _diff.SetGrid(*((GridSystem*) this));
      _diff.InitParams(_params);

      _params.GetValue("chainrule", _ChainRule, false);

      /* Create derivatives of Gmatrix elements */
      if (_ChainRule){
         int order=8; /* default order for derivatives of Gmat-elements */
         if (_params.isPresent("order"))
            _params.GetValue("order",order);

         HOFD<double> dx(1,order);
         dx.SetFactor(-0.5);
         dx.SetPBC(false);
         dx.SetGrid(*((GridSystem*) this) );

         _GmatDiff = new dVec*[_size];

         for (int i=0; i < _size; i++){
            _GmatDiff[i] = new dVec[_size];
            for (int j=0; j < _size; j++){
               if (_Gmat[i][j] != 0  || _Gmat[j][i] != 0 ){
                  _GmatDiff[i][j].newsize(Psi->size());
                  dx.Diff(& _GmatDiff[i][j],(dVec*)  _Gmat[i][j], i);
               }
            }
         }
      }
   }
   
   /**
    *
    */
   void OGridGMat::Init(ParamContainer &params)
   {
      uint n;
      string name;

      _params = params;
      _params.GetValue("dims", n);

      if (n < 1)
         throw(EParamProblem("G-matrix operator needs at least one dimension"));

      GridSystem::Dim(n);
      _size = n;

      uint i;
      _Gmat = new OGridPotential**[n];
      _GmatC.newsize(n,n);
      _src.newsize(n,n);
      _kspace = new dVec[n];
      _wfbuf = new WFGridSystem*[n];

      for (i = 0; i < n; i++) {
         _wfbuf[i] = NULL;
         _Gmat[i] = new OGridPotential*[n];
         for (uint j = 0; j <= i; j++) {
            _Gmat[i][j] = NULL;
         }
      }
      _GmatC = 0.;
      _src = none;

      _params.GetValue("coup", _KinCoup, true);

      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);

      Matrix<char> gdefs;
      gdefs.newsize(n,n);

      if (_params.isPresent("G"))
         gdefs = 'c';
      else
         gdefs = 'g';

      /* Check for definitions */
      if (_params.isPresent("Gdef")){
         vector<char> gmc;
         _params.GetArray("Gdef", gmc);

         if ( gmc.size() > (n*n + n) / 2 ) /* Cut down to matrix dimension - ignore the rest */
            gmc.resize((n*n + n) / 2);

         ReorderLowerTriangular(gmc, gdefs, true);
      }

      /* Read Constant G-Matrix elements and/or defs - lower triangular format */
      if (_params.isPresent("G")){
         vector<double> gmc;
         _params.GetArray("G", gmc);

         if ( gmc.size() > (n*n + n) / 2 ) /* Cut down to matrix dimension - ignore the rest */
            gmc.resize((n*n + n) / 2);

         ReorderLowerTriangular(gmc, _GmatC, true);
      }

      /* Check what matrix elements are requested */
      for (uint i=0; i < n; i++){
         for (uint j=0; j < n; j++){
            if (gdefs(i,j) == 'x') /* don't use this matrix element */
               _src(i,j) = none;
            else if (gdefs(i,j) == 'c')
               _src(i,j) = cnst;
            else if (gdefs(i,j) == 'g') /* Read from grid file */
               _src(i,j) = grd;
         }
      }

      /* Read Matrix elements */
      char si[32], sj[32];
      string s;
      _params.GetValue("gmat", name);

      for (i = 0; i < n; i++) { /* Loop over matrix elem. */
         for (uint j = 0; j <= i; j++) {
            if (i == j || _KinCoup) { /* No off-diagonals if kinetic coupling is turned off*/
               snprintf(si, 32, "%d", i);
               snprintf(sj, 32, "%d", j);

               if (_src(i,j) == grd) {
                  if (name.length() < 2)
                       throw(EParamProblem("No G-matrix elements given"));

                  _Gmat[i][j] = new OGridPotential();
                  _Gmat[j][i] = _Gmat[i][j];
                  s = name + string("_") + string(si) + string(sj);
                  file.Name(s);
                  file >> ((OGridSystem*) _Gmat[i][j]);
               }
            }
         }
      }
   }

   /**
    * Maximum kintetic energy is given by the maximum  possible momenta:
    * \f$ T_{max}  = \frac{\pi^2}{2} \sum_r \sum_s \frac{1}{\Delta x_r} G_{rs} \frac{1}{\Delta x_s} \f$
    */
   dcomplex OGridGMat::Emax()
   {
      if (GridSystem::Dim() == 0)
         throw(EParamProblem("Gmatrix operator not initalized"));

      /* Calc Tmax on the Grid */
      double T = 0;

      for (int i = 0; i < GridSystem::Dim(); i++) {
         for (int j = 0; j <= i; j++) {
            if (i != j && _KinCoup) {
               double max=0, min=0;
               if (_src(i,j) == grd) {
                  max = VecMax(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
                  min = VecMin(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
               } else if (_src(i,j) == cnst) {
                  max = _GmatC(i, j);
                  min = _GmatC(i, j);
               }
               if (min < 0 && fabs(min) > fabs(max))
                  T += 2 * fabs(min);
               else T += 2 * max;
            } else if (i == j){
               if (_src(i,j) == grd)
                  T += VecMax(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
               else if(_src(i,j) == cnst)
                  T += _GmatC(i, j) / (GridSystem::Dx(i) * GridSystem::Dx(j));
            }

         }
      }

      T *= (M_PI * M_PI / 2);
      return dcomplex(T);
   }
	 
   dcomplex OGridGMat::Emin()
   {
      double Tmin = 0;
      for (int i = 0; i < GridSystem::Dim(); i++) {
         for (int j = 0; j < i; j++) {
            double min=0;
            if (i == j || _KinCoup) {

               if (_src(i,j) == grd)
                  min = VecMin(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
               else if (_src(i,j) == cnst)
                  min = _GmatC(i,j);

               if (min < 0)
                  Tmin += min;
            }
         }
      }
      return dcomplex(Tmin);
   }
   
   void OGridGMat::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      *destPsi = dcomplex(0);
      OGridGMat::ApplyAdd(destPsi, sourcePsi);
   }

   void OGridGMat::ApplyAdd(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      /* Make a copy from Psi */
      for (int i = 0; i < GridSystem::Dim(); i++)
      {
         _wfbuf[i]->Reaquire();
      }

      buf->Reaquire();
      buf1->Reaquire();

      lint i;
      for (i = 0; i < _size; i++){ /* Loop over dims*/
         /* d/dx from WF */
         _diff.DnDxn(_wfbuf[i], sourcePsi, i);

//         if (_pml)
//            _pml[i].ApplyTransform(_wfbuf[i]);

         for (lint j = 0; j < _size; j++){
            if ((i == j) | _KinCoup){ /* Kinetic coupling ?*/

               if (_ChainRule){
                  /* (dxi * Gij) * dxj + Gij * dxij \todo make aware of const. G */
                  MultElementsAdd((cVec*) destPsi, (cVec*) _wfbuf[i],(dVec*) & _GmatDiff[j][i]);
                  if (j <= i){
                     _diff.DnDxn(buf, _wfbuf[i], j, (i==j) ? -.5 : -1.);
                     MultElementsAdd((cVec*) destPsi, (cVec*) buf, (dVec*) _Gmat[i][j]);
                  }
               } else {
                  /* dxi * Gij * dxj*/
                  /* Multiply Gmatrix element */
                  if (_src(i,j) == cnst){ /* Constant G-Element*/
                     /* d/dx from G* d/dx WF */
                     if (_pml){
                        _diff.DnDxn(buf, _wfbuf[i], j , -0.5 * _GmatC(i,j));
//                        if (i == j)
//                           _pml[j].ApplyTransformDiff(buf);

                        _pml[i].ApplyTransform(buf);
                        _pml[j].ApplyTransform(buf);
                        *destPsi += buf;
                     } else {
                        *buf = _wfbuf[i];
                        _diff.DnDxnAdd(destPsi, buf, j , -0.5 * _GmatC(i,j));
                     }

                  } else if (_src(i,j) == grd) {
                     /* Coordinate dependent lu*/
                     MultElements((cVec*) buf, (cVec*) _wfbuf[i], (dVec*) _Gmat[i][j]);

                     /* d/dx from G* d/dx WF */
                     if (_pml){
                        _diff.DnDxn(buf1, buf, j , -0.5);
                        _pml[j].ApplyTransform(buf1);
                        *destPsi += buf1;
                        //_pml[j].ApplyTransformAdd(destPsi, buf1);
                     } else
                        _diff.DnDxnAdd(destPsi, buf, j , -0.5);
                  }
               }
            }
         }
      }

      for (int i = 0; i < GridSystem::Dim(); i++)
      {
         _wfbuf[i]->Retire();
      }

      buf->Retire();
      buf1->Retire();
   }


   void OGridGMat::Apply( WaveFunction * Psi )
   {
     
     if (_Abuf == NULL)
         _Abuf = Psi->NewInstance();
      else
         _Abuf->Reaquire();

      Apply(_Abuf, Psi);
      *Psi = _Abuf;
      _Abuf->Retire();
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
      _GmatC = o->_GmatC;
      if (_wfbuf == NULL) _wfbuf = new WFGridSystem*[_size];

      for (int i=0; i < _size; i++){
	 _kspace[i] = o->_kspace[i];
	 _wfbuf[i] = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 buf = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 _Gmat[i] = new OGridPotential*[_size];
	 
	 for(int j=0; j <= i; j++){
            if (o->_Gmat[i][j] != NULL){
	       if (o->_GmatC(i,j) < 0) {
                  _Gmat[i][j] = new OGridPotential();
                  _Gmat[j][i] = _Gmat[i][j];
                  *(_Gmat[i][j]) = *(o->_Gmat[i][j]);
	       }
            }
	 }
      }
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


