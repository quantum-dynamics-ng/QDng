#ifndef HELPERS_H
#define HELPERS_H

#include <string>


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
