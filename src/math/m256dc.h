/*
 * m256dc.h
 *
 *  Created on: 24.10.2011
 *      Author: markus
 */

#ifndef M256DC_H_
#define M256DC_H_

#ifdef HAVE_AVX
#include <immintrin.h>

namespace QDLIB
{
   /**
    * AVX representation of a quad double
    */
   struct m256dd
   {
      __m256d v;  /* a,a,a,a */

      /** Nothing is set! */
      m256dd() {}

      /** Initializer with native type. */
      m256dd(const __m256d &a) { v = a; }

      /** Load. */
      inline void operator=(const dcomplex &a)
      {
         v = _mm256_broadcast_sd((double*) &a);
      }

   };


   /**
    * SSE2/3 representation of dcomplex
    */
   struct m256dc
   {
      __m256d v;        /* re,im */

      /** Nothing is set! */
      m256dc() {}

      /** Initializer with native type. */
      m256dc(const __m256d &a) { v = a; }

      /**
       * Init with vector.
       */
      m256dc(const dcomplex *a)
       {
          v = _mm256_load_pd( (double*) a );
       }

      /**
       * Init with vector.
       */
      m256dc(const double *a)
       {
          v = _mm256_broadcast_pd( (__m128d*) a );
       }

      /**
       * Init with scalar.
       */
      m256dc(const dcomplex &a)
       {
          v = _mm256_broadcast_pd( (__m128d*) &a );
       }

      /**
       * Init with scalar.
       */
      m256dc(const double &a)
       {
          v = _mm256_broadcast_sd( (double*) &a );
       }

      /** length of vector in units of elements **/
      static uint len() { return 2; }

      /** Load. */
      inline void operator=(const dcomplex *a)
      {
         v = _mm256_load_pd( (double*) *a );
      }

      /** Store. */
      inline void Store( dcomplex& a )
      {
         _mm256_store_pd( (double*) &a, v );
      }

      /**
       * Complex multiplication.
       */
      inline m256dc operator*(const m256dc &b)
      {
         __m256d re, im;
         static const __m256d SIGNMASK256 =
                       _mm256_castsi256_pd(_mm256_set_epi32(0x80000000,0,0,0,0x80000000,0,0,0));

         re = _mm256_mul_pd(v, b.v);       /* a.re * b.re, a.im * a.im */
         re = _mm256_xor_pd(re, SIGNMASK256);   /* a.re * b.re, -(a.im * a.im) */

         im = _mm256_permute_pd(b.v, 0x5);     /* swap re,im */
         im = _mm256_mul_pd(v,im);                /* a.re * b.im, a.re * a.im */


         return m256dc(_mm256_hadd_pd(re,im));       /* a.re * b.re -(a.im * a.im) ; a.re * b.im + a.re * a.im */
      }

      /**
       * Complex multiplication.
       */
      inline void operator*=(const m256dc &b)
      {
         (*this) = (*this) * b;
      }

      /**
       * Complex * I * real
       */
      inline m256dc MultImag(const double &a) const
      {
         __m256d res;
         __m256d sd = _mm256_broadcast_sd( (double*) &a );
         static const __m256d SIGNMASK256 =
                       _mm256_castsi256_pd(_mm256_set_epi32(0x80000000,0,0,0,0x80000000,0,0,0));

         res = _mm256_xor_pd(v, SIGNMASK256);
         res = _mm256_mul_pd(res, sd);
         res = _mm256_shuffle_pd(res, res, 0x1);

         return m256dc(res);
      }

      inline m256dc conj()
      {
         static const __m256d SIGNMASK256 =
                  _mm256_castsi256_pd(_mm256_set_epi32(0x80000000,0,0,0,0x80000000,0,0,0));

         return m256dc(_mm256_xor_pd(v, SIGNMASK256));
      }

      inline m256dc imag()
      {
         static const __m256d IMAGMASK256 =
                       _mm256_castsi256_pd(_mm256_set_epi32(0xFFFFFFFF,0xFFFFFFFF,0,0,0xFFFFFFFF,0xFFFFFFFF,0,0));

         return m256dc( _mm256_and_pd(v, IMAGMASK256) );
      }

      inline m256dc real()
      {
         static const __m256d REALMASK256=
                       _mm256_castsi256_pd(_mm256_set_epi32(0,0,0xFFFFFFFF,0xFFFFFFFF,0,0,0xFFFFFFFF,0xFFFFFFFF));

         return m256dc( _mm256_and_pd(v, REALMASK256) );
      }


      /**
       * Scalar multiplication
       */
      inline m256dc operator*(const double &a)
      {
         return m256dc( _mm256_mul_pd(v, _mm256_broadcast_sd((double*) &a)) );
      }

      /**
       * Addition
       */
      inline void operator+=(const m256dc &a)
      {
         v =  _mm256_add_pd(v, a.v) ;
      }

      /**
       * Addition
       */
      inline m256dc operator+(const m256dc &a)
      {
         return m256dc( _mm256_add_pd(v, a.v) );
      }

      /**
       * Subtraction
       */
      inline void operator-=(const m256dc &a)
      {
         v =  _mm256_sub_pd(v, a.v);
      }


      /**
       * Subtraction
       */
      inline m256dc operator-(const m256dc &a)
      {
         return m256dc( _mm256_sub_pd(v, a.v) );
      }

   };

}


#endif /* HAVE_AVX */
#endif /* M256DC_H_ */
