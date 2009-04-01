

#include "XmlNode.h"

namespace QDLIB {

   XmlNode::XmlNode()
   {
   }
   
   
   XmlNode::~XmlNode()
   {
      while(! _child_stack.empty()){
	 delete _child_stack.top();
	 _child_stack.pop();
      }
      while(! _name_stack.empty()){
	 delete _name_stack.top();
	 _name_stack.pop();
      }
      while(! _attr_stack.empty()){
	 delete _attr_stack.top();
	 _attr_stack.pop();
      }      
   }
   
   /**
    * Init the node with raw C pointer.
    * 
    * This is only needed by the XmlParser.
    */
   void XmlNode::Init( xmlNode *node )
   {
      _cur_node = node;
   }
   
   /**
    * Return the next child node.
    */
   XmlNode* XmlNode::NextChild()
   {
      if (_cur_node->children != NULL) {
	 XmlNode *child = new XmlNode();
	 child->_cur_node = _cur_node->children;
	 _child_stack.push(child);
	 return child;
      } else
	 return NULL;
      
   }
   
   /**
    * \return false if no children left in list.
    */
   bool XmlNode::EndChild()
   {
      if (_cur_node->children == NULL)
	 return false;
      else
	 return true;
   }
   
   /**
    * Step over to the next ELEMENT node.
    */
   void XmlNode::NextNode()
   {
      _cur_node = _cur_node->next;
      while ( _cur_node != NULL ){
	 if ( _cur_node->type == XML_ELEMENT_NODE ) break;
         _cur_node = _cur_node->next;
      }
	 
   }
	
   /**
    * Step over to the next ELEMENT node.
    * 
    */
   void XmlNode::AdjustElementNode()
   {
      if ( _cur_node->type != XML_ELEMENT_NODE ) NextNode();
   }   
   
   /**
    * \return false if no further element nodes left in the list.
    */   
   bool XmlNode::EndNode()
   {
      if (_cur_node == NULL)
	 return false;
      else
	 return true;
   }
   
   /**
    * Find a named element in the same level.
    */
   XmlNode * QDLIB::XmlNode::FindNode( const string & name )
   {
      xmlNode *buf = _cur_node;
      XmlNode *r = NULL;
      
      while ( buf != NULL ){
	 if (name == (char*) buf->name ) {
	    r = new XmlNode();
	    r->_cur_node = buf;
	    break;
	 }  
	 buf = buf->next;
      }
      return r;
   }
   
   /**
    * Get the node name.
    */
   string& XmlNode::Name()
   {
      string *s = new string((char*) _cur_node->name);
      _name_stack.push(s);
      return *s;
   }
      

   /**
    * Get the attributes of an element node.
    */
   ParamContainer& XmlNode::Attributes()
   {
      ParamContainer *p = new ParamContainer();
      xmlAttr *attr;
      
      _attr_stack.push(p);
      
      if (_cur_node->properties != NULL){
	 attr = _cur_node->properties;
	 while(attr != NULL){
	    if (attr->type == XML_ATTRIBUTE_NODE){
	       if (attr->children != NULL){
		  p->SetValue(string((char*) attr->name), string((char*) attr->children->content));
// 		  cout << (char*) attr->name << " = " << (char*) attr->children->content << endl;
	       } else {
		  p->SetValue(string((char*) attr->name), "");
	       }
	    }
	 attr = attr->next;
	 }
      }
      
      return *p;
   }

}
