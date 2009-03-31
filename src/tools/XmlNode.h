#ifndef QDLIBXMLNODE_H
#define QDLIBXMLNODE_H

#include <stack>

#include "libxml/parser.h"
#ifndef LIBXML_TREE_ENABLED
#error "LIBXML_TREE_ENABLED not enabled. I rely on that feature"
#endif

#include "ParamContainer.h"

namespace QDLIB
{

   /**
    * Represents a XML node.
    *
    * All the functions generating new child notes register it.
    * Thus all the generatetd nodes will be destroyed in the destructor.
    * 
    * \author Markus Kowalewski
    */
   class XmlNode
   {

      private:
         xmlNode *_cur_node;
         stack<XmlNode*> _child_stack;
	 stack<string*> _name_stack;
	 stack<ParamContainer*> _attr_stack;
      public:
         XmlNode();

         ~XmlNode();

         void Init( xmlNode *node );
         
	 XmlNode* NextChild();
	 bool EndChild();
	 
         void NextNode();
	 void AdjustElementNode();
	 
	 bool EndNode();

	 XmlNode* FindNode( const string &name);
	 
         string& Name();
         ParamContainer& Attributes();

   };

}

#endif
