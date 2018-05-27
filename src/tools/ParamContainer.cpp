#include <sstream>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include "ParamContainer.h"
#include "tools/helpers.h"


namespace QDLIB {

   /**
    * Translate a enum into textual representation.
    */
   string ParamContainer::Type2String(datatype type)
   {
      switch (type){
         case t_undef:
            return string();
         case t_int:
            return string("int");
         case t_uint:
            return string("uint");
         case t_size_t:
            return string("sizet");
         case t_double:
            return string("double");
         case t_bool:
            return string("bool");
         case t_string:
            return string("string");
      }

      return string();
   }

   /**
    * Translate the textual representation of the datatype into the enum.
    */
   ParamContainer::datatype ParamContainer::String2Type(const string& type)
   {
      if (type == "")
         return t_undef;
      else if (type == "int")
         return t_int;
      else if (type == "uint")
         return t_uint;
      else if (type == "sizet")
         return t_size_t;
      else if (type == "double")
         return t_double;
      else if (type == "bool")
         return t_bool;
      else if (type == "string")
         return t_string;
      else
         throw (EParamProblem("Unknown type"));
   }

   ParamContainer::ParamContainer()
   {
      _it = _param_map.begin();
   }


   /**
   * Constructor including Initialisation.
   *
   * \param params carries the information
   */
   ParamContainer::ParamContainer(const string_map params)
   {
         _param_map = params;
	 _it = _param_map.begin();
   }

   /**
    * Return the actual parameter set.
    */
   string_map& ParamContainer::Params()
   {
      return _param_map;
   }

   /**
   * Replaces old parameter set.
   *
   * \param params carries the information
   */
   void ParamContainer::Params(const string_map params)
   {
         _param_map = params;
   }

   /**
    * Get a string (Null terminated C-string.
    *
    * No size checking is done! Use this variant with care.
    *
    * \param name  Name of parameter
    * \param value Refernze to value
    *
    * \return true if value exist
    *
    */
   bool ParamContainer::GetValue(const string name, char *value)
   {
      if ( _param_map.find(name) != _param_map.end()){
	 strcpy(value, _param_map[name].c_str());
	 return true;
      }
      else {
	 value[0] = '\0';
	 return false;
      }
   }

