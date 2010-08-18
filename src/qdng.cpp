#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#ifdef _OPENMP
 #include <omp.h>
#endif

#include "tools/Exception.h"
#include "tools/Getopt.h"
#include "tools/XmlParser.h"
#include "tools/Logger.h"
#include "tools/GlobalParams.h"

#include "modules/ModuleLoader.h"

#include "ProgPropa.h"
#include "ProgEigen.h"
#include "ProgOCT.h"

using namespace std;
using namespace QDLIB;


/**
 * Show the version information.
 */
void show_version()
{
   Logger& log = Logger::InstanceRef();
   
   /* Version Info */
   log.cout() << endl;
   log.cout() << "QDng " << VERSION;
   log.cout() << " (build " << QDNG_BUILD_DATE;
   log.cout() << ", " << QDNG_BUILD_HOST;
   log.cout() << ", " << QDNG_BUILD_MACH;
#ifdef QDNG_REV
   log.cout() << ", " << QDNG_REV;
#endif
   log.cout() << ")" << endl; log.flush();
}

/**
 * This is the QDng main program.
 */
int main(int argc, char **argv)
{

   Getopt cmdline;
   string fname, dir;
   
   time_t wall_time;
   clock_t cpu_time;
   struct tms proc_time, proc_time_new;
   
   int retval = EXIT_SUCCESS;
   
   Logger& log = Logger::InstanceRef();
   ParamContainer& gp = GlobalParams::Instance();
   
   XmlParser XMLfile;
   XmlNode   rnodes;
   XmlNode   *prognodes;
   string progname;
   
   cmdline.SetDescription("QD next generation");
   
#ifdef USE_DYNMODS
   ModuleLoader *mods = ModuleLoader::Instance();
   cmdline.SetHelp( 'm', "module path", false,
		    "User defined path to the wave function and operator modules", "");
#endif
   
   cmdline.SetHelp( 'd', "output directory path", false,
		    "All output files are saved in this directory", "");

#ifdef _OPENMP
   int procs=1;
   cmdline.SetHelp( 'p', "num threads", false, "Specify the number of threads to use", "1");
#endif
   
   cmdline.ReadArgs(argc, argv);
   cmdline.CheckOptions();
   
   

      
   try {
      /*Show the help ?*/
      if ( cmdline.GetOption('h') ) exit(retval);
      
#ifdef USE_DYNMODS
      /* Provide a user path for module loading */
      if (cmdline.GetOption('m')){
	 cmdline.GetOption('m', fname);
	 mods->UserPath( fname );
      }
#endif

      /* Try to get output path from environment variable */      
      char *qddir;
      qddir = getenv("QD_DIR");
      if (qddir != NULL) dir = qddir;
 
      /* The global output path */
      if (cmdline.GetOption('d')){
	 cmdline.GetOption('d', dir);
      }
      
#ifdef _OPENMP
      /* Get the number of openmp threads */
      if (cmdline.GetOption('p')){
         cmdline.GetOption( 'p', procs);
         omp_set_num_threads(procs);
      }
#endif
      
      if(! cmdline.GetNonOption(0, fname) )
	 throw ( EParamProblem ("No input file given") );
      
   } catch (Exception e) {
      cerr << e.GetMessage() << "\n";
      show_version();
      cmdline.ShowHelp();
      exit(0);
   }
   
   log.Header("QDng - Quantum Dynamics", Logger::Chapter);
   log.flush();
   
   show_version();
   
   /* This is the global try-catch block */
   try {    
      /* Open input file and check programm nodes */
      XMLfile.Parse(fname);
      
      rnodes = XMLfile.Root();
      
      /* enables multiple programm nodes*/
      if (rnodes.Name() == "multi" || rnodes.Name() == "qdng") {
         /* Read out global attributes/parameters */
         gp = rnodes.Attributes();
	 prognodes = rnodes.NextChild();
      } else 
	 prognodes = &rnodes;
      
      /* overide path from command line */
      if ( ! dir.empty() )
         gp.SetValue("dir", dir);
      
      if ( gp.isPresent("dir") ) {
         gp.GetValue("dir", dir);
         if (dir[dir.length()-1] != '/' && ! dir.empty())
            dir += "/";
         gp.SetValue("dir", dir);
      }
      
      log.cout() <<  endl;
      log.Header("Global Parameters", Logger::SubSection);
#ifdef _OPENMP
      log.cout() << "Running with " << omp_get_max_threads() << " threads.\n";
#endif
#ifdef HAVE_SSE2
      log.cout() << "Using SSE2 optimizations.\n";
#endif
      log.cout() << gp;
      log.flush();
      
      if (prognodes == NULL)
	 throw ( EParamProblem ("Empty parameter file provided") );
      
      /* Loop over program nodes */
      while (prognodes->EndNode()) {
	 cpu_time = times(&proc_time);
	 wall_time = time(NULL);
	 progname = prognodes->Name();
	 if (progname == "propa"){
	    ProgPropa propa(*prognodes);
	    propa.SetDirectory(dir);
	    log.Header("Propagation", Logger::Chapter);
	    propa.Run();
	 } else if (progname == "auto"){
/*	    ProgAuto autoc(rnodes);
	    log.Header("Run Autocorrelation", Logger::Chapter);
	    autoc.Run();*/
	    throw ( EParamProblem ("Autocorrelation not implementet yet") );
	 } else if (progname == "eigen") {
	    ProgEigen eigen(*prognodes);
	    eigen.SetDirectory(dir);
	    log.Header("Imaginary Time Eigenfunctions", Logger::Chapter);
	    eigen.Run();
	 } else if (progname == "oct") {
	    ProgOCT oct(*prognodes);
	    oct.SetDirectory(dir);
	    log.Header("Optimal control theory", Logger::Chapter);
	    oct.Run();
	 } else if (progname == "densmat") {
	    throw ( EParamProblem ("Density matrix propagation not implementet yet") );
	 } else {
	    throw ( EParamProblem ("Programm type not known") );
	 }
	 
	 /* Show time consumption */
	 log.cout().precision(0);
	 log.cout() << "\nWall time: " << fixed << difftime(time(NULL), wall_time) << " s";
	 
	 cpu_time = times(&proc_time_new);
	 log.cout() << "\tCPU time: " << double(proc_time_new.tms_utime - proc_time.tms_utime)/double(sysconf(_SC_CLK_TCK)) << " s";
	 log.cout() << "\tSYS time: " << double(proc_time_new.tms_stime - proc_time.tms_stime)/double(sysconf(_SC_CLK_TCK)) << " s \n\n";
	 
	 prognodes->NextNode();
      }
      log.cout() << "Normal termination\n\n";
   } catch (Exception e) {
      log.flush();
      cerr << "\n\n\t!!!A problematic error occured:\n\t";
      cerr << e.GetMessage() << "\n\n\n";
      log.cout() << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }
  
  log.Close();
  exit(retval);
}
