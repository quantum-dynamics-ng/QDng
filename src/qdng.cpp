#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "sys/Exception.h"
#include "sys/Getopt.h"
#include "sys/XmlParser.h"

#include "modules/ModuleLoader.h"

#include "ProgPropa.h"
#include "ProgEigen.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{

   Getopt cmdline;
   string fname, dir;
   
   XmlParser XMLfile;
   XmlNode   rnodes;
   string progname;
   
   ModuleLoader *mods = ModuleLoader::Instance();
   
   cmdline.SetDescription("QD next generation");
   cmdline.SetHelp( 'm', "module path", false,
		    "User defined path to the wave function and operator modules", "");
   
   cmdline.SetHelp( 'd', "output directory path", false,
		    "All output files are saved in this directory", "");

#ifdef _OPENMP
   int procs;
   cmdline.SetHelp( 'p', "num threads", false, "Specify the number of threads to use", "1");
#endif
   
   cmdline.ReadArgs(argc, argv);
   cmdline.CheckOptions();
   
   

      
   try {
      /*Show the help ?*/
      if ( cmdline.GetOption('h') ) throw (Exception());
      
      /* Provide a user path for module loading */
      if (cmdline.GetOption('m')){
	 cmdline.GetOption('m', fname);
	 mods->UserPath( fname );
      }
      
      /* Provide a user path for module loading */
      if (cmdline.GetOption('d')){
	 cmdline.GetOption('d', dir);
	 if (dir[dir.length()-1] != '/' && ! dir.empty())
	    dir += "/";
      }
      
#ifdef _OPENMP
      /* Get the number of openmp threads */
      cmdline.GetOption( 'p', procs);
      omp_set_num_threads(procs);
      cout << "Running with " << procs << " threads.\n";
#endif
      
      if(! cmdline.GetNonOption(0, fname) )
	 throw ( EParamProblem ("No input file given") );
      
   } catch (Exception e) {
      cerr << e.GetMessage() << "\n";
      cmdline.ShowHelp();
      exit(0);
   }
   
   /* This is the global try-catch block */
   try {    
      /* Open input file and check programm nodes */
      XMLfile.Parse(fname);
      
      rnodes = XMLfile.Root();
      
      /* Loop over program nodes */
      while (rnodes.EndNode()) {
	 progname = rnodes.Name();
	 if (progname == "propa"){
	    ProgPropa propa(rnodes);
	    propa.SetDirectory(dir);
	    cout << "\n\t\t\t*** Run Propagation ***\n\n";
	    propa.Run();
	 } else if (progname == "auto"){
/*	    ProgAuto autoc(rnodes);
	    cout << "\n\t\t\t*** Run Autocorrelation ***\n\n";
	    autoc.Run();*/
	    throw ( EParamProblem ("Autocorrelation not implementet yet") );
	 } else if (progname == "eigen") {
	    throw ( EParamProblem ("Eigenvalue solving not implementet yet") );
	    /* ProgEigen eigen(rnodes);
	    eigen.SetDirectory(dir);
	    cout << "\n\t\t\t*** Run Eigenfunction solver ***\n\n";
	    eigen.Run(); */
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
