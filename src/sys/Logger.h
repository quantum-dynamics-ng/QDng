#ifndef LOGGER_H
#define LOGGER_H

#include string

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
	 string _sout;
	 string _soutdbg;
	 
	 int _laststream; /* 0: no one, 1: cout , 2: coutdbg */
	 
	 bool _supress;
	 ofstream _ofile;
	 
	 static Logger *_ref;
	 
	 ostream _global_out;
	 
	 Logger ( );
	 
         /**
	  * Empty Copy constructor
          */
	 Logger (Logger &L) {}

   
         /**
	  * Private destructor.
          */
	 ~Logger ( );

   public:
      
      /**
       * return instance for singleton
       * @return Messenger*
       */
      static Messenger* Instance ();

      
      /**
       * Return an instance
       */
      static void InstanceRef ();
   
   
   
   
      /**
      * Set a function which is called with every flush()
      * @param  notifyfunction Function pointer to notfiy function
      */
      //void SetNotify (ftpr notifyfunction );
   
   
      /**
       * Redirect output to file.
       * 
       * @param  filename
       */
      void FileOutput (string& filename );
   
   
      /** 
       * Set redirect to arbitrary stream.
       */
      void SetRedirect (ostream &redirect) {_global_out = redirect;}
      
      
      /**
       * Returns the standard output stream.
       */
      stringstream& cout ( );
   
      /**
       * Returns the standard output debug stream.
       * 
       * Use this for very detailed output.
       */
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
   
   
      /**
      * Flush the all text messages to the output
      */
      void flush ( );
   
   
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
   };

   
}
#endif // LOGGER_H
