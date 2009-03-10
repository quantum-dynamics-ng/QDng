#include <iostream>
#include <string.h>
#include "Getopt.h"


namespace QDLIB {

/**
 * Constructor.
 */
Getopt::Getopt()
{
  SetHelp('h', "Show this help");
}


/**
 * Constructor.
 * 
 * The parameters can be directly forwarded from main()
 * 
 * \param argc Number of arguments
 * \param argv C-Array of null-terminated strings
 * \param optdef define the possible options \see manpage getopt.3
 * 
 */
Getopt::Getopt(int argc, char **argv, char *optdef)
{
  ParseArgs(argc, argv, optdef);
}

/**
 * Common used method - private.
 */
void Getopt::ParseArgs(int argc, char **argv, const char *optdef)
{
  _argc = argc;
  _argv = argv;
  int i;
  opterr = 0;        /* Supress errors about unknown parameters */
  basename = argv[0];
   while ( (i = getopt(argc, argv, optdef)) != -1){
     char a = (char) i;
     if ( a != '?' && optarg != NULL) {
       value_map[a] = optarg;
     } else if (a != '?' && optarg == NULL){
	value_map[a] = "";
     }
   }
}

/**
 * Parse the arguments from the Commandline.
 * 
 * This is an alternative to the parsing constructor.
 * The native optdefs need not to defined. The prerequisist for
 * this method is that all desired arguments have been defined
 * with SetHelp.
 * 
 * \pre SetHelp
 * 
 * \param argc Number of arguments
 * \param argv C-Array of null-terminated strings
 */
void Getopt::ReadArgs(int argc, char **argv)
{
  string s;
 
  s.clear();
  for (map<char, string>::iterator it = help_map.begin(); it != help_map.end(); it++){
    if ( parameter_map[(*it).first].empty() ){
      s += (*it).first;
    }
    else {
      s += (*it).first;
      s += ':';
    }
  }
  ParseArgs(argc, argv, s.c_str());
}

/**
 * Set a program description.
 * 
 * \param desc description string which is printed with the help
 */
void Getopt::SetDescription(const string desc)
{
  _description = desc;
}

/**
 * Set help for a option.
 * 
 * \param letter The option letter
 * \param help Description
 */
void Getopt::SetHelp(const char letter, const string &help)
{
  help_map[letter] = help;
  parameter_map[letter] = "";
}


/**
 * Set help for a option.
 * 
 * \param letter the option letter
 * \param string The parameter for this option
 * \param requried defines if this option is required
 * \param help Description
 * \param value optional default value
 */
void Getopt::SetHelp(const char letter, const string &parameter, bool required, const string &help, const string &value)
{
  help_map[letter] = help;
  parameter_map[letter] = parameter;
  req_map[letter] = required;
  if (!required) default_map[letter] = value;
}


/**
 * Show help on stderr.
 * 
 * If the defines PACKAGE_NAME and PACKAGE_VERSION are defined they are shown.
 */
void Getopt::ShowHelp()
{
  /* Header */
#ifdef PACKAGE_NAME
   cerr << endl << PACKAGE_NAME;
#ifdef PACKAGE_VERSION
   cerr << " - " << PACKAGE_VERSION << endl;
#endif
#endif
  cerr << endl;
  cerr << _description  << endl;
  cerr << endl;
  /* Usage & description */
  cerr << "\tUsage:  " << basename << " ";
  for (map<char, string>::iterator it = help_map.begin(); it != help_map.end(); it++){
    if ( parameter_map[(*it).first].empty() ){
      cerr << "[-" << (*it).first << "] ";
    } else {
      if (req_map[(*it).first] == false){
	cerr << "[-" << (*it).first << " " << parameter_map[(*it).first] << "] ";
      } else {
	cerr << "<-" << (*it).first << " " << parameter_map[(*it).first] << "> ";
      }
    }
  }
  /* detailed help */
  cerr << endl << endl;  
  for (map<char, string>::iterator it= help_map.begin(); it != help_map.end(); it++){
    cerr << "\t\t-" << (*it).first << "\t\t\t" << (*it).second;
    if ( !default_map[(*it).first].empty() )
      cerr << " [" << default_map[(*it).first] << "]";
    cerr << endl;
  }
  cerr << endl << endl;
}

/**
 * Check if somethings missing.
 * 
 * If somethings wrong the first missing argument is shown + the help.
 */
bool Getopt::CheckOptions()
{
  if (GetOption('h'))
  {
      ShowHelp();
      return false;
  }
  
  for (map<char, string>::iterator it = help_map.begin(); it != help_map.end(); it++){
    if ( ! parameter_map[(*it).first].empty() && req_map[(*it).first] && value_map[(*it).first].empty() ){
      cerr << "\t>> Parameter -" << (*it).first << " is missing!" << endl;
      ShowHelp();
      return false;
    }
    if (parameter_map[(*it).first].empty() && ! default_map[(*it).first].empty()){
      value_map[(*it).first] = default_map[(*it).first];
    }
  }
  return true;
}


/**
 * Check if a command line options is set
 * 
 * \param letter letter for posix short option
 * 
 * \return true if options is existent in the command line
 */
bool Getopt::GetOption(char letter)
{
  if ( !value_map[letter].empty() ) return true;
  return false;
}

/**
 * Extracts a command line option.
 * 
 * \param letter letter for posix short option
 */
void Getopt::GetOption(char letter, char &value)
{
   if (!value_map[letter].empty())
    value = value_map[letter][0];
  else 
     value = default_map[letter][0];;
}

/**
 * Extracts a command line option.
 * 
 * \param letter letter for posix short option
 */
void Getopt::GetOption(char letter, int &value)
{
   if (!value_map[letter].empty())
      sscanf( value_map[letter].c_str(), "%d", &value);
   else 
      sscanf( default_map[letter].c_str(), "%d", &value);
}

/**
 * Extracts a command line option.
 * 
 * \param letter letter for posix short option
 */
void Getopt::GetOption(char letter, double &value)
{
   if (!value_map[letter].empty())
      sscanf( value_map[letter].c_str(), "%lf", &value);
   else
      sscanf( default_map[letter].c_str(), "%lf", &value);
}

/**
 * Extracts a command line option.
 * 
 * \param letter letter for posix short option
 */
void Getopt::GetOption(char letter, string &value)
{
   if (!value_map[letter].empty())
      value = value_map[letter];
   else
      value = default_map[letter];
}

} /* namespace QDLIB */



/**
 * Get a non-option string from the commandline.
 * 
 * \param number the index of the non-option string. starting with zero.
 * \param value  destination for the value
 * \param len    Maximum reserved space of value.
 */
bool QDLIB::Getopt::GetNonOption(int number, char *value, int len)
{
   if (((unsigned int) optind + number) < _argc ){
      strncpy(value, _argv[value_map.size() + 1 + number], len);
      return true;
   } else return false;
   
}


/**
 * Get a non-option string from the commandline.
 * 
 * \param number the index of the non-option string. starting with zero.
 * \param value  destination for the value
 * \param len    Maximum reserved space of value.
 */
bool QDLIB::Getopt::GetNonOption(int number, string &value)
{
   if (((unsigned int) optind + number) < _argc ){
      value = _argv[optind + number];
      return true;
   } else return false;
   
}

