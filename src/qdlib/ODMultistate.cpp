// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "ODMultistate.h"
#include "linalg/LapackDiag.h"

namespace QDLIB
{

   ODMultistate::ODMultistate() :  OMultistate(), ODSpace(),
         _name("ODMultistate"), _X(NULL), _T(NULL),  _state_size(0), _IsFullDiag(true)
   {
   }

   ODMultistate::~ODMultistate()
   {
      if (_X != NULL){
         for (int i = 0; i < _state_size; i++) {
            delete _X[i];
         }
         delete[] _X;
      }
      if (_T != NULL)  delete[] _T;
      if (_dspace != NULL) delete _dspace;
   }

   Operator * ODMultistate::NewInstance()
   {
      ODMultistate* r = new ODMultistate();
      
      return (OMultistate*) r;
   }
   
   void ODMultistate::Init(WaveFunction *Psi)
   {
      
      if (_state_size != 0) return;  // Avoid init twice
         
      /* Parent initalization */
      OMultistate::Init(Psi);
      
      WFMultistate* wfm = dynamic_cast<WFMultistate*>(Psi);

      
      lint last_size = wfm->State(0)->size();
      for (int i=1; i < States(); i++){
         if (last_size != wfm->State(0)->size())
            throw( EIncompatible("ODMultistate relies on states with same size") );
      }
      _state_size = last_size;
      
      bool external=false;
      _IsFullDiag=true;
      _T = new Transform*[States()];
      
      
      /* Check for transform type & validty of matrix elements */
      for (int i=0; i < States(); i++){
         for (int j=0; j < States(); j++){
            ODSpace *od =  dynamic_cast<ODSpace*>(State(i,j));
            if ( od == NULL && State(i,j) != NULL)
               throw ( EIncompatible ("ODMultistate can only handle DSpace operators") );
            
            if ( State(i,j) != NULL ) {      /* Check for external transformation */
               if ( od->Transformation() != NULL ) {
                  external = true;
                  _IsFullDiag = false;
                  if ( i != j )
                     throw ( EIncompatible ("ODMultistate can't transform non-diagonal operators in non-diagonal matrices") );

                  _T[i] = od->Transformation();
               } else if (i != j) _IsFullDiag = false;
            }
         }
      }
      if (external) _XT.ExternalTransform(_T);
      else {
         delete[] _T;
         _T = NULL;
      }
   }
   
   Operator * ODMultistate::Copy(Operator * O)
   {
      
      ODMultistate* o = dynamic_cast<ODMultistate*>(O);
      
      if (o == NULL)
         throw ( EIncompatible("ODMultistate: Can't copy ", O->Name()) );
      
      OMultistate::Copy(O);
      
      _state_size = o->_state_size;
      _IsFullDiag = o->_IsFullDiag;
      /* Copy external transformations */ 
      if (o->_T != NULL) {
         _T = new Transform*[States()];
         for (int i=0; i < States(); i++){
            for (int j=0; j < States(); j++){
               ODSpace *od =  dynamic_cast<ODSpace*>(State(i,j));
               if (od != NULL)
                  if ( od->Transformation() != NULL )
                     _T[i] = od->Transformation();
            }
         }
         _XT.ExternalTransform(_T);
      }
      
      if (o->_dspace != NULL) InitDspace();
      
      return (OMultistate*) this;
   }
   
   void ODMultistate::InitDspace()
   {
      /* First init */
      if (_dspace == NULL) {
         /* Create d-space */
         _dspace = new dVec();
         _dspace->newsize(_state_size * States(), States());
         
         if (_IsFullDiag){ /* Diagonal H*/
            for (int k = 0; k < States(); k++) {
               ODSpace *state = dynamic_cast<ODSpace*>(State(k,k));
               _dspace->StrideCopy(*(state->Dspace()), 0,k);
            }
         }
         
         if (_T == NULL && ! _IsFullDiag){ /* Init Grid of Matrices */
            _X = new dMat*[_state_size];
            for (int i = 0; i < _state_size; i++) {
               _X[i] = new dMat(States(), States());
            }
         }
      }
      
      if (_T == NULL && !_IsFullDiag){ /* Grid Diagonalization - Internal Transform */
         dVec evals(States());
   
         /* diagonalize at every grid point */
         for (int i = 0; i < _state_size; i++) { /* Run over Levels _in_ the states */
            for (int k = 0; k < States(); k++) {   /*  loop over states => create Matrix */
               for (int l = 0; l < States(); l++) {
                  if ( State(k,l) != NULL){
                     ODSpace *state = dynamic_cast<ODSpace*>(State(k,l));
                     (*(_X[i]))(k,l) = (*(state->Dspace()))[i];  /* get gridpoint */ 
                  } else
                     (*(_X[i]))(k,l) = 0;
               }
            }
            LAPACK::FullDiagHermitian(_X[i], &evals);
            
            for (int j = 0; j < States(); j++) {   /* Put Eigenvektor to states */
               double *d = _dspace->begin(j);
               d[i] = evals[j];
            }
         }
         _XT.SetMatrixArray(_X);
      } else if (_T != NULL) { /* Use external diagonalization, copy dspace */
         for (int s=0; s < States(); s++){
            ODSpace *state = dynamic_cast<ODSpace*>(State(s,s));
            _dspace->StrideCopy(*(state->Dspace()), 0, s);
         }
      }
      
   }
   
   void ODMultistate::InitExponential(cVec * exp, dcomplex c)
   {
      if (_T == NULL)
         ODSpace::InitExponential(exp, c);
      else {
         cVec tmpe(_state_size);
         for (int s=0; s < States(); s++){
            ODSpace *state = dynamic_cast<ODSpace*>(State(s,s));
            state->InitExponential(&tmpe, c);
            if (!exp->StrideCopy(tmpe, 0, s))
               throw ( EIncompatible ("Stride Error"));
         }
      }
   }
   
} /* namespace QDLIB */
