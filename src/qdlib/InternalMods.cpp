#include "InternalMods.h"

/* Operator list */
#include "OCheby.h"
#include "OSPO.h"
#include "OGSPO.h"
#include "OSIL.h"

#include "OGridNablaSq.h"
#include "OGridGMat.h"
#include "OGridPotential.h"
#include "OGridDipole.h"
#include "OGridPosition.h"
#include "OGridNAC.h"
#include "OGridKick.h"

#include "OHermitianMatrix.h"
#include "OMatDipole.h"

#include "OGobbler.h"
#include "OProjection.h"
#include "OScalar.h"

/* Wavefunction list */
#include "WFGridCartesian.h"
#include "WFLevel.h"

/**
 * Macro for creating functions which generates an instance of the class.
 * */
#define QDNG_OPERATOR_INSTANCE_FUNCTION(CLASSNAME) \
   Operator* INT_##CLASSNAME() \
   { \
      CLASSNAME *p; \
      p = new CLASSNAME (); \
      CollectorOp::Instance()->Register(p); \
      return p; \
   }

#define QDNG_WAVEFUNCTION_INSTANCE_FUNCTION(CLASSNAME) \
   WaveFunction* INT_##CLASSNAME() \
   { \
      CLASSNAME* p; \
      p = new CLASSNAME(); \
      CollectorWF::Instance()->Register(p); \
      return p; \
   }

#define QDNG_STRING_TO_INSTANCE_PTR(CLASSNAME) \
   if (name == "INT_" #CLASSNAME) return &INT_##CLASSNAME;

namespace QDLIB
{

   /* Instance functions  */

   QDNG_OPERATOR_INSTANCE_FUNCTION(OCheby)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGSPO)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OSPO)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OSIL)

   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridNablaSq)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridGMat)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridPotential)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridDipole)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridPosition)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridNabla)

   /* Leave this here because we have an individual Constructor */
   Operator* INT_Alias_OGridMomentum()
   {
      OGridNabla *p;
      p = new OGridNabla(true);
      CollectorOp::Instance()->Register(p);
      return p;
   }

   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridNAC)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridKick)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGobbler)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OProjection)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OScalar)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OHermitianMatrix)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OMatDipole)

   QDNG_WAVEFUNCTION_INSTANCE_FUNCTION(WFGridCartesian)
   QDNG_WAVEFUNCTION_INSTANCE_FUNCTION(WFLevel)

   instWF* InternalWF(const string &name)
   {
      QDNG_STRING_TO_INSTANCE_PTR(WFGridCartesian)
      QDNG_STRING_TO_INSTANCE_PTR(WFLevel)

      return NULL;
   }

   instOP* InternalOP(const string &name)
   {
      QDNG_STRING_TO_INSTANCE_PTR(OCheby)
      QDNG_STRING_TO_INSTANCE_PTR(OSPO)
      QDNG_STRING_TO_INSTANCE_PTR(OGSPO)
      QDNG_STRING_TO_INSTANCE_PTR(OSIL)

      QDNG_STRING_TO_INSTANCE_PTR(OGridNablaSq)
      QDNG_STRING_TO_INSTANCE_PTR(OGridGMat)
      QDNG_STRING_TO_INSTANCE_PTR(OGridPotential)
      QDNG_STRING_TO_INSTANCE_PTR(OGridDipole)
      QDNG_STRING_TO_INSTANCE_PTR(OGridPosition)

      /* This is an alias */
      if (name == "INT_OGridMomentum") return &INT_Alias_OGridMomentum;

      QDNG_STRING_TO_INSTANCE_PTR(OGridNabla)
      QDNG_STRING_TO_INSTANCE_PTR(OGridNAC)
      QDNG_STRING_TO_INSTANCE_PTR(OGridKick)

      QDNG_STRING_TO_INSTANCE_PTR(OHermitianMatrix)
      QDNG_STRING_TO_INSTANCE_PTR(OMatDipole)
      QDNG_STRING_TO_INSTANCE_PTR(OGobbler)
      QDNG_STRING_TO_INSTANCE_PTR(OProjection)
      QDNG_STRING_TO_INSTANCE_PTR(OScalar)

      return NULL;
   }

}
