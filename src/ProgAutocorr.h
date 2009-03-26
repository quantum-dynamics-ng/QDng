#ifndef QDNGPROGAUTOCORR_H
#define QDNGPROGAUTOCORR_H


namespace QDLIB {
   
   /**
    * Simple Module to make an auto correlation from an existing
    * Propagation.
    * 
    * XmlNode Attributes:
    * \li dt time step size
    * 
    */
   class ProgAutocorr {
      private:
	 XmlNode &_propaNode;
	 XmlNode *_ContentNodes;
 
	 int _wcycle;     /* Write and analyze rate */
	 Reporter _reporter;
	 string _fname;
	 
	 OPropagator *_U;
	 Operator *_H;
	 
	 void _InitParams();
	 void _InitSumOperator();
	 
      public:
	 ProgPropa(XmlNode &PropaNode);
	 ~ProgPropa();
	 
	 void Run();
   
   };
}

#endif

