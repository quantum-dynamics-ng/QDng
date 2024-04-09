// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "OJump.h"
#include "tools/Logger.h"
#include <stdlib.h>

// Maximum allowed jump propability
#define MAX_PJUMP 0.5

namespace QDLIB {

   QDNG_OPERATOR_NEW_INSTANCE_FUNCTION(OJump)

    OJump::OJump()
            : OList(), _name("OJump"), _max_pjump(MAX_PJUMP)
    {
    }

    OJump::~OJump()
    {
    }

    void OJump::Init(ParamContainer &params)
    {
      long int seed = 0; // default seed
      unsigned short seed16v[3];
      const unsigned short ones16 = 65535;

      params.GetValue("seed", seed);

      if (seed < 0 )
	throw EParamProblem("Invalid seed");


      // initialize random number generator
      seed16v[0] = ones16 & seed;
      seed16v[1] = ones16 & (seed >> 16);
      seed16v[2] = ones16 & (seed >> 32);

      seed48(seed16v);

      if (params.isPresent("max_pjump"))
        params.GetValue("max_pjump", _max_pjump);
    }

    dcomplex OJump::MatrixElement(WaveFunction * PsiBra, WaveFunction * PsiKet)
    {
      throw EIncompatible("OJump::MatrixElement is not implemented");
    }

    void OJump::Apply(WaveFunction * destPsi, WaveFunction * sourcePsi)
    {
      *destPsi = sourcePsi;
      Apply(destPsi);
    }

    void OJump::Apply(WaveFunction *Psi)
    {
       WaveFunction *buf = Psi->NewInstance();
       std::vector<double> csum(Size());

       Psi->Normalize();

       // calculate Pjump for all operators
       for (int i=0; i < Size(); i++) {
          Get(i)->Apply(buf, Psi);
	        double pjump = clock->Dt() * (*buf * buf).real();
	        csum[i] = pjump + ( (i > 0) ? csum[i-1] : 0.0 );
       }

       if (csum.back() >= _max_pjump)
	        throw EOverflow("OJump::Apply: jump propability greater than max_pjump");

       DELETE_WF(buf);

       // decide jump
       double rnd = drand48();

       for (int i=0; i < Size(); i++){
          if (csum[i] > rnd) {
	    Logger& log = Logger::InstanceRef();
	    log.coutdbg() << "Apply jump operator " << i << " at t = " << clock->Time() << endl;

	    // jump and normalize
	    Get(i)->Apply(Psi);
	    Psi->Normalize();
	    break;
	  }
       }
    }
}
