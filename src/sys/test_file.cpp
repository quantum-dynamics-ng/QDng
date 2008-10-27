#include "qdlib/WFLevel.h"
#include "FileSingle.h"


namespace QDLIB {
   typedef FileSingle<WaveFunction> FileSingleWF;
   
   void f(){
      FileSingleWF file;
      
      
   }
}

using namespace QDLIB;
int main()
{
   
   FileSingleWF file;
   WFLevel WF;
   
   WF.SetSize(3);
   
   WF[0] = 1;
   WF[2] = 0;
   WF[3] = 0;
   
   file.Name("test");
   
   file << &WF;
}

