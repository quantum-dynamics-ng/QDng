#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include "sys/Exception.h"
#include "sys/Getopt.h"
#include "sys/XmlParser.h"

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
   
   cmdline.SetDescription("QD next generation");
   cmdline.ReadArgs(argc, argv);
   
   /* This is the global try-catch block */
   try {
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
	    propa.Run();
	    cout << "*** Run Propagation\n\n";
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
      cerr << "A problematic error occured:\n\t";
      cerr << e.GetMessage() << endl;
   }

  return EXIT_SUCCESS;
}
