// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBXMLPARSER_H
#define QDLIBXMLPARSER_H

#include "XmlNode.h"

#include <libxml/parser.h>
#ifndef LIBXML_TREE_ENABLED
#error "LIBXML_TREE_ENABLED not enabled. I rely on that feature"
#endif


namespace QDLIB
{

   /**
    * Xml Parser class.
    * 
    */
   class XmlParser
   {
   private:
      xmlDoc *_doc;
      xmlNode *_root;
      XmlNode *_Xml_root;
   public:
      
      XmlParser();
	 
      ~XmlParser();
      
      void Parse(const string &name);
      void Parse(const char* buffer, int size);
      
      XmlNode& Root();

   
};

}

#endif
