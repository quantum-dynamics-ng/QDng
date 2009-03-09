#include "helpers.h"

namespace QDLIB {

   std::string& trim(std::string &s)
   {
      size_t pos;
	    
      pos = s.find_first_of(" \t");          // trim from left      
      if (pos == 0){
	 pos = s.find_first_not_of(" \t");
	 if (pos != s.npos) {
	    s.erase(0,pos);
	 }
      }
	 
      pos = s.find_last_of(" \t");          // trim from right
      if (pos == 0){                        // only whitespaces
	 s.clear();
	 return s;
      }
      if (pos == s.size()-1) {
	 pos = s.find_last_not_of(" \t");
	 if (pos != s.npos) {
	    s.erase(pos+1);
	 }
      }
	 
      return s;
   }

}


