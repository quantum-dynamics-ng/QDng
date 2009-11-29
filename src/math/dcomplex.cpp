#include "dcomplex.h"


namespace QDLIB {
   
   /**Gloabal instance! */
   Imaginary I;
   
   /** output to stream in a+ib format. */
   std::ostream& operator<<(std::ostream &s, const dcomplex c)
   {
     s << c._real << ",";
     s  << c._imag << "i";
     return s;
   }

   
   void dcomplex::operator =(Imaginary i)
   {
      _real = 0;
      _imag = 1;
   }

   
} /* namespace QDLIB */ 


