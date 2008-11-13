#include "qdlib/OGridNablaSq.h"
#include "qdlib/WFGridCartesian.h"
#include "qdlib/OGridPotential.h"
#include "sys/FileSingleDefs.h"
#include "qdlib/OSOD.h"
#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"

using namespace std;
using namespace QDLIB;


int main(int argc, char **argv)
{
   WaveFunction *Psi;
   Operator *Delta1;
   FileWF fileWf;
   ParamContainer params;
   
   OSum Ham;
   
   OGridSystem *pot;
   
   OGridSum V;
   OSOD U;
   
   QDClock clock;
   
   Psi = new WFGridCartesian();
   Delta1 = new OGridNablaSq();
   pot = new OGridPotential();
   
   try {
      params.SetValue("dims",  1);
      params.SetValue("mass0",  1728.400000);
	    
   /*   params.SetValue("mass0", 1.5979);
      params.SetValue("mass1", 1.0645);*/
   //   params.SetValue("mass1", 1.0645);
      Delta1->Init( params );
      
      params.clear(); 
      params.SetValue("dims",  2);
      params.SetValue("mass0", 1.0645);
   //    Delta2->Init( params );
   
   
   
      fileWf.Name(argv[1]);
      fileWf.Suffix(BINARY_WF_SUFFIX);
      fileWf >> Psi;
   
//       *Psi *= 2;
//       cout << "WF Norm: " << Psi->Norm() << endl;
//       Psi->Normalize();
      cout << "WF Norm: " << Psi->Norm() << endl;
      cout << "Ekin: " << Delta1->Expec(Psi)  << endl;
      
      params.clear();
      params.SetValue("file", string(argv[2]));
      pot->Init(params);
      
      cout << "Vpot: " << pot->Expec( Psi ) << endl;
      
      cout << "E: " << (pot->Expec( Psi ) + Delta1->Expec(Psi)) << endl;
      
      
      V.Add(pot);
      
      cout << "E: " << V.Expec( Psi ) + Delta1->Expec(Psi)  << endl;
      Ham.Add(Delta1);
      Ham.Add(pot);
      
/*      U.AddTkin(dynamic_cast<OKSpace*> (Delta1));
      U.AddVpot(&V);*/
      
      clock.Dt(0.01);
      
      U.AddHamiltonian(&Ham);
      U.Clock(&clock);
      U.Forward();
      
      cout << "run propa, exp: " << U.Exponent() << endl;
      
       for (int i=0; i < 10; i++){
 	U *= Psi;
	cout << "E: " << V.Expec( Psi ) + Delta1->Expec(Psi)  << endl;
	cout << "WF Norm: " << Psi->Norm() << endl;
       }
       cout << "WF Norm: " << Psi->Norm() << endl;
       
       
   } catch (Exception e){
      cerr << e.GetMessage() << endl;
   }

}
