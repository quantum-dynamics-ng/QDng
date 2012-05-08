namespace QDLIB {

	/**
	 * Vector Operation: B = cabs(A) 
	 */
	inline void Abs (dVec* B, cVec* A)
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
			   m128dc _res = cabs(m256dc((A->begin(s))[i])); _res.Store((B->begin(s))[i]);
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   double _res = cabs(m128dc((A->begin(s))[i])); (B->begin(s))[i] = _res;
			}
	#else
			for (i=0; i < size; i++){
			   (B->begin(s))[i] = cabs((A->begin(s))[i]);
			}
	#endif
		}
	}

}

