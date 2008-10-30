#include <sstream>

#include "ParamContainer.h"

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
    * If the value is not written correct ("true") then result will be "false".
    * 
    * \param name  Name of parameter
    * \param value Referenze to value
    */
   bool ParamContainer::GetValue(const string name, bool &value)
   {
      if ( _param_map.find(name) != _param_map.end()){
         if (_param_map[name] == "true") value = true;
	 else value = false;
         return true;
      }
      else {
         value = 0;
         return false;
      }
   
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
   bool ParamContainer::isPresent(string &name)
   {
      if ( _param_map.find(name) != _param_map.end() )
	 return true;
      else
	 return false;
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
   ParamContainer& ParamContainer::operator=(ParamContainer &params)
   {

      _param_map = params._param_map;
 
      _it = _param_map.begin();
      return *this;
   }
      
   
} /* namespace QDLIB */
