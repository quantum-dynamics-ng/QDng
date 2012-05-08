namespace QDLIB {

	/**
	 * Vector Operation: A = cexp( B * d ) 
	 */
	inline void ExpElements (cVec* A, dVec* B, dcomplex d)
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
			   m256dc _res = cexp( (B->begin(s))[i] * m256dc(d)  ); _res.Store((A->begin(s))[i]);
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   m128dc _res = cexp( (B->begin(s))[i] * m128dc(d)  ); _res.Store((A->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] = cexp( (B->begin(s))[i] * d );
			}
	#endif
		}
	}

}

