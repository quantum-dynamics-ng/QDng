#ifndef QDLIBCHAINLOADER_H
#define QDLIBCHAINLOADER_H

#include "qdlib/OPropagator.h"
#include "tools/XmlNode.h"
#include "qdlib/OSum.h"
#include "qdlib/OMultistate.h"

namespace QDLIB
{

   /**
    * Contains some methods to load Operators and WaveFunctions from XML input.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class ChainLoader
   {
      public:
	 static Operator* LoadOperatorChain( XmlNode *Onode );
	 static WaveFunction* LoadWaveFunctionChain( XmlNode *WFNode, bool empty=false);
	 static OPropagator* LoadPropagator( XmlNode *Unode, Operator **Hamiltonian);
   };

   /**
    * Find a specific type of operator in Chain of Sum/Multistate operators.
    */
   template <class C>
   Operator* FindOperatorType(Operator *O)
   {
      C* t;
      Operator* res = NULL;
	    
      t = dynamic_cast<C*>(O);
      if (t != NULL) return t;
      
      /* Crawl trough Sum operator*/
      OSum* sum;
      sum = dynamic_cast<OSum*>(O);
      if (sum != NULL){
	 for(int i=0; i < sum->Size(); i++){
	     res = FindOperatorType<C>((*sum)[i]);
	     if (res != NULL) return res;
	 }
      }
      
      /* Crawl trough Multistate operator*/
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*>(O);
      if (ms != NULL){
	 for(int i=0; i < ms->States(); i++){
	    for(int j=0; j < ms->States(); j++){
	       if (ms->State(i,j) != NULL){
		  res = FindOperatorType<C>(ms->State(i,j));
		  if (res != NULL) return res;
	       }
	    }
	 }
      }
      
      return NULL;
   }

} /* namespace QDLIB */

#endif
