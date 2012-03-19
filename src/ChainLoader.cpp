#include "ChainLoader.h"


#include "modules/ModuleLoader.h"
#include "tools/QDGlobalClock.h"
#include "tools/Logger.h"

#include "qdlib/OSum.h"
#include "qdlib/OGridSum.h"
#include "qdlib/OGridsystem.h"
#include "qdlib/OMultistate.h"
#include "qdlib/ODMultistate.h"
#include "qdlib/WFMultistate.h"
#include "qdlib/FileWF.h"

namespace QDLIB
{
   
   /**
    * Recursive method to load an operator chain.
    * 
    * Should recognize special collective operators (OSum, OGridSum, OMultistate)
    *
    * \param Onode       The XML node conataining the operator definition
    *
    * Registering the operator means that the GlobalOpList takes responsibility for destruction.
    */
   Operator* ChainLoader::LoadOperatorChain( XmlNode * Onode, bool persist )
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      GlobalOpList& OpList = GlobalOpList::Instance();
      
      ParamContainer pm;
      string name, key, ref, label;
      Operator *O=NULL;
      XmlNode *child;
      
      pm = Onode->Attributes();
      pm.GetValue( "name", name );
      
      if (pm.isPresent("ref")) {
         pm.GetValue("ref", ref);
	 log.cout() << "Reference to " << ref << endl << endl;
         return OpList[ref];
      }

      pm.GetValue("key", key);

      if (name == "GridSum") { /* Grid sum operator */
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OGridSystem *gsub;
	 OGridSum *sum = new OGridSum;
	 CollectorOp::Instance()->Register(sum);
	 while (child->EndNode()){
	    osub = LoadOperatorChain( child, persist );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    if ( (gsub = dynamic_cast<OGridSystem*>(osub) ) == NULL )
	       throw ( EIncompatible("Tried to load into GridSum"), osub->Name() );
	    sum->Add( gsub );
	    child->NextNode();
	 }
	 
	 log.flush();
	 log.IndentDec();
	 
	 O = sum;
      } else if (name == "Multistate" || name == "DMultistate") { /* Matrix of operators */
	 log.cout() << "Multistate operator:\n";
	 log.IndentInc();
	 child = Onode->NextChild();
	 child->AdjustElementNode();
	 Operator *osub;
	 OMultistate *matrix = NULL;
         if (name == "Multistate") matrix = new OMultistate();
         if (name == "DMultistate") matrix = new ODMultistate();
	 CollectorOp::Instance()->Register(matrix);
	 
	 matrix->Init(pm);
	 while (child->EndNode()){
	    string name;
	    stringstream ss_row, ss_col;
	    int row, col;
	    
	    name = child->Name();
	    ss_row << name.substr(1, name.find('.')-1);
	    ss_col << name.substr(name.find('.')+1);
	    ss_row >> row;
	    ss_col >> col;
	    
	    log.cout() << "H(" << row << "," << col << ")\n";
	    
	    osub = LoadOperatorChain( child, persist );
	    if (osub == NULL)
	       throw ( EParamProblem("Can't load operator") );
	    
	    
	    matrix->Add(osub, row, col);
	    child->NextNode();
         }
         O = matrix;
	 
	 log.flush();
	 log.IndentDec();
      } else { 
	 O = mods->LoadOp( name );
         
         if (dynamic_cast<OList*>(O) != NULL){ /* Sum/Product operator and propagators */
            log.cout() << O->Name()<<endl;
            log.IndentInc();
            log.flush();
            child = Onode->NextChild();

            O->Init(pm);

            if (child != NULL){ /* Check if sub operators are given */
               child->AdjustElementNode();
         
               Operator *osub;
               OList *list = dynamic_cast<OList*>(O);


               while (child->EndNode()){
                  osub = LoadOperatorChain( child, persist );
                  if (osub == NULL)
                     throw ( EParamProblem("Can't load operator") );
                  list->Add(child->Name(), osub );
                  child->NextNode();
               }
            }

            log.flush();
            log.IndentDec();
         } else {
         
            log.cout() << pm << "---------------\n";
            O->Init(pm);
            log.flush();
         }
      }
      
      /* Register & prepare to exit */
      OpList.Add(key, O, persist);

      if (pm.isPresent("label")){
         pm.GetValue("label", label);
         OpList.SetLabel(key, label);
      }

