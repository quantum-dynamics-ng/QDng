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
	 static WaveFunction* LoadWaveFunctionChain( XmlNode *WFNode, int seqnum=-1);
	 static OPropagator* LoadPropagator( XmlNode *Unode, Operator **Hamiltonian);
   };

   /**
    * Find a specific type of operator in Chain of Sum/Multistate operators.
    * 
    * \param O       The Operator to inspect.
    * \param found   If type is found the naked operator will be placed here
    * \param n       [in] max. number to find, [out] actual number of operators found.
    * \param label   An optional label can be specified to mark the desiered operators (and leave the rest untouched)
    * 
    * \return The whole Operator with packaging (single state or multistate)
    */
   template <class C>
   Operator* FindOperatorType(Operator *O, C **found, int &n, const int index=0)
   {
      C* t;
      Operator* res = NULL;
      C **sop;
      int nops=0;
      int max;
      
      t = dynamic_cast<C*>(O);
      if (t != NULL){
         n=1;
	 found[index] = t;
         cout << n <<" " << index << ": " << t->Name() << endl;
	 return t;
      }
      
      /* Crawl trough Sum operator*/
      OSum* sum;
      sum = dynamic_cast<OSum*>(O);
      if (sum != NULL){
         sop = new C*[n];
	 for(int i=0; i < sum->Size(); i++){
             max=n;
             cout << "Checking SUM: ("<< n <<") "<< i  <<" " << index +nops<< " " << endl;
	     res = FindOperatorType<C>((*sum)[i], sop, max, index+nops);
	     if (res != NULL){
                for (int k=0; k < max; k++){
                  found[index+nops+k] = sop[k];
                  /* Abort if limit is reached prematurely */
                  if (nops+k >= n){
                     cout << "n: " << n << endl;
                     delete[] sop;
                     return res;
                  }
                }
                nops += max;
	     }
	 }
         n = nops;
         cout << "n: " << n << " nops: " << nops << endl;
         delete[] sop;
         return res;
      }
      
      /* Crawl trough Multistate operator*/
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*>(O);
      if (ms != NULL){
	 OMultistate* package = new OMultistate();
         sop = new C*[n];
	 package->States(ms->States());
	 for(int i=0; i < ms->States(); i++){
            for(int j=0; j < i; j++){ /** \bug Only works correctly with hermitian ms */
	       if (ms->State(i,j) != NULL){
                  max=n;
                  cout << "Checking MS: (" << n <<") "<< i << ","<< j <<" " << index +nops<< " " << endl;
                  res = FindOperatorType<C>(ms->State(i,j), sop, max,index+nops);
		  if (res != NULL){
		     package->Add(res, i, j);
                     for (int k=0; k < max; k++){
                        found[index+nops+k] = sop[k];
                        if (nops+k >= n){
                           cout << "n: " << n << endl;
                           delete[] sop;
                           return res;
                        }
                     }
                     nops += max;
		  }
                  /* Abort if limit is reached prematurely */
                  if (nops > n){
                     n = nops;
                     cout << "n: " << n << endl;
                     delete[] sop;
                     return package;
                  }
	       }
	    }
	 }
         n = nops;
         cout << "n: " << n << " nops: " << nops << endl;
         delete[] sop;
         return package;
      }
      
      return NULL;
   }

} /* namespace QDLIB */

#endif
