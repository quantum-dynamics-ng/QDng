/*
 * simple2xml.cpp
 *
 *  Created on: 23.12.2010
 *      Author: Markus Kowalewski
 */

#include <iostream>
#include <cstdlib>

#include "tools/Getopt.h"
#include "Simple2Xml.h"
#include "tools/Exception.h"

using namespace std;
using namespace QDSXML;
using namespace QDLIB;

/**
 * Prepare commandline options & help
 *
 */
void PrepareCmdline(Getopt &cmdline)
{
   cmdline.SetDescription("QDng standalone input converter");
}


int main(int argc, char** argv)
{
   Getopt cmdline;
   string fname;
   bool series;
   int retval = EXIT_SUCCESS;

   PrepareCmdline(cmdline);

   try {
      cmdline.ReadArgs(argc, argv);
      cmdline.CheckOptions();

      /*Show the help ?*/
      if ( cmdline.GetOption('h') ) exit(retval);


      if(! cmdline.GetNonOption(0, fname) )
         throw ( EParamProblem ("No input file given") );

   } catch (Exception e) {
      cerr << e.GetMessage() << "\n";
      cmdline.ShowHelp();
      exit(0);
   }

   try {
      Convert(fname, fname+string("qdi"));
   } catch (Exception e) {
      cerr << "\n\n\t!!!A problematic error occured:\n\t";
      cerr << e.GetMessage() << "\n\n\n";
      cout << "Error termination\n\n";
      retval = EXIT_FAILURE;
   }

   exit(retval);
}


