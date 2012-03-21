#include "InternalMods.h"

/* Operator list */
#include "qdlib/OSum.h"
#include "qdlib/OProduct.h"

#include "qdlib/OCheby.h"
#include "qdlib/OSPO.h"
#include "qdlib/OGSPO.h"
#include "qdlib/OSIL.h"
#include "qdlib/ORK4.h"

#include "qdlib/OGridNablaSq.h"
#include "qdlib/OGridGMat.h"
#include "qdlib/OGridPotential.h"
#include "qdlib/OGridDipole.h"
#include "qdlib/OGridRaman.h"
#include "qdlib/OGridPosition.h"
#include "qdlib/OGridNAC.h"
#include "qdlib/OGridKick.h"
#include "qdlib/OGridStokes.h"
#include "qdlib/OGridGStokes.h"
#include "qdlib/OGridStokesAlbrecht.h"

#include "qdlib/OHermitianMatrix.h"
#include "qdlib/OMatDipole.h"

#include "qdlib/OGobbler.h"
#include "qdlib/OFlux.h"
#include "qdlib/OProjection.h"
#include "qdlib/OScalar.h"

/* Wavefunction list */
#include "qdlib/WFGridCartesian.h"
#include "qdlib/WFLevel.h"

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
   QDNG_OPERATOR_INSTANCE_FUNCTION(OSum)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OProduct)

   QDNG_OPERATOR_INSTANCE_FUNCTION(OCheby)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGSPO)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OSIL)
   QDNG_OPERATOR_INSTANCE_FUNCTION(ORK4)

   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridNablaSq)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridGMat)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridPotential)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridDipole)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridRaman)
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
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridStokes)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridGStokes)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGridStokesAlbrecht)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OGobbler)
   QDNG_OPERATOR_INSTANCE_FUNCTION(OFlux)
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
      QDNG_STRING_TO_INSTANCE_PTR(OSum)
      QDNG_STRING_TO_INSTANCE_PTR(OProduct)

      QDNG_STRING_TO_INSTANCE_PTR(OCheby)
      QDNG_STRING_TO_INSTANCE_PTR(OGSPO)
      QDNG_STRING_TO_INSTANCE_PTR(OSIL)
      QDNG_STRING_TO_INSTANCE_PTR(ORK4)

      QDNG_STRING_TO_INSTANCE_PTR(OGridNablaSq)
      QDNG_STRING_TO_INSTANCE_PTR(OGridGMat)
      QDNG_STRING_TO_INSTANCE_PTR(OGridPotential)
      QDNG_STRING_TO_INSTANCE_PTR(OGridDipole)
      QDNG_STRING_TO_INSTANCE_PTR(OGridRaman)
      QDNG_STRING_TO_INSTANCE_PTR(OGridPosition)

      /* This is an alias */
      if (name == "INT_OGridMomentum") return &INT_Alias_OGridMomentum;

      QDNG_STRING_TO_INSTANCE_PTR(OGridNabla)
      QDNG_STRING_TO_INSTANCE_PTR(OGridNAC)
      QDNG_STRING_TO_INSTANCE_PTR(OGridKick)
      QDNG_STRING_TO_INSTANCE_PTR(OGridStokes)
      QDNG_STRING_TO_INSTANCE_PTR(OGridGStokes)
      QDNG_STRING_TO_INSTANCE_PTR(OGridStokesAlbrecht)

      QDNG_STRING_TO_INSTANCE_PTR(OHermitianMatrix)
      QDNG_STRING_TO_INSTANCE_PTR(OMatDipole)
      QDNG_STRING_TO_INSTANCE_PTR(OGobbler)
      QDNG_STRING_TO_INSTANCE_PTR(OFlux)
      QDNG_STRING_TO_INSTANCE_PTR(OProjection)
      QDNG_STRING_TO_INSTANCE_PTR(OScalar)

      return NULL;
   }

}
