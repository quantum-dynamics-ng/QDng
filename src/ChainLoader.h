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
    * 
    * \param O       The Operator to inspect.
    * \param found   If type is found the naked operator will be placed here
    * 
    * \return The whole Operator with packaging (single state or multistate)
    */
   template <class C>
   Operator* FindOperatorType(Operator *O, C **found)
   {
      C* t;
      Operator* res = NULL;
      C *sop=NULL;
      
      t = dynamic_cast<C*>(O);
      if (t != NULL){
	 *found = t;
	 return t;
      }
      
      /* Crawl trough Sum operator*/
      OSum* sum;
      sum = dynamic_cast<OSum*>(O);
      if (sum != NULL){
	 for(int i=0; i < sum->Size(); i++){
	     res = FindOperatorType<C>((*sum)[i], &sop);
	     if (res != NULL){
		*found = sop;
		return res;
	     }
	 }
      }
      
      /* Crawl trough Multistate operator*/
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*>(O);
      if (ms != NULL){
	 OMultistate* package = new OMultistate();
	 package->States(ms->States());
	 for(int i=0; i < ms->States(); i++){
	    for(int j=0; j < ms->States(); j++){
	       if (ms->State(i,j) != NULL){
		  res = FindOperatorType<C>(ms->State(i,j), &sop);
		  if (res != NULL){
		     package->Add(res, i, j);
		     *found = sop;
		     return package;
		  }
	       }
	    }
	 }
	 delete package;
      }
      
      return NULL;
   }

} /* namespace QDLIB */

#endif
