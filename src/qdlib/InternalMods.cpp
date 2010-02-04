#include "InternalMods.h"


/* Operator list */
#include "OCheby.h"
#include "OSPO.h"
#include "OSIL.h"

#include "OGridNablaSq.h"
#include "OGridGMat.h"
#include "OGridPotential.h"
#include "OGridDipole.h"
#include "OGridPosition.h"
#include "OGridNAC.h"

#include "OHermitianMatrix.h"

#include "OGobbler.h"
#include "OProjection.h"
#include "OScalar.h"

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
   
   Operator* INT_OSIL()
   {
      OSIL *p;
      p = new OSIL ();
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
   
   Operator* INT_OGridDipole()
   {
      OGridDipole *p;
      p = new OGridDipole ();
      return p;
   }
   
   Operator* INT_OGridPosition()
   {
      OGridPosition *p;
      p = new OGridPosition();
      return p;
   }
   
   Operator* INT_OGridNabla()
   {
      OGridNabla *p;
      p = new OGridNabla();
      return p;
   }
   
   Operator* INT_Alias_OGridMomentum()
   {
      OGridNabla *p;
      p = new OGridNabla(true);
      return p;
   }
   
   Operator* INT_OGridNAC()
   {
      OGridNAC *p;
      p = new OGridNAC();
      return p;
   }
   
   Operator* INT_OGobbler()
   {
      OGobbler *p;
      p = new OGobbler ();
      return p;
   }

   Operator* INT_OProjection()
   {
      OProjection *p;
      p = new OProjection ();
      return p;
   }

   Operator* INT_OScalar()
   {
      OScalar *p;
      p = new OScalar ();
      return p;
   }

   Operator* INT_OHermitianMatrix()
   {
      OHermitianMatrix *p;
      p = new OHermitianMatrix ();
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
      if (name == "INT_OSIL") return &INT_OSIL;
      
      if (name == "INT_OGridNablaSq") return &INT_OGridNablaSq;
      if (name == "INT_OGridGmat") return &INT_OGridGMat;
      if (name == "INT_OGridPotential") return &INT_OGridPotential;
      if (name == "INT_OGridDipole") return &INT_OGridDipole;
      if (name == "INT_OGridPosition") return &INT_OGridPosition;
      if (name == "INT_OGridMomentum") return &INT_Alias_OGridMomentum;
      if (name == "INT_OGridNabla") return &INT_OGridNabla;
      if (name == "INT_OGridNAC") return &INT_OGridNAC;
      
      if (name == "INT_OHermitianMatrix") return &INT_OHermitianMatrix;
      if (name == "INT_OGobbler") return &INT_OGobbler;
      if (name == "INT_OProjection") return &INT_OProjection;
      if (name == "INT_OScalar") return &INT_OScalar;
      
      return NULL;
   }


}
