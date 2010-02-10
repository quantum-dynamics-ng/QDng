#include "ODMultistate.h"
#include "linalg/LapackDiag.h"

namespace QDLIB
{

   ODMultistate::ODMultistate() :
         _name("ODMultistate"), _T(NULL),  _state_size(0)
   {

   }

   ODMultistate::~ODMultistate()
   {
      for (int i = 0; i < _state_size; i++) {
         delete _X[i];
      }
      if (_T != NULL)  delete[] _T;
   }

   Operator * ODMultistate::NewInstance()
   {
      ODMultistate* r = new ODMultistate();
      
      return (OMultistate*) r;
   }
   
   void ODMultistate::Init(WaveFunction *Psi)
   {
      OMultistate::Init(Psi);
      
      WFMultistate* wfm = dynamic_cast<WFMultistate*>(Psi);
      
      lint last_size = wfm->State(0)->size();
      for (int i=1; i < States(); i++){
         if (last_size != wfm->State(0)->size())
            throw( EIncompatible("ODMultistate relies on states with same size") );
      }
      _state_size = last_size;
      
      bool external=false;
      _T = new Transform*[States()];
      
      
      /* Check for transform type & validty of matrix elements */
      for (int i=0; i < States(); i++){
         for (int j=0; j < States(); j++){
            ODSpace *od =  dynamic_cast<ODSpace*>(State(i,j));
            if ( od == NULL && State(i,j) != NULL)
               throw ( EIncompatible ("ODMultistate can only handle DSpace operators") );
            
            if ( State(i,j) != NULL ) {
               if ( od->Transformation() != NULL ) {
                  external = true;
                  if ( i != j )
                     throw ( EIncompatible ("ODMultistate can't transform non-diagonal operators in non-diagonal matrices") );
                  
                  _T[i] = od->Transformation();
               }
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
      InitDspace();
      
      return (OMultistate*) this;
   }

   Operator * ODMultistate::operator *(Operator * O)
   {
      return OMultistate::operator*(O);
   }

   
   void ODMultistate::InitDspace()
   {
      
      /* First init */
      if (_dspace == NULL) {
         _dspace = new dVec();
         _dspace->newsize(_state_size * States(), States());
         _X = new dMat*[_state_size];
         for (int i = 0; i < _state_size; i++) {
            _X[i] = new dMat(States(), States());
         }
      }
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
   }
   

   
} /* namespace QDLIB */




