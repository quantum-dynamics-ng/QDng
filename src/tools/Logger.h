#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <fstream>


#define LOGGER_LINE_LENGTH 80
#define LOGGER_INDENT_WIDTH 2

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
	 
	 int _line_length;
	 int _indent;
	 
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
	 ~Logger ( ){}

	 void _IndentString(string &s);
   public:
      
      enum  SectionType {Chapter, Section, SubSection};
	    
      static Logger* Instance ();

      static Logger& InstanceRef ();
   
      void Close();
      
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
   
      void Header(const string &title, SectionType type);
      
      void Header(const char *title, SectionType type);
      
      /**
       * Supress all messages.
       * 
       * Everything will be send to void if flush is called.
       */
      void Supress(bool supress) { _supress = supress;}
      
      /**
      * Ask for output supression
      */
      bool Supress ( ) { return _supress; }
      
      /**
       * Turn on/off more detailed output.
       */
      void Debug(bool on) { _debug = on; }
      
      /**
       * Ask for debug output.
       */
      bool Debug() { return _debug; }
      
      /**
       * Set assumed line length.
       */
      void SetLineLength(int length) { _line_length = length; }
      
      /**
       * Set the number of indents.
       */
      void SetIndent(int indent) { if (indent >=0) _indent = indent;}
      
      /**
       * Increase Indent.
       */
      void IndentInc() { _indent++;}
      
      /**
       * Decrease Indent.
       */
      void IndentDec() { if (_indent>0) _indent--;}

   };

   
}
#endif // LOGGER_H
