#ifndef PARAMCONTAINER_H
#define PARAMCONTAINER_H

#include <iostream>
#include <map>

using namespace std;



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
      private:
	 string_map _param_map;
         string_map::iterator _it;
	 
      public:
	 

	 ParamContainer();
	 ParamContainer(const string_map params);
      
	 void Params(const string_map params);
         string_map& Params();
	 
	 bool GetValue(const string name, string &value);
	 bool GetValue(const string name, int &value);
	 bool GetValue(const string name, long int &value);
	 bool GetValue(const string name, double &value);
	 bool GetValue(const string name, bool &value);
         
	 
	 void ResetPosition();
	 bool GetNextValue(string &name, string &value);
	 
	 bool isPresent(string &name);
	 
	 void SetValue(const string name, const string &value);
	 void SetValue(const string name, const int &value);
	 void SetValue(const string name, const long int &value);
	 void SetValue(const string name, const double &value);
	 void SetValue(const string name, const bool &value);
         
	 ParamContainer& operator=(ParamContainer &params);

   }; /* class ParamContainer */

} /* namespace QDLIB */

#endif /* #ifndef PARAMCONTAINER_H */

