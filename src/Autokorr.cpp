// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "qdlib/WFGridCartesian.h"
#include "tools/FileSingleDefs.h"
#include "tools/Getopt.h"

namespace QDLIB{

   void ProgAutocorr::Run()
   {
      WFGridCartesian Psi0, Psit;
      Getopt cmdline;
      int steps;
      
      FileWF file;
      
      cVec Ct,Cw;
      
      cmdline.SetHelp( 't', "steps", true, "number of timesteps");
      cmdline.ReadArgs(argc, argv);
      
      if (!cmdline.CheckOptions()) exit(1);
   
      cmdline.GetOption( 't', steps);
      
      file.Suffix("");
      file.Name("WF1");
      file.ActivateSequence();
      
      Ct.newsize(steps);
      Cw.newsize(steps);
      FFT fft(Ct, Cw);
      try {
	 for (int i=0; i < steps; i++){
	    file >> (WaveFunction*) &Psit;
	    Psit.Normalize();
	    if (i == 0) Psi0 = &Psit;
	    
	    Ct[i] = Psi0 * &Psit;
	 }
	 
	 fft.forward();
	 for (int i=0; i < steps; i++){
	    cout << cabs(Cw[i]) <<endl;
	 }
      } catch (Exception e) {
	 cerr << e.GetMessage() << endl;
      }
   
   
}
