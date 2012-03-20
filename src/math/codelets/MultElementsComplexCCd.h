/**
 * Vector Operation: A = (A * I) * B * d; 
 */
inline void MultElementsComplex (cVec* A, cVec* B, double d)
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
               (A->begin(s))[i] = ((A->begin(s))[i] * I) * (B->begin(s))[i] * d;;
            }
         }
      }
   }