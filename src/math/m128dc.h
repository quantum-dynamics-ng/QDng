/*
 * m128dc.h
 *
 *  Created on: 24.10.2011
 *      Author: markus
 */

#ifndef M128DC_H_
#define M128DC_H_

#include <iostream>

#ifdef HAVE_SSE2
#include <emmintrin.h>

#ifdef HAVE_SSE3
 #include <pmmintrin.h>
#endif


#include "dcomplex.h"

namespace QDLIB
{

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

      m128dc(const dcomplex* a)
      {
         v = ((__m128d*) a);
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
      inline m128dc operator*(const m128dc &b) const
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
      inline m128dc MultImag(const double &a) const
      {
         __m128d res;
         __m128d sd = _mm_load1_pd( (double*) &a );
         static const __m128d SIGNMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0x80000000,0,0,0));

         res = _mm_xor_pd(v, SIGNMASK128);
         res = _mm_mul_pd(res, sd);
         res = _mm_shuffle_pd(res, res, 0x1);

         return m128dc(res);
      }

      inline m128dc conj()
      {
         static const __m128d SIGNMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0x80000000,0,0,0));

         return m128dc(_mm_xor_pd(v, SIGNMASK128));
      }

      inline m128dc imag()
      {
         static const __m128d IMAGMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0xFFFFFFFF,0xFFFFFFFF,0,0));

         return m128dc( _mm_and_pd(v, IMAGMASK128) );
      }

      inline m128dc real()
      {
         static const __m128d REALMASK128 =
                       _mm_castsi128_pd(_mm_set_epi32(0,0,0xFFFFFFFF,0xFFFFFFFF));

         return m128dc( _mm_and_pd(v, REALMASK128) );
      }


      /* Return imag as scalar */
      inline double imags()
      {
         double res;
        _mm_store_sd( (double*) &res, v);
        return res;
      }

      /* Return real as scalar */
      inline double reals()
      {
         double res;
        _mm_store_sd( (double*) &res, v);
        return res;
      }

      /**
       * Scalar multiplication
       */
      inline m128dc operator*(const double& a) const
      {
         return m128dc( _mm_mul_pd(v, _mm_load1_pd( (double*) &a )) );
      }


      /**
       * Scalar multiplication
       */
      inline void operator*=(const double a)
      {
         v = _mm_mul_pd(v, _mm_load1_pd( (double*) &a ));
      }

      /**
       * Addition
       */
      inline m128dc operator+(const m128dc &a) const
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
      inline m128dc operator-(const m128dc &a) const
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


   inline std::ostream& operator<<(std::ostream& s, m128dc dc)
   {
      dcomplex c;
      dc.Store(c);
      s << c << std::endl;
      return s;
   }

}


#endif /* HAVE_SSE3 */
#endif /* M128DC_H_ */
