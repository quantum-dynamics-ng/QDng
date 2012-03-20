/*
 * m128dc.h
 *
 *  Created on: 24.10.2011
 *      Author: markus
 */

#ifndef M128DC_H_
#define M128DC_H_

#include "dcomplex.h"

#ifdef HAVE_SSE2
#include <emmintrin.h>

#ifdef HAVE_SSE3
 #include <pmmintrin.h>
#endif



namespace QDLIB
{


   /**
    * SSE2/3 representation of a twin double
    */
   struct m128dd
   {
      __m128d v;  /* a,a */

      /** Nothing is set! */
      m128dd() {}

      /** Initializer with native type. */
      m128dd(const __m128d &a) { v = a; }

      /** initializer with designated type */
      m128dd(const double a)
      {
         v = _mm_load1_pd( (double*) &a );
      }

      /** Load. */
      inline void operator=(const double &a)
      {
         v = _mm_load1_pd( (double*) &a );
      }

   };

   /**
    * SSE2/3 representation of dcomplex
    */
   struct m128dc
   {
      __m128d v;        /* re,im */

      /** Nothing is set! */
      m128dc() {}

      /** Initializer with native type. */
      m128dc(const __m128d &a) { v = a; }

      /** length of vector **/
      static uint len() { return 1; }

      m128dc(const dcomplex &a)
      {
         v = _mm_load_pd( (double*) &a );
      }

      /** Load. */
      inline void operator=(const dcomplex &a)
      {
         v = _mm_load_pd( (double*) &a );
      }

      /** Store. */
      inline void Store( dcomplex &a ) const
      {
         _mm_store_pd( (double*) &a, v );
      }


      inline operator dcomplex() const
      {
         dcomplex ret;
         _mm_store_pd( (double*) &ret, v );
         return ret;
      }


      /**
       * Complex multiplication.
       */
      inline m128dc operator*(const m128dc &b)
      {
         __m128d re, im;
         static const __m128d SIGNMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0x80000000,0,0,0));

         re = _mm_mul_pd(v, b.v);       /* a.re * b.re, a.im * a.im */
         re = _mm_xor_pd(re, SIGNMASK128);   /* a.re * b.re, -(a.im * a.im) */

         im = _mm_shuffle_pd(b.v, b.v, 0x1);     /* swap re,im */
         im = _mm_mul_pd(v,im);                /* a.re * b.im, a.re * a.im */

#ifdef HAVE_SSE3
         return m128dc(_mm_hadd_pd(re,im));       /* a.re * b.re -(a.im * a.im) ; a.re * b.im + a.re * a.im */
#else
         __m128d rei, imi;
         rei = _mm_unpacklo_pd(re,im);
         imi = _mm_unpackhi_pd(re,im);

         return m128dc( _mm_add_pd(rei, imi) );
#endif
      }

      /**
       * Complex multiplication.
       */
      inline void operator*=(const m128dc &a)
      {
         *this = (*this) * a;
      }

      /**
       * Complex * I * real
       */
      inline m128dc MulImag(const m128dd &a)
      {
         __m128d res;
         static const __m128d SIGNMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0x80000000,0,0,0));

         res = _mm_xor_pd(v, SIGNMASK128);
         res = _mm_mul_pd(res, a.v);
         res = _mm_shuffle_pd(res, res, 0x1);

         return m128dc(res);
      }

      /**
       * Scalar multiplication
       */
      inline m128dc operator*(const m128dd &a)
      {
         return m128dc( _mm_mul_pd(v, a.v) );
      }

      /**
       * Scalar multiplication
       */
      inline m128dc operator*(const double a)
      {
         return m128dc( _mm_mul_pd(v, _mm_load1_pd( (double*) &a ).v) );
      }


      /**
       * Scalar multiplication
       */
      inline void operator*=(const m128dd &a)
      {
         v = _mm_mul_pd(v, a.v);
      }

      /**
       * Scalar multiplication
       */
      inline void operator*=(const double a)
      {
         v = _mm_mul_pd(v, _mm_load1_pd( (double*) &a ).v);
      }

      /**
       * Addition
       */
      inline m128dc operator+(const m128dc &a)
      {
         return m128dc( _mm_add_pd(v, a.v) );
      }

      /**
       * Addition
       */
      inline void operator+=(const m128dc &a)
      {
         v = _mm_add_pd(v, a.v);
      }


      /**
       * Subtraction
       */
      inline m128dc operator-(const m128dc &a)
      {
         return m128dc( _mm_sub_pd(v, a.v) );
      }


      /**
       * Subtraction
       */
      inline void operator-=(const m128dc &a)
      {
         v = _mm_sub_pd(v, a.v) ;
      }

   };
}


#endif /* HAVE_SSE3 */
#endif /* M128DC_H_ */
