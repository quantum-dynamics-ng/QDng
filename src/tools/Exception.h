#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <string>

#include "ParamContainer.h"


namespace QDLIB {
   
   /**
    * Base class for exceptions.
    * 
    * Every dervied class should also provide a construcotor with a message.
    * If the programm crashes and no special error handling is done there
    * could be at least a description of the problem in log file etc.
    */
   class Exception
   {
      private:
         string _message;
      public:
         Exception(){}
         Exception(const char *message) : _message(message) {}
            
	 void SetMessage(const string &message)
	 {
	    _message = message;
	 }
         
         string& GetMessage()
         {
            return _message;
         }
   };
   

   /**
    * Indicates a memory problem
    */
   class EMemError : public Exception
   {
      public:
         EMemError(const char *message) : Exception(message) {}
         EMemError(int cerrno, const char *message);
   };

   /**
    * Indicates an I/O Error.
    */
   class EIOError : public Exception
   {
      public:
         EIOError(const char *message) : Exception(message) {}
	 EIOError(const char *message, const string &fname);
         EIOError(int cerrno, const string &name);
   };
   
   /**
    * Incompabilty of wavefunctions or operators.
    */
   class EIncompatible : public Exception
   {
      public:
	 EIncompatible(const char *message) : Exception(message) {}
	 
	 EIncompatible(const char *message, const string &name);
	 EIncompatible(const char *message, const string &name1, const string &name2);
   };
   
   /**
    * Indicates a parameter problem.
    */
   class EParamProblem : public Exception
   {
      private:
         ParamContainer _params;

      public:
         EParamProblem(const char *message) :  Exception(message){}
         /**
          * Reports missing parameters.
          * 
          * \param missing Should contain only the missing parameters
          */
         EParamProblem(ParamContainer &missing) : _params(missing) {}
         /**
          * Reports missing parameters and a diagnostic message.
          * 
          * \param missing Should contain only the missing parameters.
          * \param message Additional message about the problem.
          */
         EParamProblem(ParamContainer &missing, const char *message) : Exception(message), _params(missing)  {}
	 
	 EParamProblem(const char *message, const string &reason);

	 EParamProblem(const char *message, const double reason);
	 
	 EParamProblem(const char *message, const int reason);
	 

   };
   
   /**
    * Indicate an overflow/Nummerical problem.
    */
   class EOverflow : public Exception {
      public:
         EOverflow(const char *message) : Exception(message) {}
   };
   
   /**
    * Indicate a memory problem.
    */
   class EMemory : public Exception {
      public:
         EMemory() : Exception ("Out of memory") {}
         EMemory(const char *message) : Exception(message) {}
   };


   /**
    * MPI Exception.
    * 
    * This indicates a fatal MPI Problem.
    */
   class EMPI : public Exception
   {
      public:
         EMPI(const char *message) : Exception(message) {}
   };
         
} /* namespace QDLIB */

#endif /* #ifndef EXCEPTION_H */

