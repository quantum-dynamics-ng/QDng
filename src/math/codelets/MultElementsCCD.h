/**
 * Vector Operation: A = A * B * C 
 */
inline void MultElements (cVec* A, cVec* B, dVec* C)
   {
      lint size = A->lsize();
      lint strides = A->strides();

      lint s;
      int rank = A->MPIrank();
      int msize = A->MPIsize();

      for (s=rank; s < strides; s +=msize ){
         lint i;
#ifdef _OPENMP
#pragma omp parallel for schedule(static) default(shared) private(i)
#endif
         for (i=0; i < size; i++){
            {
               (A->begin(s))[i] = (A->begin(s))[i] * (B->begin(s))[i] * (C->begin(s))[i];
            }
         }
      }
   }
