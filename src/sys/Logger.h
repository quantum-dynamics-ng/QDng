#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <fstream>

using namespace std;

namespace QDLIB {
   /**
    * Global messenger for centralized output.
    * 
    * This is a singleton class.
    */
   class Logger
   {
      private:
	 stringstream _cout;
	 stringstream _coutdbg;
	 stringbuf *_sout;
	 stringbuf *_soutdbg;
	 
	 int _laststream; /* 0: no one, 1: cout , 2: coutdbg */
	 
	 bool _supress;
	 bool _debug;
	 
	 ofstream _ofile;
	 
	 static Logger *_ref;
	 
	 ostream  *_global_out;
	 
	 Logger ( );
	 
         /**
	  * Empty Copy constructor
          */
	 //Logger (Logger &L) : _global_out(cout) {}

   
         /**
	  * Private destructor.
          */
	 ~Logger ( );

   public:
      
      static Logger* Instance ();

      static Logger& InstanceRef ();
   
      /**
      * Set a function which is called with every flush()
      * @param  notifyfunction Function pointer to notfiy function
      */
      //void SetNotify (ftpr notifyfunction );
   
      void FileOutput (string& filename );
      
      void SetRedirect (ostream &redirect);
      
      stringstream& cout ( );
   
      stringstream& coutdbg ( );

   
      /**
       * @param  Caller
       */
//       stringstream& cout (Operator* Caller );
   
   
      /**
       * @param  Caller
       */
//       stringstream& cout (WaveFunction* Caller );
  
      /**
      * @param  Caller
      */
//       stringstream& coutdbg (Operator* Caller );
   
   
      /**
      * @param  Caller
      */
//       stringstream& coutdbg (WaveFunction* Caller );
   
      void flush ();
   
   
      /**
      * Supress all messages.
      * Everything will be send to void if flush is called
      */
      void SupressOn ( ) { _supress = true;}
   
   
      /**
      * Turn messaging on.
      */
      void SupressOff ( ){ _supress = false;}
   
   
      /**
      * @return bool
      */
      bool Supress ( ) { return _supress; }
      
      void Debug(bool on) { _debug = on; }
      
   };

   
}
#endif // LOGGER_H
