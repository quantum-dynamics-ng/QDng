#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "KeyValFile.h"
#include "paramcontainer.h"

using namespace std;
using namespace QDLIB;



int main(int argc, char **argv)
{

   ParamContainer p;
   KeyValFile file("test.meta");
   string k,v;
   bool verbose;
   
   if ( file.Parse(p) ){
      cout << "File parsed" << endl;
      p.ResetPosition();
      p.GetValue("verbose", verbose);
      if (verbose) {
         while( p.GetNextValue(k, v) ){
	  cout << k << "=" << v << "<" << endl;
         }
      }
   }
   else {
      cerr << "Parser Error" << endl;
   }
   
   file.SetName("test.clean.meta");
   
   if ( ! file.Write(p) ) 
   {
      cerr << "Writer Error" << endl;
   }   
}

