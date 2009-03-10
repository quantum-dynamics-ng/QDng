#include "Logger.h"

#include "Exception.h"

namespace QDLIB {
   
   
   /* Singleton ref intialization */
   Logger* Logger::_ref = 0;
   
   
   Logger() : _global_out(cout), _supress(false), _laststream(0)
   {
      /* Set the string objects for the output streams */
      _cout.str(_sout);
      _coutdbg.str(_soutdbg);
   }
   
   Logger::~ Logger()
   {
      flush();
      _ofile.close();
   }

   
   Messenger * Logger::Instance()
   {
      if(!_ref)
	 _ref = new Logger();
      
      return _ref;
   }
   
   void Logger::InstanceRef()
   {
      Logger *ref;
      
      ref = Instance();
      
      return *ref;
   }
   
   
   void Logger::FileOutput(string & filename)
   {
      _ofile.open(filename.c_str());
      if( _ofile.fail() ) {
	 string msg;
	 msg = "Can not open " + filename +  " for output redirection";
	 throw ( EIOError(msg.c_str()) );
      }

      _global_out = _ofile;
   }

   stringstream& Logger::cout ( )
   {
      if (_laststream != 1){
	 flush();
	 _laststream = 1;
      }
	 
      return _cout;
   }
   
   stringstream& coutdbg ( )
   {
      if (_laststream != 2){
	 flush();
	 _laststream = 2;
      }
      
      return _coutdbg;
   }
   
   void Logger::flush()
   {
      if (_supress) return;
      
      if (_laststream == 1){
	 _global_out << _sout;
	 _sout.clear();
      }
      if (_laststream == 2){
	 _global_out << _soutdbg;
	 _soutdbg.clear();
      }
   }
   
}





