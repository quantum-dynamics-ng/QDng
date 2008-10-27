#ifndef KEYVALFILE_H
#define KEYVALFILE_H

#include "paramcontainer.h"

#define MAX_LINE_LENGTH 2048

namespace QDLIB {
   
   /**
    * This class parses an ASCII file which consists of Key = Value pairs.
    * 
    * The pairs found in the file will be stored in a ParamContainer.
    * Lines not containing "=" or don't have key are ignored.
    * 
    * \todo Support for sections.
    */
   class KeyValFile {
      private:
	 string _name;
	 size_t _pos;    // Actual line
      public:
	 KeyValFile();
	 KeyValFile(const string name);
	 
	 bool Parse(ParamContainer &p);
	 bool Write(ParamContainer &p);
	 
	 void SetName(const string name);
	 size_t GetPos();
   };
   
} /* namespace QDLIB */

#endif /* KEYVALFILE_H */
