// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef PARAMCONTAINER_H
#define PARAMCONTAINER_H

#include <vector>
#include <iostream>
#include <map>

using namespace std;

#define MAX_LINE_LENGTH 2048

namespace QDLIB {

typedef map<string,string> string_map;


   /**
   * The ParamContainer class allows reading structed string lists.
   * 
   * The intention of the ParamContainer class is to provide an
   * easy hand over of input parameters no matter which of which type
   * they are.
   * 
   * String and boolean values are case sensitive (and also the keys).
   * The boolean values "true" and "false" have to be lower case.
   * 
   * \todo Add support for complex numbers?
   * \bug  GetNextValue() won't work if ResetPosition() is not called before Readout
   */
   class ParamContainer
   {
      public:
         typedef enum {t_undef, t_int, t_uint, t_size_t, t_double, t_bool, t_string} datatype;

      private:
         map<string, datatype> _type_map;
         string_map _param_map;
         string_map::iterator _it;
	 

      public:
	 
         static string Type2String(datatype type);
         static datatype String2Type(const string& type);

	 ParamContainer();
	 ParamContainer(const string_map params);
      
	 void Params(const string_map params);
         string_map& Params();
	 
	 bool GetValue(const string name, char *value);
	 bool GetValue(const string name, string &value);
	 bool GetValue(const string name, int &value);
	 bool GetValue(const string name, unsigned int &value);
	 bool GetValue(const string name, long int &value);
    bool GetValue(const string name, size_t &value);
	 bool GetValue(const string name, double &value);
	 bool GetValue(const string name, bool &value, bool def = false);
         
	 bool GetArray(const string name, vector<string>& array);
	 bool GetArray(const string name, vector<double>& array);
	 bool GetArray(const string name, vector<int>& array);
    bool GetArray(const string name, vector<char>& array);

	 bool GetArray(const string name, vector<unsigned int>& array);
	 bool GetArray(const string name, vector<bool>& array);
	 
	 void ResetPosition();
	 bool GetNextValue(string &name, string &value);
	 
	 bool isPresent(const string &name);
	 bool isPresent(const char *name);

	 bool isType(const string &name, datatype type);

	 bool isType(const string &name, const string& type);
	 datatype GetType(const string &name);
	 
	 void clear();
	 
	 size_t Size() { return _param_map.size(); }

	 void SetValue(const string name, const char *value, datatype type = t_undef);
	 void SetValue(const string name, const string &value, datatype type = t_undef);
	 void SetValue(const string name, const int &value);
	 void SetValue(const string name, const long int &value);
    void SetValue(const string name, const size_t &value);
	 void SetValue(const string name, const double &value);
	 void SetValue(const string name, const bool &value);

	 void SetArray(const string& name, const double* values, size_t n);
	 void SetArray(const string& name, const int* values, size_t n);
	 void SetArray(const string& name, const size_t* values, size_t n);
	 void SetArray(const string& name, const bool* values, size_t n);

	 ParamContainer& operator=(const ParamContainer &params);
	 ParamContainer& operator+=(const ParamContainer &params);

    void Parse(istream &stream, const char* eof = NULL);
    void Parse(const string &buffer);
    bool ReadFromFile(const string& name);

    void Write(ostream& stream, bool with_types = false);
    void Write(string& buffer, bool with_types = false);
    bool WriteToFile(const string& name, bool with_types = false);

   }; /* class ParamContainer */

   
   /**
    * ASCII output for ParamContainers.
    */
   std::ostream& operator<<(std::ostream &s, ParamContainer &p);
   
   
} /* namespace QDLIB */

#endif /* #ifndef PARAMCONTAINER_H */

