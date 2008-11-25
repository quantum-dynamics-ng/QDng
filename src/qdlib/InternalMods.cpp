#include "InternalMods.h"


/* Operator list */
#include "OCheby.h"
#include "OSPO.h"
#include "OSOD.h"

#include "OGridNablaSq.h"
#include "OGridGMat.h"
#include "OGridPotential.h"

#include "OGobbler.h"

/* Wavefunction list */
#include "WFGridCartesian.h"
#include "WFLevel.h"


namespace QDLIB
{


   /* Instance functions  */
   Operator* INT_OCheby()
   {
      OCheby *p;
      p = new OCheby ();
      return p;
   }

   Operator* INT_OSPO()
   {
      OSPO *p;
      p = new OSPO ();
      return p;
   }
   
   Operator* INT_OSOD()
   {
      OSOD *p;
      p = new OSOD ();
      return p;
   }
   
   Operator* INT_OGridNablaSq()
   {
      OGridNablaSq *p;
      p = new OGridNablaSq ();
      return p;
   }
   
   Operator* INT_OGridGMat()
   {
      OGridGMat *p;
      p = new OGridGMat ();
      return p;
   }
   
   Operator* INT_OGridPotential()
   {
      OGridPotential *p;
      p = new OGridPotential ();
      return p;
   }
   
   
   WaveFunction* INT_WFGridCartesian()
   {
      WFGridCartesian* p;
      p = new WFGridCartesian();
      return p;
   }
   
   
   WaveFunction* INT_WFLevel()
   {
      WFLevel* p;
      p = new WFLevel();
      return p;
   }	    
   
   instWF* InternalWF(const string &name)
   {
      if (name == "INT_WFGridCartesian") return &INT_WFGridCartesian;
      if (name == "INT_WFLevel") return &INT_WFLevel;
      return NULL;
   }
   
   instOP* InternalOP(const string &name)
   {
      if (name == "INT_OCheby") return &INT_OCheby;
      if (name == "INT_OSPO") return &INT_OSPO;
      if (name == "INT_OSOD") return &INT_OSOD;
      
      if (name == "INT_OGridNablaSq") return &INT_OGridNablaSq;
      if (name == "INT_OGridGmat") return &INT_OGridGMat;
      if (name == "INT_OGridPotential") return &INT_OGridPotential;
      return NULL;
   }


}
