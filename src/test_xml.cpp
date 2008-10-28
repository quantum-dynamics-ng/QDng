#include "sys/XmlNode.h"
#include "sys/XmlParser.h"

using namespace QDLIB;
using namespace std;

void print_names(XmlNode *node)
{
   int i = 0;
   XmlNode *child;
   if (node != NULL){
      while( node->EndNode() ){
	 cout << node->Name() << endl;
	 node->Attributes();
	 child = node->NextChild();
	 if (child) print_names(node->NextChild());
	 
	 node->NextNode();
	 if (i > 5) break;
	 i++;
      }
   }

}

int main(int argc, char **argv)
{
   XmlParser xml;
   XmlNode root;
   string s(argv[1]);
   cout << XML_ELEMENT_NODE << endl;

   
   xml.Parse(s);
   root = xml.Root();
   print_names(&root);
   
}
   
