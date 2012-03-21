namespace QDLIB {

	/**
	 * Vector Operation: C = A * (B * I) * d 
	 */
	inline void MultElementsComplexEq (cVec* C, cVec* A, dVec* B, double d)
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
			   m128dc _res = m128dc((A->begin(s))[i]).MultImag((B->begin(s))[i]) * d; _res.Store((C->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (C->begin(s))[i] = (A->begin(s))[i] * ((B->begin(s))[i] * I) * d;
			}
	#endif
		}
	}

}
