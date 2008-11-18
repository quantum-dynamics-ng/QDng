#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include "sys/Exception.h"
#include "sys/Getopt.h"
#include "sys/XmlParser.h"

#include "modules/ModuleLoader.h"

#include "ProgPropa.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{

   Getopt cmdline;
   string fname;
   
   XmlParser XMLfile;
   XmlNode   rnodes;
   string progname;
   
   ModuleLoader *mods = ModuleLoader::Instance();
   
   cmdline.SetDescription("QD next generation");
   cmdline.SetHelp( 'm', "module path", false,
		    "User defined path to the wave function and operator modules", "");
   cmdline.ReadArgs(argc, argv);
//    cmdline.CheckOptions();
   
   /* This is the global try-catch block */
   try {
      
      /* Provide a user path for module loading */
      if (cmdline.GetOption('m')){
	 cmdline.GetOption('m', fname);
	 cout << fname << endl;
	 mods->UserPath( fname );
      }
      
      
      if(! cmdline.GetNonOption(0, fname) )
	 throw ( EParamProblem ("No input file given") );
      

      
      /* Open input file and check programm nodes */
      XMLfile.Parse(fname);
      
      rnodes = XMLfile.Root();
      
      /* Loop over program nodes */
      while (rnodes.EndNode()) {
	 progname = rnodes.Name();
	 
	 if (progname == "propa"){
	    ProgPropa propa(rnodes);
	    cout << "\n\t\t\t*** Run Propagation ***\n\n";
	    propa.Run();
	 } else if (progname == "eigen") {
	    throw ( EParamProblem ("Eigenvalue solving not implementet yet") );
	 } else if (progname == "oct") {
	    throw ( EParamProblem ("OCT not implementet yet") );
	 } else if (progname == "densmat") {
	    throw ( EParamProblem ("Density matrix propagation not implementet yet") );
	 } else {
	    throw ( EParamProblem ("Programm type not known") );
	 }
	 
	 rnodes.NextNode();
      }
      
   } catch (Exception e) {
      cerr << "\n\n\t!!!A problematic error occured:\n\t";
      cerr << e.GetMessage() << "\n\n\n";
   }
  
  delete mods;
  return EXIT_SUCCESS;
}
