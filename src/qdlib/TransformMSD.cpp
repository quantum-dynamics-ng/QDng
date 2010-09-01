#include "TransformMSD.h"

#include "WFMultistate.h"

namespace QDLIB
{

   TransformMSD::TransformMSD() : _X(NULL), _T(NULL), _norm(1)
   {

   }

   TransformMSD::~TransformMSD()
   {

   }

   double TransformMSD::Normalization()
   {
      if (_T != NULL) return _T[0]->Normalization();
      else return _norm;
   }
   
}

/** \bug Result is not saved in k-space (stays in real-space buffer) */
void QDLIB::TransformMSD::Forward(WaveFunction * Psi)
{
   WFMultistate *wfm = dynamic_cast<WFMultistate*>(Psi);
   int states;
   
   states = wfm->States();
   
   if (_T != NULL) { /* Use external transform */
      for (int i=0; i < states; i++){
         if( _T[i] != NULL) {
            _T[i]->Forward(wfm->State(i));
            _norm = _T[i]->Normalization();
         }
      }
      wfm->IsKspace(true);
   } else {
      cVec psiI,psiR;
   
      psiI.newsize(states);
      psiR.newsize(states);
      
      for (int i=0; i < wfm->State(0)->size(); i++){
         for (int s=0; s < states; s++){
            dcomplex *d = wfm->begin(s);
            psiI[s] = d[i];
         }
         MatVecMult(&psiR, _X[i], &psiI, true);
         for (int s=0; s < states; s++){
            dcomplex *d = wfm->begin(s);
            d[i] = psiR[s];
         }
      }
   }
}

/** \bug Input is not taken from k-space (from real-space buffer) */
void QDLIB::TransformMSD::Backward(WaveFunction * Psi)
{
   WFMultistate *wfm = dynamic_cast<WFMultistate*>(Psi);
   int states;
   
   states = wfm->States();
   
   if (_T != NULL) { /* Use external transform */
      for (int i=0; i < states; i++){
         if( _T[i] != NULL) {
            _T[i]->Backward(wfm->State(i));
            _norm = _T[i]->Normalization();
         }
      }
      wfm->IsKspace(false);
   } else {
      cVec psiI,psiR;
   
      psiI.newsize(states);
      psiR.newsize(states);
      
      for (int i=0; i < wfm->State(0)->size(); i++){
         for (int s=0; s < states; s++){
            dcomplex *d = wfm->begin(s);
            psiI[s] = d[i];
         }
         MatVecMult(&psiR, _X[i], &psiI, false);
         for (int s=0; s < states; s++){
            dcomplex *d = wfm->begin(s);
            d[i] = psiR[s];
         }
      }
   }
}


