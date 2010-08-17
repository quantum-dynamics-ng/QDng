#ifndef CPLX_SSE2_H
#define CPLX_SSE2_H
#include <emmintrin.h>
#include "dcomplex.h"


using QDLIB::dcomplex;

namespace QDSSE {

    /**
     * Packed complex format for SSE2.
     */

    struct cplx_sse_packed {
        __m128d real;
        __m128d imag;
    };

    /**
     * Load two complex numbers into packed format
     */
    inline void LoadPacked(cplx_sse_packed& c,  const dcomplex& a1, const dcomplex& a2)
    {
        c.real = _mm_set_pd(a1._real, a2._real);
        c.imag = _mm_set_pd(a1._imag, a2._imag);
    }

    /**
     * Shuffle into unpacked format.
     */
    inline void UnPack(__m128d& c1, __m128d& c2, const cplx_sse_packed& a)
    {
        c1 = _mm_shuffle_pd(a.real, a.imag, _MM_SHUFFLE2(1, 1));
        c2 = _mm_shuffle_pd(a.real, a.imag, _MM_SHUFFLE2(0, 0));
    }


    /**
     * Store SSE registers back to memory.
     */
    inline void Store(dcomplex& a1, dcomplex& a2, const __m128d& c1, const __m128d& c2)
    {
        _mm_store_pd((double*) &a1, c1);
        _mm_store_pd((double*) &a2, c2);
    }

    /**
     * Store SSE registers back to memory.
     */
    inline void Store(dcomplex& a, const __m128d& c)
    {
        _mm_store_pd((double*) &a, c);
    }

    /**
     * Multiply packed complex numbers,
     */
    inline cplx_sse_packed MulPacked(const cplx_sse_packed& c1, const cplx_sse_packed& c2)
    {
        cplx_sse_packed c;

        c.real = _mm_sub_pd(_mm_mul_pd(c1.real, c2.real), _mm_mul_pd(c1.imag, c2.imag));
        c.imag = _mm_add_pd(_mm_mul_pd(c1.real, c2.imag), _mm_mul_pd(c1.imag, c2.real));

        return c;
    }

    /**
     * Multiply packed complex numbers,
     */
    inline cplx_sse_packed MulPacked(const cplx_sse_packed& c1, const __m128d &d)
    {
       cplx_sse_packed c;

       c.real = _mm_mul_pd(c1.real, d);
       c.imag = _mm_mul_pd(c1.imag, d);
       
       return c;
    }
    
    /**
    * Add packed complex numbers,
    */
    inline cplx_sse_packed AddPacked(const cplx_sse_packed& c1, const cplx_sse_packed& c2)
    {
        cplx_sse_packed c;

        c.real = _mm_add_pd(c1.real, c2.real);
        c.imag = _mm_add_pd(c1.imag, c2.imag);

        return c;
    }

    /**
     * Add packed complex numbers,
     */
    inline cplx_sse_packed SubPacked(const cplx_sse_packed& c1, const cplx_sse_packed& c2)
    {
       cplx_sse_packed c;

       c.real = _mm_sub_pd(c1.real, c2.real);
       c.imag = _mm_sub_pd(c1.imag, c2.imag);

       return c;
    }

    /**
     * Vectorized, complex a *= b;
     */
    inline void MultEq(dcomplex& a1, dcomplex& a2, const dcomplex& b1, const dcomplex& b2)
    {
        cplx_sse_packed c1, c2, c;
        __m128d r1, r2;

        LoadPacked(c1, a1, a2);
        LoadPacked(c2, b1, b2);

        c = MulPacked(c1, c2);

        UnPack(r1, r2, c);

        Store(a1, a2, r1, r2);
    }

}
#endif