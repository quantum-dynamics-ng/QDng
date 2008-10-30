#include "qdlib/WFGrid1D.h"
#include "qdlib/OGrid1Dd2dx.h"

#include "sys/FileSingleDefs.h"
#include "qdlib/OGridPotential.h"
#include "qdlib/WFGrid1D.h"
#include "sys/FileSingleDefs.h"
#include "sys/Exception.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{
   WFGrid1D  Psi, Mix;
   WaveFunction *Tket, *Vket;

   OGridPotential pot;
   OGrid1Dd2dx    d2dx;
   
   FileWF     fileWF;
   FileOGrid  filePot;
   
   try {
      fileWF.Name(argv[1]);
      fileWF.Suffix(BINARY_WF_SUFFIX);
      fileWF >> (WaveFunction*) &Psi;
      cout << "Reading WF done..\n";
//        cout << Psi;
      
      filePot.Name(argv[2]);
      filePot.Suffix(BINARY_O_SUFFIX);
      filePot >> (OGridSystem*) &pot;
      cout << "Reading Potential done..\n";
//        cout << pot;
      
      cout << "Norm: " << Psi.Norm() << endl;
      
      d2dx.Mass(1728.400000);
      cout << "Mass : " << d2dx.Mass() << endl;
      
      Tket = d2dx * &Psi;
      Vket = pot * &Psi;
      
      cout << "Psi*T*Psi:" << (Psi * Tket)  << endl;
       
       cout << "Psi*V*Psi:" << (Psi * Vket)  << endl;
       cout << "Psi*H*Psi:" << ( Psi * ( *(pot * &Psi) + (d2dx * &Psi) ) )  << endl;
       
       Mix = Psi;
       Mix.ToKspace();
       Mix.ToXspace();
       cout << "Norm: " << (Mix * &Psi)  << endl;
       
      
      
   } catch ( Exception e) {
      cout << "Exception: " << e.GetMessage() << endl;
   }
   
   
   
}
