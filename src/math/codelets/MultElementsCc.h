namespace QDLIB {

	/**
	 * Vector Operation: A = A * d 
	 */
	inline void MultElements (cVec* A, dcomplex d)
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
			   m256dc _res = m256dc((A->begin(s))[i]) * m256dc(d) ; _res.Store((A->begin(s))[i]);
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   m128dc _res = m128dc((A->begin(s))[i]) * m128dc(d) ; _res.Store((A->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] = (A->begin(s))[i] * d;
			}
	#endif
		}
	}

}

