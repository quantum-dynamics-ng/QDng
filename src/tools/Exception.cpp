#include "Exception.h"

#include <sstream>
#include "sys/stat.h"
#include "sys/errno.h"

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
   

   EMemError::EMemError(int cerrno, const char *message) : Exception()
   {
      string s;

      switch(cerrno){
         case EINVAL:
            s = "EINVAL";
            break;
         case ENOMEM:
            s = "Out of memory";
            break;
         default:
            s = "Error in system call";
      }
      SetMessage(string(message) + " - " + s);
   }

   EIOError::EIOError( const char * message, const string &fname ) : Exception()
   {
      SetMessage(string(message) + ": " + fname);
   }

   EIOError::EIOError(int cerrno, const string & name)
   {
      string s;
      
      switch(cerrno){
         case EACCES:
               s = "Access denied";
            break;
         case EIO:
               s = "I/O read error";
            break;
         case EEXIST:
               s = "File exists";
            break;
         case ENOTDIR:
               s = "Path does not exist";
            break;
         case ENOSPC:
               s = "No space left on device";
            break;
         case ENAMETOOLONG:
               s = "File name too long";
            break;
         case EROFS:
               s = "Access to read-only fs";
            break;
         default:
            s = "Error in accessing file";
            break;
      }
      SetMessage(s + ": " + name);
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
   
   EWrongClassType::EWrongClassType(const char* expectype, const string &classname) :
          EIncompatible(string("Class not of type " + string(expectype) + ": " + classname).c_str())
   {
   }

}





