#include "mex.h"

#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include "mex/init_wf_file.h"

#include "mex/convert_to_mxArray.h"
#include "tools/Exception.h"
#include "tools/FileSingleDefs.h"
#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/WFMultistate.h"
using namespace QDLIB;
int main()
{
std::cout << "Hallo Welt 3" << std::endl;

int nlhs = 1;
int nrhs = 2;
mxArray *plhs[1];
mxArray *prhs[2];


std::string str1,str2;
char *cstr1[1], *cstr2[1];
 
str1= "/home.asteria/pvh/K2/Propa_Pot_Baumert/WF0";
str2= "GridCartesian";


cstr1[0] = new char [str1.size()+1];
cstr2[0] = new char [str2.size()+1];

std::strcpy (cstr1[0], str1.c_str());
std::strcpy (cstr2[0], str2.c_str());

prhs[0] = mxCreateCharMatrixFromStrings(1, (const char **) cstr1); 
prhs[1] = mxCreateCharMatrixFromStrings(1, (const char **) cstr2);
mexFunction( nlhs, plhs, nrhs, (const mxArray**) prhs);
std::cout << "Hallo Welt 6" << std::endl;
return 0;
}