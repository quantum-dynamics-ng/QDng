#ifndef QDLIBCHAINLOADER_H
#define QDLIBCHAINLOADER_H

#include "qdlib/OPropagator.h"
#include "sys/XmlNode.h"


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

}

#endif
