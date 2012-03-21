namespace QDLIB {

	/**
	 * Vector Operation: A = cexp( B * d ) 
	 */
	inline void ExpElements (cVec* A, cVec* B, dcomplex d)
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
			   m128dc _res = cexp( m128dc((B->begin(s))[i]) * m128dc(d)  ); _res.Store((A->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] = cexp( (B->begin(s))[i] * d );
			}
	#endif
		}
	}

}
