#include "WaveFunction.h"

namespace QDLIB {
   
   WaveFunction* DirectProduct(WaveFunction* PsiA, WaveFunction* PsiB)
   {
      WaveFunction *wf;
         
      wf = PsiA->NewInstance();
               
      if (PsiB->size() != PsiA->size()) throw ( EParamProblem("WFs differ in size") );
               
      MultElementsCopy((cVec*) wf, (cVec*) PsiA, (cVec*) PsiB);
      return wf;
   }
   
   WaveFunction* DirectProductComplex(WaveFunction* PsiA, WaveFunction* PsiB)
   {
      WaveFunction *wf;
         
      wf = PsiA->NewInstance();
               
      if (PsiB->size() != PsiA->size()) throw ( EParamProblem("WFs differ in size") );
               
       MultElementsComplex((cVec*) wf, (cVec*) PsiA, (cVec*) PsiB, (double) 1.0);
      return wf;
   }

}