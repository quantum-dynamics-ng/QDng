#include <sstream>
#include <string.h>
#include <stdio.h>
#include "ParamContainer.h"
#include "tools/helpers.h"


namespace QDLIB {
   
   
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
	 value = '\0';
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
	 value = '\0';
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
    * Remove all parameters from the container.
    */
   void QDLIB::ParamContainer::clear( )
   {
      _param_map.clear();
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
   void ParamContainer::SetValue(const string name, const char *value)
   {
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
   void ParamContainer::SetValue(const string name, const string &value)
   {
     
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
   }
   
   /**
    * Copy operator.
    */
   ParamContainer& ParamContainer::operator=(const ParamContainer &params)
   {

      _param_map = params._param_map;
 
      
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

      string_map::iterator it;
      string_map addmap = params._param_map;
      
      for (it = addmap.begin(); it != addmap.end(); it++)
      {
         _param_map[it->first] = it->second;
      }

      _it = _param_map.begin();

      return *this;
   }
   
   /**
    * ASCII output for ParamContainers.
    */
   std::ostream& operator<<(std::ostream &s, ParamContainer &p)
   {
      string key, value;
      
      p.ResetPosition();
      while ( p.GetNextValue( key, value ) ){
	 s << key << " = " << value << std::endl;
      }
      return s;
   }
   
   
} /* namespace QDLIB */