   /**
   * Get a string.
   *
   * \param name  Name of parameter
   * \param value Refernze to value
   *
   * \return true if value exist
   */
   bool ParamContainer::GetValue(const string name, string &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
	 value = _param_map[name];
	 return true;
      }
      else {
	 value.clear();
	 return false;
      }
   }


   /**
   * Get an integer value.
   *
   * \param name  Name of parameter
   * \param value Referenze to value
   */
   bool ParamContainer::GetValue(const string name, int &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         sscanf( _param_map[name].c_str(), "%d", &value);
         return true;
      }
      else {
         value = 0;
         return false;
      }
   }

   /**
   * Get an unsigned integer value.
   *
   * \param name  Name of parameter
   * \param value Reference to value
   */
   bool ParamContainer::GetValue(const string name, unsigned int &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         sscanf( _param_map[name].c_str(), "%u", &value);
         return true;
      }
      else {
         value = 0;
         return false;
      }
   }


   /**
   * Get an long int value.
   *
   * \param name  Name of parameter
   * \param value Referenze to value
   */
   bool ParamContainer::GetValue(const string name, long int &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         sscanf( _param_map[name].c_str(), "%ld", &value);
         return true;
      }
      else {
         value = 0;
         return false;
      }
   }

   /**
    * Get an size_t value.
    *
    * \param name  Name of parameter
    * \param value Referenze to value
    */
   bool ParamContainer::GetValue(const string name, size_t &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         sscanf( _param_map[name].c_str(), "%lu", &value);
         return true;
      }
      else {
         value = 0;
         return false;
      }
   }


   /**
   * Get an long int value.
   *
   * \param name  Name of parameter
   * \param value Referenze to value
   */
   bool ParamContainer::GetValue(const string name, double &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         sscanf( _param_map[name].c_str(), "%lf", &value);
         return true;
      }
      else {
         value = 0;
         return false;
      }

   }

   /**
    * Get a boolean value.
    *
    * If the value is not written correct ("true") then result will be the default value.
    *
    * \param name  Name of parameter
    * \param value Referenze to value
    * \param def   Set the default value for the case key is not found
    */
   bool ParamContainer::GetValue(const string name, bool &value, bool def)
   {
      if ( _param_map.find(name) != _param_map.end()){
         if (_param_map[name] == string("true")) value = true;
         else if (_param_map[name] == string("false")) value = false;
         else value=def; /* Value misspelled or empty */
         return true;
      } /* value not found */
      else {
         value=def;
         return false;
      }
   }


   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<string>& array)
   {
      string buf;
      if (! GetValue(name, buf) ) return false;

      split(buf, array);

      return true;
   }

   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<double>& array)
   {
      string buf;
      vector<string> vbuf;

      if (! GetValue(name, buf) ) return false;

      split(buf, vbuf);

      for (size_t i=0; i < vbuf.size(); i++){
         double value;
         sscanf( vbuf[i].c_str(), "%lf", &value);
         array.push_back(value);
      }

      return true;
   }

   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<int>& array)
   {
      string buf;
      vector<string> vbuf;

      if (! GetValue(name, buf) ) return false;

      split(buf, vbuf);

      for (size_t i=0; i < vbuf.size(); i++){
         int value;
         sscanf( vbuf[i].c_str(), "%d", &value);
         array.push_back(value);
      }

      return true;
   }

   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<unsigned int>& array)
   {
      string buf;
      vector<string> vbuf;

      if (! GetValue(name, buf) ) return false;

      split(buf, vbuf);

      for (size_t i=0; i < vbuf.size(); i++){
         unsigned int value;
         sscanf( vbuf[i].c_str(), "%u", &value);
         array.push_back(value);
      }

      return true;
   }

   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<char>& array)
   {
      string buf;
      vector<string> vbuf;

      if (! GetValue(name, buf) ) return false;

      split(buf, vbuf);

      for (size_t i=0; i < vbuf.size(); i++){
         array.push_back(vbuf[i][0]);
      }

      return true;
   }

   /**
    * Extract an array from an Entry.
    * The array elements are separated by kommas.
    */
   bool ParamContainer::GetArray(const string name, vector<bool>& array)
   {
      string buf;
      vector<string> vbuf;

      if (! GetValue(name, buf) ) return false;

      split(buf, vbuf);

      for (size_t i=0; i < vbuf.size(); i++){
         bool value;
         if (vbuf[i] == "true") value=true;
         else value = false;
         array.push_back(value);
      }

      return true;
   }



   /**
    * Reset the position pointer.
    */
   void ParamContainer::ResetPosition()
   {
     _it = _param_map.begin();
   }

   /**
    * Get the next key, value pair in the list.
    *
    * \param name    Entry name/key
    * \param value   Value of the entry
    *
    * \return  false if end of list.
    *
    * \see ResetPosition
    */
   bool ParamContainer::GetNextValue(string &name, string &value)
   {
      if (_it != _param_map.end()){
	 name  = (*_it).first;
	 value  = (*_it).second;
	 _it++;
	 return true;
      }
      return false;

   }

   /**
    * Check if a value is in the container.
    *
    * \return true if value is there.
    */
   bool ParamContainer::isPresent(const string &name)
   {
      if ( _param_map.find(name) != _param_map.end() )
	 return true;
      else
	 return false;
   }

   /**
    * Check if a value is in the container.
    *
    * \return true if value is there.
    */
   bool ParamContainer::isPresent(const char *name)
   {
      if ( _param_map.find(string(name)) != _param_map.end() )
	 return true;
      else
	 return false;
   }

   /**
    * Check is entry has a certain data type.
    */
   bool ParamContainer::isType(const string &name, datatype type)
   {
      if ( _type_map.find(name) != _type_map.end())
         if(_type_map[name] == type) return true;

      return false;
   }

   /**
    * Check is entry has a certain data type.
    */
   bool ParamContainer::isType(const string &name, const string& type)
   {
      if ( _type_map.find(name) != _type_map.end())
         if(_type_map[name] == String2Type(type)) return true;

      return false;
   }

   /**
    * Get datatype for an entry.
    */
   ParamContainer::datatype ParamContainer::GetType(const string &name)
   {
      if ( _type_map.find(name) != _type_map.end())
         return _type_map[name];

      return t_undef;
   }

   /**
    * Remove all parameters from the container.
    */
   void ParamContainer::clear()
   {
      _param_map.clear();
      _type_map.clear();
   }


   /**
    * Set a string value (Null terminated C-string).
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const char *value, datatype type)
   {
      _type_map[name] = type;
      _param_map[name] = string(value);
   }

   /**
    * Set a string value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const string &value, datatype type)
   {
     _type_map[name] = type;
     _param_map[name] = value;

   }

   /**
    * Set an integer value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const int &value)
   {
     stringstream ss;

     ss << value;
     ss >> _param_map[name];

     _type_map[name] = t_int;
   }

   /**
    * Set a long int value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const long int &value)
   {
     stringstream ss;

     ss << value;
     ss >> _param_map[name];

     _type_map[name] = t_int;
   }

   /**
    * Set a size_t  value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const size_t &value)
   {
     stringstream ss;

     ss << value;
     ss >> _param_map[name];

     _type_map[name] = t_size_t;
   }


   /**
    * Set a double value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name  Name of parameter
    * \param value Referenze to value
    *
    * TODO: check for number of digits (formating)
    */
   void ParamContainer::SetValue(const string name, const double &value)
   {
     stringstream ss;

     ss << value;
     ss >> _param_map[name];

     _type_map[name] = t_double;
   }

   /**
    * Set a bool value.
    *
    * If the the name exists, the value is simply replaced,
    * if not a new entry will be added.
    *
    * \param name Name of parameter
    * \param value Referenze to value
    */
   void ParamContainer::SetValue(const string name, const bool &value)
   {
      if (value == true) _param_map[name] = "true";
      else _param_map[name] = "false";

      _type_map[name] = t_bool;
   }

   /**
    * Creates a sequence of values in one one entry.
    */
   void ParamContainer::SetArray(const string& name, const double* values, size_t n)
   {
      stringstream cat;

      for (size_t i=0; i < n; i++){
         cat << values[i];
         if (i != n-1) cat << ",";
      }

      SetValue(name, cat.str(), t_double);
   }

   /**
    * Creates a sequence of values in one one entry.
    */
   void ParamContainer::SetArray(const string& name, const int* values, size_t n)
   {
      stringstream cat;

      for (size_t i=0; i < n; i++){
         cat << values[i];
         if (i != n-1) cat << ",";
      }

      SetValue(name, cat.str(), t_int);
   }

   /**
    * Creates a sequence of values in one one entry.
    */
   void ParamContainer::SetArray(const string& name, const size_t* values, size_t n)
   {
      stringstream cat;

      for (size_t i=0; i < n; i++){
         cat << values[i];
         if (i != n-1) cat << ",";
      }

      SetValue(name, cat.str(), t_size_t);
   }

   /**
    * Creates a sequence of values in one one entry.
    */
   void ParamContainer::SetArray(const string& name, const bool* values, size_t n)
   {
      stringstream cat;

      for (size_t i=0; i < n; i++){
         if (values[i] == true) cat << "true";
         else cat << "false";
         if (i != n-1) cat << ",";
      }

      SetValue(name, cat.str(), t_bool);
   }


   /**
    * Copy operator.
    */
   ParamContainer& ParamContainer::operator=(const ParamContainer &params)
   {

      _param_map = params._param_map;
      _type_map = params._type_map;

      _it = _param_map.begin();
      return *this;
   }

   /**
    * Add operator.
    *
    * Merges the two Containers. Entries may be overridden by the rhs argument.
    */
   ParamContainer& ParamContainer::operator+=(const ParamContainer &params)
   {

      string_map addpmap = params._param_map;

      for (string_map::iterator it = addpmap.begin(); it != addpmap.end(); it++)
      {
         _param_map[it->first] = it->second;
      }

      map<string, datatype> addtmap = params._type_map;

      for (map<string, datatype>::iterator it = addtmap.begin(); it != addtmap.end(); it++)
      {
         _type_map[it->first] = it->second;
      }

      _it = _param_map.begin();

      return *this;
   }

   /**
    * Parse from string buffer.
    */
   void ParamContainer::Parse(const string &buffer)
   {
      stringstream ss;

      ss << buffer;
      Parse(ss);
   }


   /**
    * Parse ParamContainer from a stream.
    *
    * \param eof if eof is non-NULL then eof string marks the end.
    * otherwise it will continue reading from the stream until a real end-of file is reached.
    */
   void ParamContainer::Parse(istream &stream, const char* eof)
   {
      string s;
      char line[MAX_LINE_LENGTH];
      size_t pos;

      clear();
      while (!stream.eof()) { // Reading loop
         stream.getline(line, MAX_LINE_LENGTH);
         //if (file.fail()) return false;

         s.assign(line);

         if (eof != NULL)
            if (s == eof) break;

         pos = s.find("="); // Delimiter between Key Val is always =

         if (pos != s.npos) { // Check if line is valid
            string key, val;
            key = s.substr(0, pos);
            val = s.substr(pos + 1);

            trim(key); // remove whitespaces
            trim(val);

            /* look for type definition */
            ParamContainer::datatype type = ParamContainer::t_undef;
            size_t tpos1 = key.find("[");
            size_t tpos2 = key.find("]");

            if (tpos1 != key.npos) { /* extract typedef */
               string stype;
               if (tpos2 != key.npos)
                  stype = key.substr(tpos1+1, tpos1);
               else
                  stype = key.substr(tpos1+1);

               type = String2Type(stype);

               key = key.substr(0, tpos1);
               trim(key);
            }

            /*  At least the key has to contain something
             * Lines begining with "#" are treated as comment
             */
            if (!key.empty() && key.find("#") != 0) {
               SetValue(key, val, type);
            }
         }
      }
   }

   bool ParamContainer::ReadFromFile(const string& name)
   {
      fstream file;

      file.open(name.c_str());           // Open file
      if (file.fail()) return false;

      Parse(file);

      return true;

   }


   void ParamContainer::Write(string& buffer, bool with_types)
   {
      stringstream ss;

      Write(ss, with_types);

      buffer = ss.str();
   }

   /**
    * Write Key value pairs to stream.
    */
   void ParamContainer::Write(ostream& stream, bool with_types)
   {
      string key,value;

      //stream << "# Generated file" << endl;

      ResetPosition();

      /* Punch out key-value pairs */
      while (GetNextValue(key, value)) {
         if (!with_types)
            stream << key << " = " << value << endl;
         else {
            datatype type = GetType(key);

            if (type == t_undef)
               stream << key << " = " << value << endl; /* undefined type => no type */
            else
               stream << key << "[" << ParamContainer::Type2String(type) << "] = " << value << endl;
         }
      }
   }



   /**
    * Write a KeyValue File
    *
    * \param name path
    * \param with_types add type info to keys
    *
    * \return true if written correct
    */
   bool ParamContainer::WriteToFile(const string& name, bool with_types)
   {
      ofstream file;

      file.open(name.c_str(),  ios_base::out);
      if( file.bad() ) return false;

      Write(file, with_types);

      file.close();
      if( file.bad() ) return false;

      return true;
   }


   /**
    * ASCII output for ParamContainers.
    */
   std::ostream& operator<<(std::ostream &s, ParamContainer &p)
   {
      string key, value;

      p.Write(s, true);

      return s;
   }


} /* namespace QDLIB */
