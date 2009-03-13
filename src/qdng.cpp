#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "tools/Exception.h"
#include "tools/Getopt.h"
#include "tools/XmlParser.h"
#include "tools/Logger.h"

#include "modules/ModuleLoader.h"

#include "ProgPropa.h"
#include "ProgEigen.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{

   Getopt cmdline;
   string fname, dir;
   
   int retval = EXIT_SUCCESS;
   
   Logger& log = Logger::InstanceRef();
   
   XmlParser XMLfile;
   XmlNode   rnodes;
   XmlNode   *prognodes;
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
      
      /* The global output path */
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
      
      /* enables multiple programm nodes*/
      if (rnodes.Name() == "multi")
	 prognodes = rnodes.NextChild();
      else 
	 prognodes = &rnodes;
      
      if (prognodes == NULL)
	 throw ( EParamProblem ("Empty parameter file provided") );
      
      /* Loop over program nodes */
      while (prognodes->EndNode()) {
	 progname = prognodes->Name();
	 if (progname == "propa"){
	    ProgPropa propa(*prognodes);
	    propa.SetDirectory(dir);
	    log.Header("Propagation", Logger::Chapter);
	    propa.Run();
	 } else if (progname == "auto"){
/*	    ProgAuto autoc(rnodes);
	    cout << "\n\t\t\t*** Run Autocorrelation ***\n\n";
	    autoc.Run();*/
	    throw ( EParamProblem ("Autocorrelation not implementet yet") );
	 } else if (progname == "eigen") {
	    ProgEigen eigen(*prognodes);
	    eigen.SetDirectory(dir);
	    log.Header("Imaginary Time Eigenfunctions", Logger::Chapter);
	    eigen.Run();
	 } else if (progname == "oct") {
	    throw ( EParamProblem ("OCT not implementet yet") );
	 } else if (progname == "densmat") {
	    throw ( EParamProblem ("Density matrix propagation not implementet yet") );
	 } else {
	    throw ( EParamProblem ("Programm type not known") );
	 }
	 
	 prognodes->NextNode();
      }
      log.cout() << "Normal termination\n\n";
   } catch (Exception e) {
      cerr << "\n\n\t!!!A problematic error occured:\n\t";
      cerr << e.GetMessage() << "\n\n\n";
      log.cout() << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }
  
  log.Close();
  exit(retval);
}
