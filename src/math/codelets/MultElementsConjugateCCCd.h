namespace QDLIB {

	/**
	 * Vector Operation: C = A.conj() * B * d 
	 */
	inline void MultElementsConjugate (cVec* C, cVec* A, cVec* B, double d)
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
			   m256dc _res = m256dc((A->begin(s))[i]).conj() * m256dc((B->begin(s))[i]) * d; _res.Store((C->begin(s))[i]);
			}
	#elif HAVE_SSE2
			for (i=0; i < size; i++){
			   m128dc _res = m128dc((A->begin(s))[i]).conj() * m128dc((B->begin(s))[i]) * d; _res.Store((C->begin(s))[i]);
			}
	#else
			for (i=0; i < size; i++){
			   (C->begin(s))[i] = (A->begin(s))[i].conj() * (B->begin(s))[i] * d;
			}
	#endif
		}
	}

}

