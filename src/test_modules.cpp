#include "modules/ModuleLoader.h"
#include "sys/FileSingleDefs.h"
#include "sys/Exception.h"

using namespace std;
using namespace QDLIB;


int main(int argc, char **argv)
{
   ModuleLoader mod;
   WaveFunction* Psi;
   FileWF     fileWF;
   string name;
   
   name = "";
   try {
      mod.UserPath( name );
      Psi = mod.LoadWF("/home/mko/src/QDng/devel-main/debug/src/modules/.libs/modwfgrid");
      cout << "Loaded module: " << Psi->Name() << endl;
      
      fileWF.Suffix(BINARY_WF_SUFFIX);
      fileWF.Name("WFBeigenstate_0");
      cout << "Ready to read: " << endl;
      fileWF >> Psi;
      cout << "Read success: " << endl;
      
      cout << "Norm: " << Psi->Norm() << endl;
   } catch ( Exception e) {
      cerr <<  e.GetMessage() << endl;
   }
}

