#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/utsname.h>


#ifdef _OPENMP
 #include <omp.h>
#endif

#include "tools/Exception.h"
#include "tools/Getopt.h"
#include "tools/XmlParser.h"
#include "tools/Logger.h"
#include "tools/GlobalParams.h"

#include "modules/ModuleLoader.h"

#include "simpleXml/Simple2Xml.h"

#include "ChainLoader.h"
#include "ProgPropa.h"
#include "ProgFilter.h"
#include "ProgEigen.h"
#include "ProgOCT.h"

#ifdef HAVE_MPI
#include "mpi.h"
#endif

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
   log.cout() << PACKAGE_STRING;
   log.cout() << " (build " << QDNG_BUILD_DATE;
   log.cout() << ", " << QDNG_BUILD_HOST;
   log.cout() << ", " << QDNG_BUILD_MACH;
#ifdef QDNG_REV
   if (strlen(QDNG_REV) > 0)
      log.cout() << ", " << QDNG_REV;
#endif
   log.cout() << ")" << endl;
   
#ifdef HAVE_UNAME
   /* Get machine info */
   struct utsname ubuf;
   uname(&ubuf);
   
   log.cout() << "Running on " << ubuf.nodename;
   log.cout() << " (" << ubuf.sysname<< ", " <<ubuf.machine << ")";
   log.cout()<<endl;
   log.flush();
#endif
}

/**
 * Show initial Memory informations
 **/
void show_memory_info()
{
   Memory& mem = Memory::Instance();
   Logger& log = Logger::InstanceRef();

   log.cout() << endl;
   log.cout() << "Maximum Memory available for data: " << mem.Format(mem.MaximumSize()) << endl;
   log.cout() << endl;
   log.flush();
}

/**
 * Check input format type & convert if needed.
 */
