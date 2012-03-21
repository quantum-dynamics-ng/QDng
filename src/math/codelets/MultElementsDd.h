namespace QDLIB {

	/**
	 * Vector Operation: A = A * d 
	 */
	inline void MultElements (dVec* A, double d)
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
			   double _res = (A->begin(s))[i] * d; (A->begin(s))[i] = _res;
			}
	#else
			for (i=0; i < size; i++){
			   (A->begin(s))[i] = (A->begin(s))[i] * d;
			}
	#endif
		}
	}

}
