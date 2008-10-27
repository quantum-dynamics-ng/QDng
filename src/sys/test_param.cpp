#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "paramcontainer.h"
using namespace  std;
using namespace  QDLIB;



int main(int argc, char **argv)
{
   
   ParamContainer p;
   
   int i;
   double d;
   
   string s,k,v;
   stringstream ss;
   
   
   p.SetValue("key", "val");
   p.SetValue("number", 10);
   p.SetValue("double", 0.1);
   
   
   p.GetValue("key", s);
   p.GetValue("number", i);
   p.GetValue("double", d);
   
   
   cout << s << endl;
   cout << i << endl;
   cout << d << endl;
   
   p.ResetPosition();
   while (p.GetNextValue(k,v)){
      cout << k << " = " << v << endl;
   }
   
      
      fstream fs;
      
      
      /** Write meta file */
      p.ResetPosition();
      
      s = "test.meta";
      fs.open(s.c_str(),  ios_base::out);
      
      while ( p.GetNextValue(k,v) ){
	 fs << k << " = " << v << endl;
      }
      
      fs.close();
      
}