void CheckInput(string &name)
{
   Logger& log = Logger::InstanceRef();
   QDSXML::FileType type;
   
   type = QDSXML::CheckType(name);
   
   if (type == QDSXML::SIMPLE) {
      log.cout() << "SimpleXML-Input format"<< endl;
      log.cout() << "Converting to XML file... " << name+".qdi"<<endl;
      QDSXML::Convert(name, name+".qdi");
      name = name +".qdi";
   }
   if (type == QDSXML::XML)
      log.cout() << "XML-Input format" << endl;
      
   if (type == QDSXML::UNKNOWN) {
         throw( EIncompatible("Undefined format of input file"));
   }
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
   
#ifdef HAVE_MPI
   MPI::Init();
#endif

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
   
   cmdline.SetHelp( 'g', "Turn debug output on. Some extra informations will be printed");
   cmdline.SetHelp( 'v', "Show version information and exit.");
   
#ifdef _OPENMP
   int procs=1;
   cmdline.SetHelp( 'p', "num threads", false, "Specify the number of threads to use", "1");
#endif
   
   cmdline.ReadArgs(argc, argv);
   cmdline.CheckOptions();
   
   

      
   try {
      /*Show the help ?*/
      if ( cmdline.GetOption('h') ) exit(retval);
      
      if ( cmdline.GetOption('v') ) {
         show_version();
         log.cout() << endl;
         log.flush();
         exit(retval);
      }
      
#ifdef USE_DYNMODS
      /* First check if a path is given in the global options */
      if (gp.isPresent("modpath")) {
         string path;
         gp.GetValue("modpath", path);
         mods->UserPath( path );
      }

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
      }
      omp_set_num_threads(procs);
#endif
      
      if(! cmdline.GetNonOption(0, fname) )
	 throw ( EParamProblem ("No input file given") );
      
      /* Check & parse variable definitions */
      int i=1;
      string vardef;
      extern map<string, string> varlist; /* defined along with Syntatic Tree model */
      while ( cmdline.GetNonOption(i, vardef) ){
         size_t pos = vardef.find_first_of('=');
         
         if (pos == string::npos)
            throw (EParamProblem ("Invalid variable definition"));
         
         string varname = vardef.substr(0,pos);
         string varval = vardef.substr(pos+1);

         varlist[varname] = varval; /* Write variable into storage of qds parser */
         
         i++;
      }
      
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
      /* Check for input format */
      CheckInput(fname);
      
      /* Open input file and check programm nodes */
      XMLfile.Parse(fname);
      
      rnodes = XMLfile.Root();
      
      /* enables multiple programm nodes*/
      if (rnodes.Name() == "multi" || rnodes.Name() == "qdng") {
         /* Read out global attributes/parameters */
         gp += rnodes.Attributes();
	 prognodes = rnodes.NextChild();
      } else 
	 prognodes = &rnodes;
      
	 /* Check if debug output is requested */
	 if (gp.isPresent("debug")){
	    bool dbg;
	    gp.GetValue("debug", dbg);
	    log.Debug(dbg);
	 }
	 
	 if (cmdline.GetOption('g'))
	    log.Debug(true);
	 
	 /* overide path from command line */
	 if ( ! dir.empty() )
	    gp.SetValue("dir", dir);
	 
	 if ( gp.isPresent("dir") ) {
	    gp.GetValue("dir", dir);
	    if (dir[dir.length()-1] != '/' && ! dir.empty())
	       dir += "/";
	    gp.SetValue("dir", dir);
      }

	 if (gp.Size() > 0) {
         log.cout() << endl;
         log.Header("Global Parameters", Logger::SubSection);
         log.cout() << gp;
         log.cout() << endl;
         log.flush();
      }
      
      if (log.Debug())
	 log.cout() << "Debug output on\n";
#ifdef _OPENMP
      log.cout() << "Running with " << omp_get_max_threads() << " threads.\n";
#endif
#ifdef HAVE_SSE2
      log.cout() << "Using SSE2 optimizations.\n";
#endif
      
      show_memory_info();

      if (prognodes == NULL)
	 throw ( EParamProblem ("Empty parameter file provided") );
      
      /* Look for global Operator definitions */
      if ( prognodes->Name() == "opdefs" ){
         XmlNode *opdefs = prognodes->NextChild();
         if (opdefs != NULL) {
            log.Header("Loading global operator definitions", Logger::Section);
            while (opdefs->EndNode()) {
               Operator *O;
               
               log.Header( opdefs->Name(), Logger::SubSection );
               log.IndentInc();
               if ( opdefs->Name() == "udef" ) {
                  O = ChainLoader::LoadPropagator( opdefs, true );
               } else if ( opdefs->Name() == "opdef" ) {
                  O = ChainLoader::LoadOperatorChain( opdefs, true );
               } else {
                  throw(EParamProblem("Unknown definition type: ", opdefs->Name()));
               }
               
               log.IndentDec();
               opdefs->NextNode();
               log.cout() << endl;
            }
         }
         prognodes->NextNode();
         
         if ( log.Debug() )
            log.cout() << endl;
            log.Header("Internal Operator list", Logger::SubSection);
            log.IndentInc();
            GlobalOpList::Instance().PrintList();
      }
      
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
    } else if (progname == "filter") {
       ProgFilter filter(*prognodes);
       filter.SetDirectory(dir);
       log.Header("Filter", Logger::Chapter);
       filter.Run();
	 } else if (progname == "eigen") {
	    ProgEigen eigen(*prognodes);
	    eigen.SetDirectory(dir);
	    log.Header("Eigenfunctions calculation", Logger::Chapter);
	    eigen.Run();
	 } else if (progname == "oct") {
	    ProgOCT oct(*prognodes);
	    oct.SetDirectory(dir);
	    log.Header("Optimal control theory", Logger::Chapter);
	    oct.Run();
	 } else if (progname == "densmat") {
	    throw ( EParamProblem ("Density matrix propagation not implementet yet") );
	 } else {
	    throw ( EParamProblem ("Programm type not known ", progname) );
	 }
	 
	 /* Show memory consumption */
	 log.cout() << "\nMaximum amount of memory used: " << Memory::Format(Memory::Instance().MaximumSizeUsed()) << endl;

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
      log.ConsoleOutput();
      log.flush();
      log.cout() << "\n\n\t!!!A problematic error occured:\n\t";
      log.cout() << e.GetMessage() << "\n\n\n";
      log.cout() << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }
  
  log.Close();

#ifdef HAVE_MPI
   MPI::Finalize();
#endif

  exit(retval);
}
