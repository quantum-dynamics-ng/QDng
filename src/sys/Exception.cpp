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
      string s;

      sst << message << ": " << reason;
      sst >> s;
      SetMessage(s);
   }
   
}
