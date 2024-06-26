// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

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

#include "CmdHandler.h"

#ifdef HAVE_MPI
#include "mpi.h"
#endif

#ifdef HAVE_PERFCOUNTERS
#include "PerfCounterList.h"
#endif

#include "config.h"

using namespace std;
using namespace QDLIB;

#define QDNG_INPUT_BUFFER 4096

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
   log.cout() << " (" << ubuf.sysname << ", " << ubuf.machine << ")";
   log.cout() << endl;
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
void CheckInput(const string &name, string &xml)
{
   Logger& log = Logger::InstanceRef();
   QDSXML::FileType type;

   type = QDSXML::CheckType(name);

   if (type == QDSXML::SIMPLE) {
      log.cout() << "SimpleXML-Input format" << endl;
      log.cout() << "Converting to XML... " << endl;
      log.flush();
      QDSXML::Parse(name, xml);
   }

   if (type == QDSXML::XML){
      log.cout() << "XML-Input format" << endl;
      fstream infile;
      infile.open(name.c_str(), fstream::in);

      char* buf = (char*) malloc(QDNG_INPUT_BUFFER);
      size_t size = 0;

      /* Read clean XML input */
      while (! infile.eof() ){
         if (size > 0) buf = (char*) realloc(buf, size + QDNG_INPUT_BUFFER);

         infile.read(buf+size, QDNG_INPUT_BUFFER);

         size += infile.gcount();
      }
      xml = buf;
      free(buf);
   }

   if (type == QDSXML::UNKNOWN) {
      throw(EIncompatible("Undefined format of input file"));
   }
}

/**
 * Run main programs defined in input.
 */
void RunProgrammes(const string& name, const string& dir)
{
   /* Check for input format */
   string buf;
   CheckInput(name, buf);

   CmdHandler::RunXML(buf.c_str(), buf.size(),  dir);
}

/**
 * This is the QDng main program.
 */
int main(int argc, char **argv)
{

   Getopt cmdline;
   string fname, dir;

   int retval = EXIT_SUCCESS;

#ifdef HAVE_MPI
   MPI::Init();
#endif

   Logger& log = Logger::InstanceRef();
   ParamContainer& gp = GlobalParams::Instance();

   bool CmdMode = false; /* interactive command mode */

   cmdline.SetDescription("QD next generation");

#ifdef USE_DYNMODS
   ModuleLoader<WaveFunction> *mods_wf = ModuleLoader<WaveFunction>::Instance();
   ModuleLoader<Operator> *mods_op = ModuleLoader<Operator>::Instance();
   cmdline.SetHelp( 'm', "module path", false,
            "User defined path to the wave function and operator modules", "");
#endif

   cmdline.SetHelp('d', "output directory path", false, "All output files are saved in this directory", "");

   cmdline.SetHelp('g', "Turn debug output on. Some extra informations will be printed");
   cmdline.SetHelp('c', "Start QDng in interactive command mode.");
   cmdline.SetHelp('v', "Show version information and exit.");

#ifdef _OPENMP
   int procs=1;
   cmdline.SetHelp( 'p', "num threads", false, "Specify the number of threads to use", "1");
#endif

   cmdline.ReadArgs(argc, argv);
   cmdline.CheckOptions();

   try {
      /*Show the help ?*/
      if (cmdline.GetOption('h'))
         exit(retval);

      if (cmdline.GetOption('v')) {
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
         mods_op->UserPath( path );
         mods_wf->UserPath( path );
      }

      /* Provide a user path for module loading */
      if (cmdline.GetOption('m')) {
         cmdline.GetOption('m', fname);
         mods_op->UserPath( fname );
         mods_wf->UserPath( fname );
      }
#endif

      /* Try to get output path from environment variable */
      char *qddir;
      qddir = getenv("QD_DIR");
      if (qddir != NULL)
         dir = qddir;

      /* The global output path */
      if (cmdline.GetOption('d')) {
         cmdline.GetOption('d', dir);
      }

#ifdef _OPENMP
      /* Get the number of openmp threads */
      if (cmdline.GetOption('p')) {
         cmdline.GetOption( 'p', procs);
      }
      omp_set_num_threads(procs);
#endif

      if (!cmdline.GetNonOption(0, fname))
         throw(EParamProblem("No input file given"));

      /* Check & parse variable definitions */
      int i = 1;
      string vardef;
      extern map<string, string> varlist; /* defined along with Syntatic Tree model */
      while (cmdline.GetNonOption(i, vardef)) {
         size_t pos = vardef.find_first_of('=');

         if (pos == string::npos)
            throw(EParamProblem("Invalid variable definition"));

         string varname = vardef.substr(0, pos);
         string varval = vardef.substr(pos + 1);

         varlist[varname] = varval; /* Write variable into storage of qds parser */

         i++;
      }

      if (cmdline.GetOption('c')){
         CmdMode = true;
         /* First thing we do is stating our PID */
         cout << getpid() << endl;
      }

   } catch (Exception &e) {
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

      /* Check if debug output is requested */
      if (gp.isPresent("debug")) {
         bool dbg;
         gp.GetValue("debug", dbg);
         log.Debug(dbg);
      }

      if (cmdline.GetOption('g'))
         log.Debug(true);

      if (log.Debug())
         log.cout() << "Debug output on\n";
#ifdef _OPENMP
      log.cout() << "Running with " << omp_get_max_threads() << " threads.\n";
#endif
#ifdef HAVE_SSE2
      log.cout() << "Using SSE2 optimizations.\n";
#endif

      show_memory_info();

      if (CmdMode) { /* */
         log.cout() << "Running in interactive mode" << endl;
         log.flush();

         if (fname == "-"){
            CmdHandler cmd(cin, cout);    // stdin/stdout version
            cmd.RunInteractive(dir);
         } else {
            CmdHandler cmd(fname);        // connect via FIFO
            cmd.RunInteractive(dir);
         }
      } else
         RunProgrammes(fname, dir);

      log.cout() << "Normal termination\n\n";
   } catch (Exception &e) {
      log.ConsoleOutput();
      log.flush();
      log.cout() << "\n\n\t!!!A problematic error occured:\n\t";
      log.cout() << e.GetMessage() << "\n\n\n";
      log.cout() << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }


#ifdef HAVE_MPI
   MPI::Finalize();
#endif

#ifdef HAVE_PERFCOUNTERS
   log.cout() << endl << endl;
   PerfCounterList::Reference().PrintStats();
#endif

   log.Close();
   exit(retval);
}
