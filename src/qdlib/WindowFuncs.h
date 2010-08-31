#ifndef QDLIBWINDOWFUNCS_H
#define QDLIBWINDOWFUNCS_H

namespace QDLIB
{
   /**
    * This class provides some typical window functions.
    * The Window function is always applied to vec.
    */
   template <class V>
   class WindowFuncs {
      public:
         static void Hann(V &vec);
   };
   
   
   /**
    * Hann window.
    *
    * \f$ w(n) = 0.5 \left( 1-cos(\frac{2\pi n}{N-1}) \right) \f$
    */
   template <class V>
   void WindowFuncs<V>::Hann(V &vec)
   {
      int size = vec.size();
      for (int n=0; n < size; n++)
         vec[n] *= 0.5 * (1. - cos( 2*M_PI*double(n) / double(size-1) ));
   }
}


#endif
