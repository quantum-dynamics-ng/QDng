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
      
      void Parse(string &name);
      
      XmlNode& Root();

   
};

}

#endif
