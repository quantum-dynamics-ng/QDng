namespace QDLIB {

	/**
	 * Vector Operation: A =  B * C * d 
	 */
	inline void MultElementsCopy (cVec* A, cVec* B, dVec* C, double d)
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
	#ifdef HAVE_AVX
			for (i=0; i < size; i++){
			   m256dc _res =  m256dc((B->begin(s))[i]) * (C->begin(s))[i] * d; _res.Store((A->begin(s))[i]);
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   m128dc _res =  m128dc((B->begin(s))[i]) * (C->begin(s))[i] * d; _res.Store((A->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] =  (B->begin(s))[i] * (C->begin(s))[i] * d;
			}
	#endif
		}
	}

}

