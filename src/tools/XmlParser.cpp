#include "XmlParser.h"
#include "Exception.h"

namespace QDLIB
   {
   
      XmlParser::XmlParser() : _doc(NULL), _root(NULL), _Xml_root(NULL)
   {
      /* Taken from the expample - macro to check for version conflicts */
      LIBXML_TEST_VERSION;
   }
   
   /**
    * Do not destroy the parser until every value is in your own memory.
    */
   XmlParser::~XmlParser()
   {
      if (_Xml_root != NULL)
	 delete _Xml_root;
      xmlFreeDoc(_doc);
      xmlCleanupParser();
   }

   /**
    * Parse an XML input file.
    * 
    * \param name File name or URL
    */
   void XmlParser::Parse(string &name)
   {
      _doc = xmlReadFile(name.c_str(), NULL, XML_PARSE_NOBLANKS);

      if (_doc == NULL) {
	 throw ( EIOError("Can not parse XML file: ") );
      }

      /*Get the root element node */
      _root = xmlDocGetRootElement(_doc);
      if (_root == NULL) {
	 throw ( EIOError("Can not create XML root node: ") );
      }
      
   }

   /**
    * Get the root node
    */
   XmlNode& XmlParser::Root()
   {
      if(_Xml_root == NULL) {
	 _Xml_root = new XmlNode();
         _Xml_root->Init(_root);
      }
      return *_Xml_root;
   }
}
