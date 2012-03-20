/**
 * Vector Operation: A = cexp(B * c) 
 */
inline void ExpElements (cVec* A, cVec* B, dcomplex c)
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
               (A->begin(s))[i] = cexp((B->begin(s))[i] * c);
            }
         }
      }
   }
