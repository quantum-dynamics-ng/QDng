#ifndef GETOPT_H
#define GETOPT_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

using namespace std;


namespace QDLIB {

   struct cmp_char
   {
      /*bool operator()(char const *a, char const *b) 
      {
	 return std::strcmp(a, b) < 0;
      }*/
     bool operator()(char const a, char const b) 
      {
	 if (a < b) 
	   return false;
	 else
	   return true;
      }
   };
   
   
   
   /**
    * A comfortable interface to the getopt.
    * 
    * The simplest way to use it:
    *  \li Define your parameters (SetHelp, SetDescription)
    *  \li Parse the commandline (ReadArgs)
    *  \li Check if somethings missing (CheckOptions)
    * 
    * \todo some error handling for wrong options, etc., missing options ...
    * \todo some handling for non-options commandline parameters
    * \todo maybe some different output device (other than stderr)?
    * \bug Default values doesn't work
    */
   class Getopt {
     private:    
       vector<string> argvalues;
       map<char, string> value_map, help_map, parameter_map, default_map;
       map<char, bool> req_map;
       string basename;
       int _argc;
       string _description;
       
       void ParseArgs(int argc, char **argv, const char *optdef);
       
     public:
       Getopt();
       Getopt(int argc, char **argv, char *optdef);
       void ReadArgs(int argc, char **argv);
       
       void SetDescription(const string desc);
       void SetHelp(const char letter, const string &help);
       void SetHelp(const char letter, const string &parameter, bool required, const string &help, const string &value = "");
       
       void ShowHelp();
       bool CheckOptions();
       
       bool GetOption(char letter);
       
       void GetOption(char letter, char &value);
       void GetOption(char letter, int &value);
       void GetOption(char letter, double &value);
       void GetOption(char letter, string &value);
   };

} /* namespace QDLIB */

#endif /* #ifndef GETOPT_H */

