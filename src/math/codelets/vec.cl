namespace QDLIB {

	/**
	 * Vector Operation: @DESC@ 
	 */
	inline void @NAME@ (@PARAMS@)
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
			   @AVX_EXP@
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   @SSE_EXP@
			}
	#else
			for (i=0; i < size; i++){
			   @EXP@
			}
	#endif
		}
	}

}
