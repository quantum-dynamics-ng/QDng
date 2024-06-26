// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "Simple2Xml.h"

#include <stdio.h>
#include <sys/errno.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include "tools/Exception.h"

#define CHUNK 1024

int yyparse();
extern FILE* yyin;
extern FILE* _yyout;
extern stringstream ss_yy_buf;

namespace QDSXML {

   /**
    * Check which file type we have (simpleXML or XML).
    * 
    * \param name path to *xml file.
    */
   FileType CheckType(const string & name)
   {
      FILE *file;
      char buf[CHUNK];
      char *ind = NULL;
      size_t size = CHUNK;
      
      /* Open file */
      file = fopen(name.c_str(), "r");
      
      if (!file)
         throw (QDLIB::EIOError(errno, name) );
      
      memset( (void*) buf, 0, CHUNK);
      
      /* Read a test chunk */
      size = fread( (void*) buf, 1, CHUNK, file );
      
      if ( size == 0 ) {
         throw (QDLIB::EIOError("Can't read from file: ", name) );
      }
      
      fclose(file);
      
      /* Look for file type indications - quick & dirty */
      bool simple = false;
      bool xml = false;
      
      /* Check for XML */
//      ind = strstr(buf, "<");
//      if (ind != NULL) xml = true;
      ind = strstr(buf, "/>");
      if (ind != NULL) xml |= true;
      ind = strstr(buf, "</");
      if (ind != NULL) xml |= true;
//      ind = strstr(buf, ">");
//      if (ind != NULL) xml &= true;
      
      if (xml) return XML;
      
      /* Check for XML */
      ind = strstr(buf, "{");
      if (ind != NULL) simple = true;
      ind = strstr(buf, "}");
      if (ind != NULL) simple &= true;
      
      if (simple) return SIMPLE;
      
      return UNKNOWN;
   }

   /**
    * Convert file from simple format to clean XML
    */
   void Convert(const string & iname, const string & oname)
   {
      /* Open input file */
      yyin = fopen(iname.c_str(), "r");
      
      if (!yyin)
         throw (QDLIB::EIOError(errno, iname) );
      
      /* Open output file */
      std::ofstream of(oname.c_str(), std::ofstream::trunc);
      //_yyout = fopen(oname.c_str(), "w");
      
      //if (!_yyout)
      if (! of.is_open())
         throw (QDLIB::EIOError(errno, oname) );

      
      yyparse();

      of << ss_yy_buf.str();
      //fclose(_yyout);
      of.close();
   }

   /**
    * Convert file from simple format to clean XML
    */
   void Parse(const string &iname, string &buf)
   {
      /* Open input file */
      yyin = fopen(iname.c_str(), "r");

      if (!yyin)
         throw (QDLIB::EIOError(errno, iname) );

      yyparse();

      buf = ss_yy_buf.str();
   }
}
