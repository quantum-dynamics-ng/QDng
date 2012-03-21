namespace QDLIB {

	/**
	 * Vector Operation: A = B * C 
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
	#ifdef HAVE_SSE2
			for (i=0; i < size; i++){
			   m128dc _res = m128dc((B->begin(s))[i]) * (C->begin(s))[i]; _res.Store((A->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] = (B->begin(s))[i] * (C->begin(s))[i];
			}
	#endif
		}
	}

}
