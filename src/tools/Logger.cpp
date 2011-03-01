#include "Logger.h"

#include "Exception.h"

namespace QDLIB {
   
   
   /* Singleton ref intialization */
   Logger* Logger::_ref = 0;
   
   
   Logger::Logger() :_laststream(0), _supress(false), _debug(false),
                     _line_length(LOGGER_LINE_LENGTH),  _indent(0),
                     _global_out(&std::cout)
   {
      /* Set the string objects for the output streams */
      _sout = _cout.rdbuf();
      _soutdbg = _coutdbg.rdbuf();
   }
   
   /**
    * Add indentation to string.
    */
   void Logger::_IndentString(string &s)
   {
      string sind;
      
      for (int i=0; i < _indent * LOGGER_INDENT_WIDTH; i++)
      {
	 sind += " ";
      }
      
      unsigned long int spos;
      
      s.insert(0, sind);
      spos = s.find("\n");
      while (spos != string::npos && spos+1 < s.length()){
	 s.insert(spos+1, sind);
	 if (spos+1 >= s.length())
	    spos = string::npos;
	 else
	    spos = s.find("\n", spos+1);
	 
      }
   }
   
   /**
    * Close the logger.
    * 
    * Flushes the output and closes open files.
    * Destroys the singleton.
    * 
    */
   void Logger::Close()
   {
      flush();
      _ofile.close();
      delete _ref;
      _ref = 0;
   }
   
   /**
    * return instance for singleton
    * @return Messenger*
    */
   Logger* Logger::Instance()
   {
      if(!_ref)
	 _ref = new Logger();
      
      return _ref;
   }
   
   /**
    * Return an instance
    */
   Logger& Logger::InstanceRef()
   {
      Logger *ref;
      
      ref = Instance();
      
      return *ref;
   }
   
   
   /**
    * Redirect output to file.
    * 
    * @param  filename
    */
   void Logger::FileOutput(string & filename)
   {
      _ofile.open(filename.c_str());
      if( _ofile.fail() ) {
	 string msg;
	 msg = "Can not open " + filename +  " for output redirection";
	 throw ( EIOError(msg.c_str()) );
      }

      _global_out = &_ofile;
   }

   /**
    * Close output file and switch back to console output.
    */
   void Logger::FileClose()
   {
      flush();
      _ofile.close();
      _global_out = &std::cout;
   }
   
   
   /** 
    * Set redirect to arbitrary stream.
    */
   void Logger::SetRedirect(ostream &redirect)
   {
      _global_out = &redirect;
   }
   
   /**
    * Set output to console. This is the default behavior.
    */
   void Logger::ConsoleOutput()
   {
      if ( _ofile.is_open() ) FileClose();
      _global_out = &std::cout;
   }
   
   /**
    * Returns the standard output stream.
    */
   stringstream& Logger::cout ( )
   {
      if (_laststream != 1){
	 flush();
	 _laststream = 1;
      }
	 
      return _cout;
   }
   
   /**
    * Returns the standard output debug stream.
    * 
    * Use this for very detailed output.
    */
   stringstream& Logger::coutdbg ( )
   {
      if (_laststream != 2){
	 flush();
	 _laststream = 2;
      }
      return _coutdbg;
   }
   
   
   /**
    * Flush the all text messages to the output
    */
   void Logger::flush()
   {
      if (_supress){
         _sout->str("");
         _soutdbg->str("");
         return;
      }
      
      string s;
      
      if (_laststream == 1){ /* Write standard output stream */
	 s = _sout->str();
	 _IndentString(s);
	 *_global_out << s;
	 _sout->str("");
      }
      if (_laststream == 2){/* Write debug output stream */
	 s = _soutdbg->str();
	 _IndentString(s);
         if (_debug) {
	    s = _soutdbg->str();
	    _IndentString(s);
	    *_global_out << s;
	 }
	 _soutdbg->str("");
      }
      _global_out->flush();
      _laststream = 0;
   }
   
   /**
    * Create a formated section header.
    */
   void Logger::Header(const string &title, SectionType type)
   {
      flush();
      switch(type){
	 case Chapter:
	       *_global_out << "\n\n";
	       for(int i=0; i < _line_length; i++)
		  *_global_out << "*";
	       *_global_out << "\n";
	       for(unsigned long int i=0; i < (_line_length-title.length())/2; i++)
		  *_global_out << " ";
	       *_global_out << title<<endl;
	       for(int i=0; i < _line_length; i++)
		  *_global_out << "*";
	       *_global_out << "\n\n";
	    break;
	 case Section:
	       *_global_out << "\n*";
	       for(unsigned long int i=0; i < (_line_length-title.length())/2-2; i++)
		  *_global_out << "*";
	       *_global_out << " ";
	       *_global_out << title << " ";
	       for(unsigned long int i=0; i < (_line_length-title.length())/2-2; i++)
		  *_global_out << "*";
	       *_global_out << "\n\n";
	    break;
	 case SubSection:
	    *_global_out << "* ";
	    *_global_out << title << endl<< endl;
	    break;
      }
   }
   
   /**
    * Create a formated section header.
    */
   void Logger::Header(const char *title, SectionType type)
   {
      string stitle(title);
      
      Header(stitle, type);
   }
}


