#include "KeyValFile.h"

#include <fstream>
namespace QDLIB {
   
   KeyValFile::KeyValFile() : _name(""), _pos(0) {}
   
   /**
    * Constructor with file name initialisation.
    */
   KeyValFile::KeyValFile(const string name) : _name(name), _pos(0)  {}
   
   /**
    * Parses the whole file.
    */
   bool KeyValFile::Parse(ParamContainer &p)
   {
      string s,k,v;
      char line[MAX_LINE_LENGTH];
      size_t pos;
      
      ifstream file;
      
      file.open(_name.c_str());           // Open file
      if (file.fail()) return false;
      
      
      while(!file.eof()){                     // Reading loop
	 file.getline(line, MAX_LINE_LENGTH);
	 //if (file.fail()) return false;
	 
	 s.assign(line);
	 
	 pos = s.find ("=");      // Delimiter between Key Val is always =
	 
	 if (pos != s.npos){      // Check if line is valid 
	    k = s.substr(0,pos);
	    v = s.substr(pos+1);
	 
	    trim(k);              // remove whitespaces
	    trim(v);
	    
            /*  At least the key has to contain something
	     * Lines begining with "#" are treated as comment
	     */
	    if ( !k.empty() && k.find("#") != 0 ){
	       p.SetValue(k,v);
	    }
	 }
      }
      
      return true;
  }
   
   /**
    * Write a KeyValue File
    * 
    * \param p ParamContainer with KeyValue Set
    * 
    * \return true if written correct
    */
   bool KeyValFile::Write(ParamContainer &p)
   {
      ofstream file;
      string key,value;
      
      file.open(_name.c_str(),  ios_base::out);
      if( file.bad() ) return false;
      
      file << "# Generated file" << endl;
      
      p.ResetPosition();
      
      while ( p.GetNextValue(key,value) ){
	 file << key << " = " << value << endl;
      }
      
      file.close();
      if( file.bad() ) return false;
      
      return true;
   }
    
   /**
    * Set the file name.
    */
   void KeyValFile::SetName(const string name)
   {
      _name = name;
   }
   
   
} /* namespace QDLIB */
