#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <iomanip>
#include "math/typedefs.h"

#define printbuffer(BUFFER,SIZE) \
   unsigned char* buffercast = (unsigned char*) BUFFER; \
   unsigned int sz = SIZE; \
   for (unsigned int loopcounter=0; loopcounter < (sz/16)*16; loopcounter+=16){ \
      for (int ill=0; ill < 16; ill++) { \
    	  cout << setw(2) << setfill('0') << hex << static_cast<int>(buffercast[loopcounter+ill]) << " "; \
      }\
      cout << "     ";\
      for (int ill=0; ill < 16; ill++) { \
         if (buffercast[loopcounter+ill] > 32) \
           cout << buffercast[loopcounter+ill]; \
         else \
           cout << '.'; \
      }\
      cout << endl;\
   } \
   for (unsigned int ill=(sz/16)*16; ill < sz; ill++) { \
     cout << setw(2) << setfill('0') << hex << static_cast<int>(buffercast[ill]) << " "; \
   }\
   cout << "     ";\
   for (unsigned int ill=(sz/16)*16; ill < sz; ill++) { \
      if (buffercast[ill] > 32) \
        cout << buffercast[ill]; \
      else \
        cout << '.'; \
   }\
   cout << dec; \
   cout << endl;


namespace QDLIB {
   
   using namespace std;

   /**
    * Removes whitespaces from the string.
    * 
    * Spaces and Tabulators are removed from the left and
    * the right of string. Whitespace in between of the characters
    * are conserved.
    * 
    */
   extern string& trim(string &s);

   extern void split(const string &s, vector<string>& array);

   /**
    * Convert and std vector into an raw C-array.
    *
    * \param vec Input array.
    * \param buf Output array.
    * \param len Size of buf
    */
   template <typename T>
   void ConvertArray(const vector<T> &vec, T* buf, size_t len)
   {
      for (size_t i=0; i < vec.size() && i < len; i++)
         buf[i] = vec[i];
   }

   /**
    * Convert and std vector into an QDLIB Vector.
    *
    * \param vec Input array.
    * \param buf Output array.
    */
   template <typename T>
   void ConvertArray(const std::vector<T> &vec, QDLIB::Vector<T>& buf)
   {
      buf.newsize(vec.size());
      for (size_t i=0; i < vec.size(); i++)
         buf[i] = vec[i];
   }


   /**
    * Reorder a linear sequenced lower triangular matrix into 2D matrix array.
    *
    * No boundary check is done!
    *
    * \param sym treat matrix as symmetric.
    */
   template <class V, class M>
   void ReorderLowerTriangular(V &vec, M &mat, bool sym = false)
   {
      int r = 0;
      int clen = 1;
      int c = 0;

      for (uint i = 0; i < vec.size(); i++) {
         mat(r, c) = vec[i];
         if (sym) mat(c, r) = vec[i];
         c++;
         if (c == clen) {
            r++;
            clen++;
            c = 0;
         }
      }
   }

} /* namespace QDLIB */

#endif /* #ifndef HELPERS_H */
