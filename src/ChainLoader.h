// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#ifndef QDLIBCHAINLOADER_H
#define QDLIBCHAINLOADER_H

#include "qdlib/OPropagator.h"
#include "tools/XmlNode.h"
#include "qdlib/OSum.h"
#include "qdlib/OMultistate.h"

#include "GlobalOpList.h"

namespace QDLIB
{

   /**
    * Contains some methods to load Operators and WaveFunctions from XML input.
    * 
    * @author Markus Kowalewski <markus.kowalewski@cup.uni-muenchen.de>
    */
   class ChainLoader
   {
      private:
         static int OpListAnonKey; /* Counter for anonymous key names */

         static WaveFunction* LoadWFMultimap_( XmlNode *WFNode, ParamContainer& pm, int seqnum);
         static WaveFunction* LoadWFMultistate_( XmlNode *WFNode, ParamContainer& pm, int seqnum);
         static WaveFunction* LoadWFLC_( XmlNode *WFNode, ParamContainer& pm, int seqnum);
      public:
	 static Operator* LoadOperatorChain( XmlNode *Onode, bool persist = false );
	 static WaveFunction* LoadWaveFunctionChain( XmlNode *WFNode, int seqnum=-1);
         static OPropagator* LoadPropagator( XmlNode *Unode, bool persist = false );
   };

   /**
    * Find a specific type of operator in Chain of Sum/Multistate operators.
    * 
    * \param O       The Operator to inspect.
    * \param found   If type is found the naked operator will be placed here
    * \param n       [in] max. number to find, [out] actual number of operators found.
    * \param label   An optional label can be specified to mark the desired operators (and leave the rest untouched)
    * 
    * \return The whole Operator with packaging (single state or multistate)
    */
   template <class C>
   Operator* FindOperatorType(Operator *O, C **found, int &n, const string *label = NULL, const int index=0)
   {
      C* t;
      Operator* res = NULL;
      C **sop;
      int nops=0;
      int max;
      
      t = dynamic_cast<C*>(O);
      if (t != NULL){
         /* Check for label */
         if (label != NULL){
               if (! GlobalOpList::Instance().CheckLabel(O, *label))
                  return NULL;
         }
         /* return successful result */
         n=1;
         found[index] = t;
         return t;
      }
      
      /* Crawl trough List operators: Sum, Products, etc. */
      OList* list;
      list = dynamic_cast<OList*>(O);
      if (list != NULL){
         sop = new C*[n];
         OList* package = dynamic_cast<OList*>(list->NewInstance());
	 for(int i=0; i < list->Size(); i++){
             max=n;
             res = FindOperatorType<C>((*list)[i], sop, max, label, index+nops);
	     if (res != NULL){
                package->Add(res);
                for (int k=0; k < max; k++){
                  found[index+nops+k] = sop[index+nops+k];
                  /* Abort if limit is reached prematurely */
                  if (nops+k >= n){
                     delete[] sop;
                     return res;
                  }
                }
                nops += max;
	     }
	 }
         n = nops;
         delete[] sop;
         if (nops == 0) {
            delete package;
            return NULL;
         } else {
            return package;
         }
      }
      
      /* Crawl trough Multistate operator*/
      OMultistate* ms;
      ms = dynamic_cast<OMultistate*>(O);
      if (ms != NULL){
	 OMultistate* package = new OMultistate();
         sop = new C*[n];
	 package->States(ms->States());
	 for(int i=0; i < ms->States(); i++){
            for(int j=0; j < i; j++){
	       if (ms->State(i,j) != NULL){ 
                  max=n;
                  res = FindOperatorType<C>(ms->State(i,j), sop, max, label, index+nops);
		  if (res != NULL){
		     package->Add(res, i, j);
                     for (int k=0; k < max; k++){
                        found[index+nops+k] = sop[index+nops+k];
                        if (nops+k >= n){
                           delete[] sop;
                           return package;
                        }
                     }
                     nops += max;
		     if (ms->State(i,j) != ms->State(j,i) && ms->State(j,i) != NULL) {
                        cout << "Look for NH counterpart: " << ms->State(i,j) << " " << ms->State(j,i) << endl;
                        cout << "Label: " << *label << endl;
                        max=n;
                        res = FindOperatorType<C>(ms->State(j,i), sop, max, label, index+nops);
		        if (res != NULL){
                           cout << "Found an j,i" << ms->State(j,i)->Name()  << endl;
		           package->Add(res, j, i);
                           for (int k=0; k < max; k++){
                              found[index+nops+k] = sop[index+nops+k];
                              if (nops+k >= n){
                                 delete[] sop;
                                 return package;
                              }
                           }
                           nops += max;
                        }
		     }
		  }
                  /* Abort if limit is reached prematurely */
                  if (nops > n){
                     n = nops;
                     delete[] sop;
                     if (nops == 0) {
                        delete package;
                        return NULL;
                     } else {
                        return package;
                     }
                  }
	       }
	    }
	 }
         n = nops;
         delete[] sop;
         if (nops == 0) {
            delete package;
            return NULL;
         } else {
            return package;
         }
      }
      
      n = 0;
      return NULL;
   }

} /* namespace QDLIB */

#endif
