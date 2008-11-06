#include "qdlib/WFGrid1D.h"
#include "qdlib/OGrid1Dd2dx.h"
#include "qdlib/OGridPotential.h"
#include "qdlib/OSPO.h"
#include "sys/FileSingleDefs.h"
#include "sys/Exception.h"

#include "sys/QDGlobalClock.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{
   WFGrid1D  Psi,Psi2,LC;
   WaveFunction *Tket, *Vket;

   OGridPotential pot;
   OGrid1Dd2dx    d2dx;
   
   FileWF     fileWF;
   FileOGrid  filePot;
   
   OSPO U;
   
   QDClock *clock;
   
   try {
      fileWF.Name(argv[1]);
      fileWF.Suffix(BINARY_WF_SUFFIX);
      fileWF >> (WaveFunction*) &Psi;
      fileWF.Name("WFBeigenstate_1");
      fileWF >> (WaveFunction*) &Psi2;
      LC = Psi + &Psi2;
      cout << "Reading WF done..\n";
//        cout << Psi;
      Psi.Params();
      filePot.Name(argv[2]);
      filePot.Suffix(BINARY_O_SUFFIX);
      filePot >> (OGridSystem*) &pot;
      cout << "Reading Potential done..\n";
//        cout << pot;
      
      cout << "Norm: " << Psi.Norm() << endl;
      
      d2dx.Mass(1728.400000);
      cout << "Mass : " << d2dx.Mass() << endl;
      
      cout << "Kinetic energy: " << d2dx.Expec( &Psi ) << endl;
      
      cout << "Setup clock:\n";
      
      clock = QDGlobalClock::Instance();
      clock->Dt(5.0);
      clock->Steps(10000);
      
      cout << "Nt = " << clock->Steps() << ", dt = " << clock->Dt() << endl;
      
      U.Clock(clock);
      U.AddTkin( &d2dx );
      U.AddVpot( &pot );
      
      
      
      U.Forward();
      U.ReInit();
      
      cout << "SPO init done:\n";
      
      fileWF.ActivateSequence();
      fileWF.ResetCounter();
      fileWF.Name("WF");
      fileWF.Suffix("");
      cout << "Norm: " << LC.Norm() << endl;
      for (int i=0; i<clock->Steps(); i++){
         U *= &LC;
 	 cout << "Norm: " << LC.Norm() << endl;
// 	 fileWF << &LC;
      }
      
   } catch ( Exception e) {
      cout << "Exception: " << e.GetMessage() << endl;
   }
   
   
   
}
