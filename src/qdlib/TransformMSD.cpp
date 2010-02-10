#include "TransformMSD.h"

#include "WFMultistate.h"

namespace QDLIB
{

   TransformMSD::TransformMSD() : _X(NULL), _T(NULL)
   {

   }

   TransformMSD::~TransformMSD()
   {

   }

}

void QDLIB::TransformMSD::Forward(WaveFunction * Psi)
{
   WFMultistate *wfm = dynamic_cast<WFMultistate*>(Psi);
   int states;
   
   states = wfm->States();
   
   if (_T != NULL) { /* Use external transform */
      for (int i=0; i < states; i++)
         _T[i]->Forward(wfm->State(i));
      
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

void QDLIB::TransformMSD::Backward(WaveFunction * Psi)
{
   WFMultistate *wfm = dynamic_cast<WFMultistate*>(Psi);
   int states;
   
   states = wfm->States();
   
   if (_T != NULL) { /* Use external transform */
      for (int i=0; i < states; i++)
         if( _T[i] != NULL) _T[i]->Backward(wfm->State(i));
      
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

