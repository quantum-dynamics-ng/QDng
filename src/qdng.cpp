#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include "sys/Getopt.h"
#include "sys/XmlParser.h"

using namespace std;
using namespace QDLIB;

int main(int argc, char **argv)
{

   Getopt cmdline;
   
   cmdline.SetDescription("QD next generation");
   cmdline.SetHelp('h', "Show help");
   cmdline.ReadArgs(argc, argv);
   if (!cmdline.CheckOptions() ){
      cmdline.ShowHelp();
      exit(1);
   }

  return EXIT_SUCCESS;
}
