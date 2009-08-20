#include "Exception.h"

#include <sstream>

using namespace std;

namespace QDLIB{
   
   EParamProblem::EParamProblem(const char *message, const string &reason) : Exception()
   {
      SetMessage(string(message) + reason);
   }
   
   EParamProblem::EParamProblem(const char *message, const double reason) : Exception()
   {
      stringstream sst;

      sst << message << ": " << reason;
      SetMessage(sst.str());
   }
   
   EParamProblem::EParamProblem(const char *message, const int reason) : Exception()
   {
      stringstream sst;

      sst << message << ": " << reason;
      SetMessage(sst.str());
   }
   

   EIOError::EIOError( const char * message, string &name ) : Exception()
   {
      SetMessage(string(message) + ": " + name);
   }

   
   EIncompatible::EIncompatible(const char * message, const string & name)
   {
      string s = string(message) + ": " + name;
      SetMessage(s.c_str());
   }

   EIncompatible::EIncompatible(const char * message, const string & name1, const string & name2)
   {
      string s = string(message) + ": " + name1 + " <-> " + name2;
      SetMessage(s.c_str());

   }

   
}



