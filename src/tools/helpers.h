#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <iomanip>

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
   
   /**
    * Removes whitespaces from the string.
    * 
    * Spaces and Tabulators are removed from the left and
    * the right of string. Whitespace in between of the characters
    * are conserved.
    * 
    */
   extern std::string& trim(std::string &s);

} /* namespace QDLIB */

#endif /* #ifndef HELPERS_H */
