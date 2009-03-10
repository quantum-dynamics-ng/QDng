#include "Logger.h"

#include "Exception.h"

namespace QDLIB {
   
   
   /* Singleton ref intialization */
   Logger* Logger::_ref = 0;
   
   
   Logger::Logger() : _global_out(&std::cout), _supress(false), _debug(false), _laststream(0)
   {
      /* Set the string objects for the output streams */
      _sout = _cout.rdbuf();
      _soutdbg = _coutdbg.rdbuf();
   }
   
   Logger::~ Logger()
   {
      flush();
      _ofile.close();
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
    * Set redirect to arbitrary stream.
    */
   void Logger::SetRedirect(ostream &redirect)
   {
      _global_out = &redirect;
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
      if (_supress) return;
      
      
      if (_laststream == 1){
	 *_global_out << _sout->str();
// 	 std::cout << _sout->str();
	 _sout->str("");
      }
      if (_laststream == 2){
	 if (_debug) *_global_out << _soutdbg->str();
	 _soutdbg->str("");
      }
      
      _laststream = 0;
   }
   
}





