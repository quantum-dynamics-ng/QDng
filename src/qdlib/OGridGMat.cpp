#include "OGridGMat.h"

#include "WFGridSystem.h"
#include "tools/FileSingleDefs.h"
#include "qdlib/Diff.h"
#include "math/math_functions.h"

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OGridGMat)
   
   OGridGMat::OGridGMat(): _name("OGridGmat"), _size(0), _Gmat(NULL), _GmatC(NULL), _GmatDiff(NULL), _kspace(NULL),
                           _wfbuf(NULL), buf(NULL), _Abuf(NULL), _diff(1), _KinCoup(true), _ChainRule(false)
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

      if (_GmatC != NULL)
         for (int i = 0; i < _size; i++)
            delete[] _GmatC[i];

      delete[] _GmatC;
   }

   void OGridGMat::Init(WaveFunction *Psi)
   {
      WaveFunction *psi = dynamic_cast<WFGridSystem*>(Psi);
            
      if (psi == NULL)
	 throw (EIncompatible ("Psi not a WFGridSystem"), Psi->Name() );
      
      if ( *((GridSystem*) this) != *((GridSystem*) _Gmat[0][0]) )
	 throw (EIncompatible ("Gmatrix is incompatible with wave function!"));
      
      if (buf != NULL) return;  // Avoid init twice
     
     buf = dynamic_cast<WFGridSystem*>(Psi->NewInstance());
     buf->Retire();
     
      for (int i=0; i < _size; i++){
         _wfbuf[i] = dynamic_cast<WFGridSystem*> (Psi->NewInstance());
         _wfbuf[i]->Retire();
      }
      
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
                  if (j > i)
                     dx.Diff(& _GmatDiff[i][j],(dVec*)  _Gmat[j][i], i);
                  else
                     dx.Diff(& _GmatDiff[i][j],(dVec*)  _Gmat[i][j], i);
               }
            }
         }
      }
   }
   
   /**
    * \todo Clean memory if already initialized.
    */
   void OGridGMat::Init(ParamContainer &params)
   {
      int n;
      string name;

      _params = params;
      _params.GetValue("dims", n);

      if (n < 1)
         throw(EParamProblem("G-matrix operator needs at least one dimension"));

      GridSystem::Dim(n);
      _size = n;

      int i;
      _Gmat = new OGridPotential**[n];
      _GmatC = new double*[n];
      _kspace = new dVec[n];
      _wfbuf = new WFGridSystem*[n];

      for (i = 0; i < n; i++) {
         _wfbuf[i] = NULL;
         _Gmat[i] = new OGridPotential*[n];
         _GmatC[i] = new double[n];
         for (int j = 0; j <= i; j++) {
            _Gmat[i][j] = new OGridPotential();
         }
         for (int j = 0; j < n; j++)
            _GmatC[i][j] = 0;
      }

      _params.GetValue("gmat", name);
      if (name.empty())
         throw(EParamProblem("No G-matrix elements given"));

      _params.GetValue("coup", _KinCoup, true);

      OGridSystem::FileOGrid file;
      file.Suffix(BINARY_O_SUFFIX);

      /* Read Matrix elements */
      char si[32], sj[32];
      string s;
      for (i = 0; i < n; i++) { /* Loop over matrix elem. */
         for (int j = 0; j <= i; j++) {
            if (i == j || _KinCoup) { /* No off-diagonals if kinetic coupling is turned off*/
               snprintf(si, 32, "%d", i);
               snprintf(sj, 32, "%d", j);
               s = string("G") + string(si) + string(sj);
               if (_params.isPresent(s)) { /* Constant element - has precedence over grided element */
                  _params.GetValue(s, _GmatC[i][j]);
                  _GmatC[i][j] = _GmatC[j][i]; /* Symmetrize */
               } else { /* Coordinate dependent element */
                  s = name + string("_") + string(si) + string(sj);
                  file.Name(s);
                  file >> ((OGridSystem*) _Gmat[i][j]);

                  if (i != 0 && j != 0) { /* Check grid compatibility of G-Mat elem. */
                     if (*((GridSystem*) _Gmat[i][j]) != *((GridSystem*) _Gmat[0][0]))
                        throw(EIncompatible("Gmatrix elements incompatible"));
                  }
               }
            }
         }
      }
      *((GridSystem*) this) = *((GridSystem*) _Gmat[0][0]);

   }

   /**
    * Maximum kintetic energy is given by the maximum  possible momenta:
    * \f$ T_{max}  = \frac{\pi^2}{2} \sum_r \sum_s \frac{1}{\Delta x_r} G_{rs} \frac{1}{\Delta x_s} \f$
    */
   dcomplex OGridGMat::Emax()
   {
      if (GridSystem::Dim() == 0) throw ( EParamProblem("Gmatrix operator not initalized") );
      
      /* Calc Tmax on the Grid */
      double T=0;

      for (int i=0; i < GridSystem::Dim(); i++){
	 for(int j=0; j <= i; j++) {
	    if (i != j && _KinCoup) {
	       double max, min;
	       max = VecMax(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
	       min = VecMin(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
	       if (min < 0 && fabs(min) > fabs(max))
		  T += 2 * fabs(min);
	       else
		  T += 2 * max;
            } else if (i==j)
	       T += VecMax(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));

	 }
      }
      
      T *= ( M_PI*M_PI / 2 );
      return dcomplex(T);
   }
	 
   dcomplex OGridGMat::Emin()
   {
      double Tmin = 0;
      for (int i=0; i < GridSystem::Dim(); i++){
	 for(int j=0; j < i; j++) {
	    double min;
            if ( i == j || _KinCoup ) {
               min = VecMin(*(_Gmat[i][j])) / (GridSystem::Dx(i) * GridSystem::Dx(j));
               if (min < 0)
                  Tmin += min;
            }
	 }
      }
      return dcomplex(Tmin);
   }
   
   void OGridGMat::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
   {
      *destPsi = dcomplex(0, 0);
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

      lint i;
      for (i = 0; i < _size; i++)
      { /* Loop over dims*/
         /* d/dx from WF */
         _diff.DnDxn(_wfbuf[i], sourcePsi, i);

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
                  if (_GmatC[i][j] != 0){ /* Constant G-Element*/
                     /* d/dx from G* d/dx WF */
                     _diff.DnDxnAdd(destPsi, _wfbuf[i], j, -0.5);
                  } else {
                     /* Coordinate dependent lu*/
                     if (j > i) /* Gmatrix it self is symmetric - but not the mixed derivatives !!!*/
                        MultElementsCopy((cVec*) buf, (cVec*) _wfbuf[i] ,  (dVec*) _Gmat[j][i]);
                     else MultElements((cVec*) buf, (cVec*) _wfbuf[i], (dVec*) _Gmat[i][j]);

                     /* d/dx from G* d/dx WF */
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
      if (_GmatC == NULL) _GmatC = new double*[_size];
      if (_wfbuf == NULL) _wfbuf = new WFGridSystem*[_size];
      
      for (int i=0; i < _size; i++)
         _GmatC[i] = new double[_size];
      
      for (int i=0; i < _size; i++){
	 _kspace[i] = o->_kspace[i];
	 _wfbuf[i] = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 buf = dynamic_cast<WFGridSystem*>(o->_wfbuf[i]->NewInstance());
	 _Gmat[i] = new OGridPotential*[_size];
	 
	 for(int j=0; j <= i; j++){
            if (o->_Gmat[i][j] != NULL){
	       _GmatC[i][j] = o->_GmatC[i][j];
	       _GmatC[j][i] = o->_GmatC[i][j];
	       if (o->_GmatC[i][j] == 0) {
                  _Gmat[i][j] = new OGridPotential();
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