      return O;
   }


   
   /**
    * Load all wave functions from the input.
    * 
    * This method also recognizes Multistate, LC (Linear combination).
    * Works infinetly recursive.
    * 
    * \li Special Parameter:
    * \li normalize:  Works on every level (default false)
    * 
    * \li coeff   Coefficient in LC
    * \li coeff2  Coefficient squared  in LC
    * 
    */
   WaveFunction * ChainLoader::LoadWaveFunctionChain( XmlNode * WFNode, int seqnum)
   {
      ModuleLoader* mods = ModuleLoader::Instance();
      Logger& log = Logger::InstanceRef();
      
      ParamContainer pm;
      string name;
      WaveFunction *WF=NULL;
      XmlNode *child;
      bool onoff;
      
      pm = WFNode->Attributes();
      pm.GetValue( "name", name );
      
      if (name == "Multistate"){ /* Further recursion for multistate WF */
	 log.cout() << "Multi state wave function:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
         
         WaveFunction *wfsub;
         
         /* Read single WF definitions into Multistate */
         WFMultistate *multi = new WFMultistate();
	 CollectorWF::Instance()->Register(multi);
         while (child->EndNode()){
            string name;
            stringstream ss;
            int state;
            
            name = child->Name();
            ss << name.substr(2);
            ss >> state;
            log.cout() << "-State " << state << endl;
            
            wfsub = LoadWaveFunctionChain( child, seqnum );
            
            multi->Add( wfsub, state);
            child->NextNode();
         }
         
	 multi->Init(pm);
         
         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << multi->Norm() <<endl;
         }
         
	 pm.GetValue( "normalize", onoff);
	 if ( onoff) {
	    log.cout() << "Normalized\n";
	    multi->Normalize();
	 }
	 log.IndentDec();
	 
	 return multi;
      } else if (name == "LC"){ /* Further recursion for linear combination */
	 log.cout() << "Build linear combination from wave functions:" << endl;
	 log.IndentInc();
	 child = WFNode->NextChild();
	 child->AdjustElementNode();
	 WaveFunction *wfadd;
	 ParamContainer pm_child;
	 while (child->EndNode()){
            double coeff=0;
            
            pm_child = child->Attributes();
            if (pm_child.isPresent("coeff")){
               pm_child.GetValue("coeff", coeff);
               
            } else if (pm_child.isPresent("coeff2")){
               pm_child.GetValue("coeff2", coeff);
               coeff = sqrt(coeff);
            }
            if(coeff != 0){
               log.cout().precision(8);
               log.cout() << fixed << "Coefficient = " << coeff << endl;
               log.cout() << fixed << "Coefficient^2 = " << coeff*coeff << endl;
            }
                    
            wfadd = LoadWaveFunctionChain( child, seqnum );
            
            if(coeff != 0)
               MultElements((cVec*) wfadd, coeff);
            
	    if (WF == NULL){
	       WF = wfadd->NewInstance();
	       *((cVec*) WF) = dcomplex(0,0);
	    }
	    
	    *WF += wfadd;
	    DELETE_WF(wfadd);
	    child->NextNode();
	 }
         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << WF->Norm() <<endl;
         }


	 pm.GetValue( "normalize", onoff);
	 if ( onoff) {
	    log.cout() << "Normalized\n";
	    WF->Normalize();
	 }
	 log.IndentDec();
	 return WF; 
      } else { /* load a specific wf */
	 string fname;
	 
	 if ( name.length() != 1 ){ /* modules name is given */
	    WF = mods->LoadWF( name );
	    if (WF == NULL)
	       throw ( EParamProblem("WaveFunction module loading failed") );
	 }
	    
	 if (!pm.isPresent("file"))
	    throw ( EParamProblem("No file for loading wave function given") );
	 
	 /* load wf */
	 FileWF file;
         
	 pm.GetValue( "file", fname );
         file.Suffix(BINARY_WF_SUFFIX);
         file.Name(fname);
         if (seqnum > -1){ /* Read file from a sequence => only basename is given */
            file.ActivateSequence();
            file.Counter(seqnum);
         }
	 
	 if ( name.length() != 1 ) /* modules name is given */
            file >> WF;
	 else                      /* Load by meta */
	    file >> &WF;
	 
         /* Print norm */
         if (log.Debug()){
            log.coutdbg().precision(8);
            log.coutdbg() << fixed;
            log.coutdbg() << "Norm : " << WF->Norm() <<endl;
         }
         
         pm.GetValue( "normalize", onoff);
         if ( onoff) {
            log.cout() << "Normalizing...\n";
            WF->Normalize();
         }
         if (pm.isPresent("phase")){
            double phase;
            pm.GetValue("phase", phase);
            log.cout() << "Apply phase factor: " << phase << " pi\n";
            *WF *= cexpI(phase * M_PI);
         }
	 
	 if ( name.length() == 1 ) log.cout() << WF->Params() << endl;
	 log.cout() << pm << "------------------\n" << endl;
      }
      return WF;
   }
   
   
   /**
    * Load a propagator module and depending operators.
    * 
    * \param Unode   The XmlNode containing the parameters.
    * \param persist Make the Propagator persistent in the GlobalOpList.
    * \return The pre-initialized propagator (still need a Init(WF) )
    */
   OPropagator* ChainLoader::LoadPropagator( XmlNode *Unode, bool persist)
   {
      Operator* O = LoadOperatorChain(Unode, persist);
      OPropagator* U = dynamic_cast<OPropagator*>(O);

      if (U == NULL)
         throw (EIncompatible("This is not a propagator!"), O->Name());

      return U;
   }
   
   

}
